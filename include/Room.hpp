#ifndef ROOM_HPP
#define ROOM_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Statue.hpp"

class Room {
public:
    Room(float width = 3.0f, float height = 2.0f, float length = 6.0f);
    
    ~Room();
    
    bool initialize();
    
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint shaderProgram);
    
    void update(float deltaTime);
    
    void controlCamera(float deltaTime);
    
    glm::vec3 getCameraPosition() const;
    
    glm::vec3 getCameraDirection() const;

    glm::vec3 getRoomDimensions() const { return glm::vec3(roomWidth, roomHeight, roomLength); }
    
    const glm::vec3* getLightPositions() const { return lightPositions; }
    
    const glm::vec3* getLightColors() const { return lightColors; }

private:
    float roomWidth, roomHeight, roomLength;
    
    glm::vec3 cameraPosition;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    float cameraSpeed;
    float yaw, pitch;
    
    GLuint roomVAO, roomVBO;
    GLuint wallTextureID;
    
    glm::vec3 lightPositions[4];
    glm::vec3 lightColors[4];
    
    void setupRoom();
    void setupLights();
};

#endif
