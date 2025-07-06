#ifndef ROBOT_HPP
#define ROBOT_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

#include "Statue.hpp"
#include "RobotMesh.hpp"

class Robot {
public:
    Robot(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f));
    
    ~Robot();
    
    bool initialize();
    
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint shaderProgram);
    
    void update(float deltaTime, const std::vector<std::unique_ptr<Statue>>& statues);
    
    void moveTo(const glm::vec3& targetPosition);
    
    glm::vec3 getPosition() const;
    
    glm::vec3 getDirection() const;
    
    void showInfoPopup(const Statue& statue);
    
    void closeInfoPopup();
    
    void handleUserInteraction(int key, int action);

    void moveForward(float speed);
    void moveBackward(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed);
    void moveDown(float speed);
    void rotate(float angle);

    glm::vec3 getFront() const { return front; }
    float getRotation() const { return rotation; }
    void setMovementSpeed(float speed) { movementSpeed = speed; }
    void setPosition(const glm::vec3& newPosition);

private:
    glm::vec3 position;
    glm::vec3 front;
    float rotation;
    float movementSpeed;
    float rotationSpeed;
    glm::vec3 targetPosition;
    bool isMoving;
    bool hasReachedTarget;
    bool isStopped;
    int currentTargetIndex;
    std::vector<glm::vec3> targetPoints;
    
    bool isPopupVisible;
    
    GLuint popupVAO, popupVBO; 
    RobotMesh robotMesh;       
    
    bool loadRobotModel();
    
    void createPopupInterface();
    

    
    const Statue* findNearestStatue(const std::vector<std::unique_ptr<Statue>>& statues);
    
    bool isNearStatue(const Statue& statue, float threshold = 2.0f);
};

#endif
