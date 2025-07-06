#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Statue.hpp"
#include "Robot.hpp"
#include "UIManager.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool initialize(float roomWidth, float roomHeight, float roomLength);

    void cleanup();

    void renderScene(GLFWwindow* window, const std::vector<Statue*>& statues, Robot* robot, 
                    UIManager* uiManager, const glm::vec3& cameraPos, const glm::mat4& viewMatrix, 
                    const glm::mat4& projectionMatrix, int activeStatueIndex, bool showInfoPanel, 
                    bool isScanning, float scanProgress, float scanDuration);

private:
    unsigned int shaderProgram;
    
    unsigned int roomVAO, roomVBO;
    
    unsigned int wallTextureID;
    
    float roomWidth, roomHeight, roomLength;
    
    GLint modelLoc, viewLoc, projLoc, objectColorLoc, lightPosLoc, lightColorsLoc, viewPosLoc;
    
    void setupRoom();
    void setupShaders();
    void setupLights();
    unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
    unsigned int loadTexture(const char* path);
    
    void renderRoom(const glm::mat4& view, const glm::mat4& projection);
    void renderStatues(const std::vector<Statue*>& statues, const glm::mat4& view, const glm::mat4& projection);
    void renderRobot(Robot* robot, const glm::mat4& view, const glm::mat4& projection);
    
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
            gl_Position = projection * view * vec4(FragPos, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;

        in vec3 FragPos;
        in vec3 Normal;
        in vec2 TexCoord;

        uniform vec3 objectColor;
        uniform vec3 lightPositions[4];
        uniform vec3 lightColors[4];
        uniform vec3 viewPos;
        uniform sampler2D texture1;
        uniform sampler2D wallTexture;
        uniform bool hasTexture;
        uniform bool useWallTexture;

        void main() {
            float ambientStrength = 0.3;
            vec3 ambient = ambientStrength * vec3(1.0, 1.0, 1.0);
            
            vec3 result = vec3(0.0, 0.0, 0.0);
            vec3 norm = normalize(Normal);
            
            for(int i = 0; i < 4; i++) {
                vec3 lightDir = normalize(lightPositions[i] - FragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * lightColors[i];
                
                float specularStrength = 0.5;
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
                vec3 specular = specularStrength * spec * lightColors[i];
                
                float distance = length(lightPositions[i] - FragPos);
                float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
                
                diffuse *= attenuation;
                specular *= attenuation;
                
                result += (diffuse + specular);
            }
            
            result += ambient;
            
            vec4 texColor;
            if(hasTexture) {
                texColor = texture(texture1, TexCoord);
            } else if(useWallTexture) {
                texColor = texture(wallTexture, TexCoord);
            } else {
                texColor = vec4(objectColor, 1.0);
            }
            
            FragColor = vec4(result * texColor.rgb, 1.0);
        }
    )";
};
