#include "../include/Room.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

// Odayı kurmak icin fonksiyon
Room::Room(float width, float height, float length)
    : roomWidth(width), roomHeight(height), roomLength(length),
      cameraPosition(0.0f, 0.0f, 1.0f), cameraFront(0.0f, 0.0f, -1.0f), cameraUp(0.0f, 1.0f, 0.0f),
      cameraSpeed(5.0f), yaw(-90.0f), pitch(0.0f),
      roomVAO(0), roomVBO(0), wallTextureID(0) {
    
    setupLights();
}

// Oda silinince hafızayı temizlemek için
Room::~Room() {
    glDeleteVertexArrays(1, &roomVAO);
    glDeleteBuffers(1, &roomVBO);
    glDeleteTextures(1, &wallTextureID);
}

// Odayı kullanıma hazırlar, duvar resmini yüklenir ambientcg sitesinden aldim
bool Room::initialize() {
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/walls/duvarresmi.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &wallTextureID);
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
    } else {
        std::cerr << "Duvar dokusu yüklenemedi!" << std::endl;
        return false;
    }
    
    setupRoom();
    
    return true;
}

// Odayı ekrana çizdiğimiz kisim
void Room::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint objectColorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    GLint lightColorsLoc = glGetUniformLocation(shaderProgram, "lightColors");
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    GLint hasTextureLoc = glGetUniformLocation(shaderProgram, "hasTexture");
    GLint useWallTextureLoc = glGetUniformLocation(shaderProgram, "useWallTexture");
    GLint wallTextureLoc = glGetUniformLocation(shaderProgram, "wallTexture");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glUniform3fv(lightPosLoc, 4, glm::value_ptr(lightPositions[0]));
    glUniform3fv(lightColorsLoc, 4, glm::value_ptr(lightColors[0]));
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(cameraPosition));
    
    glUniform1i(hasTextureLoc, GL_FALSE); 
    glUniform1i(useWallTextureLoc, GL_TRUE); 
    
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, wallTextureID);
    glUniform1i(wallTextureLoc, 1); 
    
    glBindVertexArray(roomVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); 
    glBindVertexArray(0);
}

// Odanın iskeletini  oluşturdum
void Room::setupRoom() {
    float vertices[] = {
        -roomWidth, -roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
         roomWidth, -roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
         roomWidth,  roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
         roomWidth,  roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
        -roomWidth,  roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
        -roomWidth, -roomHeight, -roomLength,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

        -roomWidth, -roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         roomWidth, -roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -roomWidth,  roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -roomWidth, -roomHeight,  roomLength,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -roomWidth,  roomHeight,  roomLength, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -roomWidth,  roomHeight, -roomLength, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -roomWidth, -roomHeight, -roomLength, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -roomWidth, -roomHeight, -roomLength, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -roomWidth, -roomHeight,  roomLength, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -roomWidth,  roomHeight,  roomLength, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         roomWidth,  roomHeight,  roomLength,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         roomWidth,  roomHeight, -roomLength,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         roomWidth, -roomHeight, -roomLength,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         roomWidth, -roomHeight, -roomLength,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         roomWidth, -roomHeight,  roomLength,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         roomWidth,  roomHeight,  roomLength,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -roomWidth, -roomHeight, -roomLength,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         roomWidth, -roomHeight, -roomLength,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         roomWidth, -roomHeight,  roomLength,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         roomWidth, -roomHeight,  roomLength,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -roomWidth, -roomHeight,  roomLength,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -roomWidth, -roomHeight, -roomLength,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,

        -roomWidth,  roomHeight, -roomLength,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         roomWidth,  roomHeight, -roomLength,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         roomWidth,  roomHeight,  roomLength,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -roomWidth,  roomHeight,  roomLength,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -roomWidth,  roomHeight, -roomLength,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f
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

// Sahnenin ışıklarını yerleştirme
void Room::setupLights() {
    lightPositions[0] = glm::vec3(-roomWidth * 0.8f, roomHeight * 0.8f, -roomLength * 0.8f);
    lightPositions[1] = glm::vec3(roomWidth * 0.8f, roomHeight * 0.8f, -roomLength * 0.8f);
    lightPositions[2] = glm::vec3(-roomWidth * 0.8f, roomHeight * 0.8f, roomLength * 0.8f);
    lightPositions[3] = glm::vec3(roomWidth * 0.8f, roomHeight * 0.8f, roomLength * 0.8f);
    
    lightColors[0] = glm::vec3(1.0f, 0.9f, 0.9f);
    lightColors[1] = glm::vec3(0.9f, 0.9f, 1.0f);
    lightColors[2] = glm::vec3(0.9f, 1.0f, 0.9f);
    lightColors[3] = glm::vec3(1.0f, 1.0f, 0.9f);
}

void Room::update(float deltaTime) {
}

void Room::controlCamera(float deltaTime) {
}

glm::vec3 Room::getCameraPosition() const {
    return cameraPosition;
}

glm::vec3 Room::getCameraDirection() const {
    return cameraFront;
}
