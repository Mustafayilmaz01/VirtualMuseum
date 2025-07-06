#include "../include/Shader.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>


const char* Shader::vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoords;

    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoords;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main() {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        TexCoords = aTexCoords;
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
)";

const char* Shader::fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    in vec3 Normal;
    in vec3 FragPos;
    in vec2 TexCoords;

    
    uniform vec3 lightPos[4];
    uniform vec3 lightColors[4];
    uniform vec3 viewPos;
    uniform vec3 objectColor;
    uniform bool hasTexture;
    uniform bool useWallTexture;
    uniform sampler2D texture_diffuse1;
    uniform sampler2D wallTexture;

    void main() {
        
        float ambientStrength = 0.3;
        vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
        
        vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);
        
        
        vec3 diffuse = vec3(0.0, 0.0, 0.0);
        vec3 specular = vec3(0.0, 0.0, 0.0);
        
        
        for(int i = 0; i < 4; i++) {
           
            vec3 lightDir = normalize(lightPos[i] - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            diffuse += diff * lightColors[i] * 0.4;
            
            
            float specularStrength = 0.3;
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
            specular += specularStrength * spec * lightColors[i] * 0.4;
        }

        vec3 result;
        if (hasTexture) {
            vec3 texColor = texture(texture_diffuse1, TexCoords).rgb;
            result = (ambient + diffuse + specular) * texColor;
        } else if (useWallTexture) {
            vec3 wallColor = texture(wallTexture, TexCoords).rgb;
            result = (ambient + diffuse + specular) * wallColor;
        } else {
            result = (ambient + diffuse + specular) * objectColor;
        }

        FragColor = vec4(result, 1.0);
    }
)";

Shader::Shader() : ID(0) {
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

bool Shader::initialize() {
    ID = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    if (ID == 0) {
        std::cerr << "Shader programı oluşturulamadı!" << std::endl;
        return false;
    }
    std::cout << "Shader programı başarıyla oluşturuldu." << std::endl;
    return true;
}

void Shader::use() const {
    glUseProgram(ID);
}

unsigned int Shader::getID() const {
    return ID;
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3Array(const std::string &name, const glm::vec3 *values, int count) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), count, glm::value_ptr(values[0]));
}

unsigned int Shader::createShaderProgram(const char* vertexSource, const char* fragmentSource) {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexSource, nullptr);
    glCompileShader(vs);
    
   
    int success;
    char infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cerr << "Vertex shader derleme hatası: " << infoLog << std::endl;
        return 0;
    }
    
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentSource, nullptr);
    glCompileShader(fs);
    
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cerr << "Fragment shader derleme hatası: " << infoLog << std::endl;
        return 0;
    }
    
    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Shader program bağlama hatası: " << infoLog << std::endl;
        return 0;
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}
