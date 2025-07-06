#include "../include/ExhibitionArea.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Sergi alanını kurma
ExhibitionArea::ExhibitionArea() : shaderProgram(0) {
}

// Shader'ları silme
ExhibitionArea::~ExhibitionArea() {
    glDeleteProgram(shaderProgram);
}

// Sergi alanını başlatma
bool ExhibitionArea::initialize() {
    createShaders();
    
    placeObjects();
    
    return true;
}

// Sergi alanını ekrana çizer
void ExhibitionArea::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    glUseProgram(shaderProgram);
    
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    for (const auto& statue : statues) {
        statue->render(viewMatrix, projectionMatrix, shaderProgram);
    }
}

// Animasyonları günceller fakat eklemedik
void ExhibitionArea::update(float deltaTime) {
    applyAnimations();
}

// Model yükleyip listeye ekler
Statue& ExhibitionArea::loadModel(const std::string& filePath, 
                                 const glm::vec3& position, 
                                 const glm::vec3& scale, 
                                 const glm::vec3& rotation) {
    auto statue = std::make_unique<Statue>(filePath, position, scale, rotation);
    
    if (!statue->initialize()) {
        std::cerr << "Heykel başlatılamadı: " << filePath << std::endl;
    }
    
    statues.push_back(std::move(statue));
    return *statues.back();
}

const std::vector<std::unique_ptr<Statue>>& ExhibitionArea::getStatues() const {
    return statues;
}

// Işık ve gölge efektlerini  oluşturur
void ExhibitionArea::createShaders() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aNormal;
        layout (location = 2) in vec2 aTexCoord;
        
        out vec3 FragPos;
        out vec3 Normal;
        out vec2 TexCoord;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            FragPos = vec3(model * vec4(aPos, 1.0));
            Normal = mat3(transpose(inverse(model))) * aNormal;
            TexCoord = aTexCoord;
            
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        
        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;
        
            uniform vec3 objectColor = vec3(0.8, 0.8, 0.8);
        uniform float shininess = 32.0;
        
            uniform vec3 lightPos = vec3(0.0, 2.0, 0.0);
        uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
        uniform vec3 viewPos;
        
        void main() {
                    float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * lightColor;
            
                    vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = diff * lightColor;
            
                    float specularStrength = 0.5;
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
            vec3 specular = specularStrength * spec * lightColor;
            
            vec3 result = (ambient + diffuse + specular) * objectColor;
            FragColor = vec4(result, 1.0);
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

// Heykelleri sahneye yerleştirir.bilgiler hazir
void ExhibitionArea::placeObjects() {
    
    float floorY = -2.5f;
    
    loadModel("models/statue1.obj", glm::vec3(-5.0f, floorY, -5.0f), glm::vec3(1.0f), glm::vec3(0.0f));
    loadModel("models/statue2.obj", glm::vec3(5.0f, floorY, -5.0f), glm::vec3(1.0f), glm::vec3(0.0f, 90.0f, 0.0f));
    loadModel("models/statue3.obj", glm::vec3(0.0f, floorY, 5.0f), glm::vec3(1.0f), glm::vec3(0.0f, 180.0f, 0.0f));
    
    statues[0]->setInformation(
        "Venüs de Milo", 
        "Afrodit'i tasvir eden antik Yunan heykeli. MÖ 100 civarında yapılmıştır.", 
        "Aleksandros", 
        -100
    );
    
    statues[1]->setInformation(
        "David", 
        "Michelangelo'nun Rönesans döneminde yaptığı ünlü mermer heykel.", 
        "Michelangelo", 
        1504
    );
    
    statues[2]->setInformation(
        "Düşünen Adam", 
        "Auguste Rodin tarafından yapılan bronz ve mermer heykel.", 
        "Auguste Rodin", 
        1904
    );
}

void ExhibitionArea::applyAnimations() {
   
}
