#include "../include/Renderer.hpp"
#include <iostream>
#include "stb_image.h"

// render sınıfını kurar.
Renderer::Renderer() : roomVAO(0), roomVBO(0), wallTextureID(0), 
                       roomWidth(0), roomHeight(0), roomLength(0) {
}

// render sınıfını kapatırken temizlik 
Renderer::~Renderer() {
    cleanup();
}

// render sınıfını başlatır, shader ve oda ayarlarını yapılan yer
bool Renderer::initialize(float width, float height, float length) {
    roomWidth = width;
    roomHeight = height;
    roomLength = length;
    
    setupShaders();
    
    setupRoom();
    
    wallTextureID = loadTexture("textures/walls/duvarresmi.jpg");
    if (wallTextureID == 0) {
        std::cerr << "Duvar dokusu yüklenemedi!" << std::endl;
        return false;
    }
    
    return true;
}

// Kullanılan kaynakları temizlemek icin gerekli fonksiyon
void Renderer::cleanup() {
    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &roomVBO);
    glDeleteTextures(1, &wallTextureID);
    glDeleteProgram(shaderProgram);
}

// Bütün sahneyi  ekrana çizdigimiz yer
void Renderer::renderScene(GLFWwindow* window, const std::vector<Statue*>& statues, Robot* robot, 
                           UIManager* uiManager, const glm::vec3& cameraPos, const glm::mat4& viewMatrix, 
                           const glm::mat4& projectionMatrix, int activeStatueIndex, bool showInfoPanel, 
                           bool isScanning, float scanProgress, float scanDuration) {
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    
    glUseProgram(shaderProgram);
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPos));
    
    setupLights();
    
    
    renderStatues(statues, viewMatrix, projectionMatrix);
    
    renderRobot(robot, viewMatrix, projectionMatrix);
    
    if (isScanning && activeStatueIndex >= 0 && activeStatueIndex < statues.size()) {
        uiManager->renderScanEffect(window, *statues[activeStatueIndex], viewMatrix, projectionMatrix, scanProgress, scanDuration);
    }
    
    if (showInfoPanel && activeStatueIndex >= 0 && activeStatueIndex < statues.size()) {
        uiManager->renderInfoPanel(window, *statues[activeStatueIndex], viewMatrix, projectionMatrix);
    }
    
    uiManager->renderRobotControlPanel(width, height, activeStatueIndex, statues);
    
    uiManager->render();
}

// Işık ve gölge efektleri için shader'ları kurar
void Renderer::setupShaders() {
    shaderProgram = createShaderProgram(vertexShaderSource, fragmentShaderSource);
    
    modelLoc = glGetUniformLocation(shaderProgram, "model");
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");
    objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    lightPosLoc = glGetUniformLocation(shaderProgram, "lightPositions");
    lightColorsLoc = glGetUniformLocation(shaderProgram, "lightColors");
    viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
}

// Odanın iskeletini  oluşturur
void Renderer::setupRoom() {
    float vertices[] = {
                -roomWidth, -roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         roomWidth, -roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         roomWidth,  roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         roomWidth,  roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -roomWidth,  roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -roomWidth, -roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -roomWidth, -roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         roomWidth, -roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -roomWidth,  roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -roomWidth, -roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -roomWidth, -roomHeight, -roomLength, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -roomWidth, -roomHeight,  roomLength, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -roomWidth,  roomHeight,  roomLength, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -roomWidth,  roomHeight,  roomLength, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -roomWidth,  roomHeight, -roomLength, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -roomWidth, -roomHeight, -roomLength, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

             roomWidth, -roomHeight, -roomLength,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         roomWidth, -roomHeight,  roomLength,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         roomWidth,  roomHeight,  roomLength,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         roomWidth,  roomHeight,  roomLength,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         roomWidth,  roomHeight, -roomLength,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         roomWidth, -roomHeight, -roomLength,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,

            -roomWidth, -roomHeight, -roomLength,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         roomWidth, -roomHeight, -roomLength,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         roomWidth, -roomHeight,  roomLength,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         roomWidth, -roomHeight,  roomLength,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        -roomWidth, -roomHeight,  roomLength,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -roomWidth, -roomHeight, -roomLength,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

            -roomWidth,  roomHeight, -roomLength,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
         roomWidth,  roomHeight, -roomLength,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        -roomWidth,  roomHeight,  roomLength,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        -roomWidth,  roomHeight, -roomLength,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f
    };
    
    glGenVertexArrays(1, &roomVAO);
    glGenBuffers(1, &roomVBO);
    
    glBindVertexArray(roomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, roomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Sahnenin ışıklarını ayarlamak
void Renderer::setupLights() {
    glm::vec3 lightPositions[4] = {
        glm::vec3(-roomWidth * 0.8f, roomHeight * 0.8f, -roomLength * 0.8f),
        glm::vec3(roomWidth * 0.8f, roomHeight * 0.8f, -roomLength * 0.8f),
        glm::vec3(-roomWidth * 0.8f, roomHeight * 0.8f, roomLength * 0.8f),
        glm::vec3(roomWidth * 0.8f, roomHeight * 0.8f, roomLength * 0.8f)
    };
    
    glm::vec3 lightColors[4] = {
        glm::vec3(1.0f, 0.9f, 0.9f),
        glm::vec3(0.9f, 1.0f, 0.9f),
        glm::vec3(0.9f, 0.9f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };
    
    glUniform3fv(lightPosLoc, 4, glm::value_ptr(lightPositions[0]));
    glUniform3fv(lightColorsLoc, 4, glm::value_ptr(lightColors[0]));
}

// Odanın duvarlarını ve zeminini çizer
void Renderer::renderRoom(const glm::mat4& view, const glm::mat4& projection) {
   
    GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
    GLint useWallTextureLoc = glGetUniformLocation(shaderProgram, "useWallTexture");
    
   
    if (hasTextureLoc != -1) {
        glUniform1i(hasTextureLoc, GL_FALSE);
    }
    if (useWallTextureLoc != -1) {
        glUniform1i(useWallTextureLoc, GL_TRUE);
    }
    
   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wallTextureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "wallTexture"), 0);
    
    
    glm::mat4 model = glm::mat4(1.0f); 
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3f(objectColorLoc, 1.0f, 0.9f, 0.2f); 
    
    glBindVertexArray(roomVAO);
    
    glDrawArrays(GL_TRIANGLES, 0, 30);
    
   
    glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f); 
   
    glDrawArrays(GL_TRIANGLES, 30, 6);
    
    glBindVertexArray(0);
}

void Renderer::renderStatues(const std::vector<Statue*>& statues, const glm::mat4& view, const glm::mat4& projection) {
   
    GLint useWallTextureLoc = glGetUniformLocation(shaderProgram, "useWallTexture");
    GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
    
    if (useWallTextureLoc != -1) {
        glUniform1i(useWallTextureLoc, GL_FALSE);
    }
    if (hasTextureLoc != -1) {
        glUniform1i(hasTextureLoc, GL_TRUE);
    }
    
   
    glUniform3f(objectColorLoc, 1.0f, 0.7f, 0.3f); 
    

    for (auto statue : statues) {
        statue->render(view, projection, shaderProgram);
    }
}

void Renderer::renderRobot(Robot* robot, const glm::mat4& view, const glm::mat4& projection) {
    if (robot) {
        robot->render(view, projection, shaderProgram);
    }
}

unsigned int Renderer::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
   
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
   
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
   
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

unsigned int Renderer::loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
       
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    } else {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
        return 0;
    }
    
    return textureID;
}
