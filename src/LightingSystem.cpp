#include "../include/LightingSystem.hpp"
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

LightingSystem::LightingSystem() : lightVAO(0), lightVBO(0), shaderProgram(0) {
}

LightingSystem::~LightingSystem() {
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &lightVBO);
    glDeleteProgram(shaderProgram);
}

bool LightingSystem::initialize() {
    createShaders();
    
    float vertices[] = {
        // Pozisyon
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
        -0.1f,  0.1f, -0.1f,
        
        -0.1f, -0.1f,  0.1f,
         0.1f, -0.1f,  0.1f,
         0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f
    };
    
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        7, 3, 0, 0, 4, 7,
        1, 5, 6, 6, 2, 1,
        0, 1, 5, 5, 4, 0,
        3, 2, 6, 6, 7, 3
    };
    
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);
    
    glBindVertexArray(lightVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    
    addAmbientLight(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.7f);
    
    addDirectionalLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
    addDirectionalLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
    addDirectionalLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
    addDirectionalLight(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.3f);
    
    addSpotLight(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 0.8f, 30.0f, 45.0f);
    
    return true;
}

void LightingSystem::apply(const glm::mat4& viewMatrix, 
                          const glm::mat4& projectionMatrix, 
                          GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    
    for (size_t i = 0; i < ambientLights.size(); ++i) {
        std::string prefix = "ambientLights[" + std::to_string(i) + "].";
        
        GLint posLoc = glGetUniformLocation(shaderProgram, (prefix + "position").c_str());
        GLint colorLoc = glGetUniformLocation(shaderProgram, (prefix + "color").c_str());
        GLint intensityLoc = glGetUniformLocation(shaderProgram, (prefix + "intensity").c_str());
        
        glUniform3fv(posLoc, 1, glm::value_ptr(ambientLights[i].position));
        glUniform3fv(colorLoc, 1, glm::value_ptr(ambientLights[i].color));
        glUniform1f(intensityLoc, ambientLights[i].intensity);
    }
    
    for (size_t i = 0; i < spotLights.size(); ++i) {
        std::string prefix = "spotLights[" + std::to_string(i) + "].";
        
        GLint posLoc = glGetUniformLocation(shaderProgram, (prefix + "position").c_str());
        GLint dirLoc = glGetUniformLocation(shaderProgram, (prefix + "direction").c_str());
        GLint colorLoc = glGetUniformLocation(shaderProgram, (prefix + "color").c_str());
        GLint intensityLoc = glGetUniformLocation(shaderProgram, (prefix + "intensity").c_str());
        GLint cutOffLoc = glGetUniformLocation(shaderProgram, (prefix + "cutOff").c_str());
        GLint outerCutOffLoc = glGetUniformLocation(shaderProgram, (prefix + "outerCutOff").c_str());
        
        glUniform3fv(posLoc, 1, glm::value_ptr(spotLights[i].position));
        glUniform3fv(dirLoc, 1, glm::value_ptr(spotLights[i].direction));
        glUniform3fv(colorLoc, 1, glm::value_ptr(spotLights[i].color));
        glUniform1f(intensityLoc, spotLights[i].intensity);
        glUniform1f(cutOffLoc, glm::cos(glm::radians(spotLights[i].cutOff)));
        glUniform1f(outerCutOffLoc, glm::cos(glm::radians(spotLights[i].outerCutOff)));
    }
    
    for (size_t i = 0; i < directionalLights.size(); ++i) {
        std::string prefix = "directionalLights[" + std::to_string(i) + "].";
        
        GLint dirLoc = glGetUniformLocation(shaderProgram, (prefix + "direction").c_str());
        GLint colorLoc = glGetUniformLocation(shaderProgram, (prefix + "color").c_str());
        GLint intensityLoc = glGetUniformLocation(shaderProgram, (prefix + "intensity").c_str());
        
        glUniform3fv(dirLoc, 1, glm::value_ptr(directionalLights[i].direction));
        glUniform3fv(colorLoc, 1, glm::value_ptr(directionalLights[i].color));
        glUniform1f(intensityLoc, directionalLights[i].intensity);
    }
    
    renderLightSources(viewMatrix, projectionMatrix);
}

void LightingSystem::update(float deltaTime) {
}

int LightingSystem::addAmbientLight(const glm::vec3& position, 
                                   const glm::vec3& color, 
                                   float intensity) {
    AmbientLight light;
    light.position = position;
    light.color = color;
    light.intensity = intensity;
    
    ambientLights.push_back(light);
    return ambientLights.size() - 1;
}

int LightingSystem::addSpotLight(const glm::vec3& position, 
                                const glm::vec3& direction, 
                                const glm::vec3& color, 
                                float intensity, 
                                float cutOff, 
                                float outerCutOff) {
    SpotLight light;
    light.position = position;
    light.direction = direction;
    light.color = color;
    light.intensity = intensity;
    light.cutOff = cutOff;
    light.outerCutOff = outerCutOff;
    
    spotLights.push_back(light);
    return spotLights.size() - 1;
}

int LightingSystem::addDirectionalLight(const glm::vec3& direction, 
                                       const glm::vec3& color, 
                                       float intensity) {
    DirectionalLight light;
    light.direction = direction;
    light.color = color;
    light.intensity = intensity;
    
    directionalLights.push_back(light);
    return directionalLights.size() - 1;
}

void LightingSystem::createShaders() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        uniform vec3 lightColor;
        
        void main() {
            FragColor = vec4(lightColor, 1.0);
        }
    )";
    
  
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
  
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
   
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
  
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void LightingSystem::renderLightSources(const glm::mat4& viewMatrix, 
                                       const glm::mat4& projectionMatrix) {
    
    glUseProgram(shaderProgram);
    
    
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
   
    for (const auto& light : ambientLights) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light.position);
        
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.color));
        
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    
    for (const auto& light : spotLights) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, light.position);
        
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
        glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.color));
        
        glBindVertexArray(lightVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);
}
