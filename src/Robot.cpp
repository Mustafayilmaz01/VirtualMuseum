#include "../include/Robot.hpp"
#include "../include/Texture.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <glm/common.hpp> 

const float MIN_X = -2.8f;
const float MAX_X = 2.8f;
const float MIN_Y = -1.75f; 
const float MAX_Y = 1.75f;
const float MIN_Z = -5.8f;
const float MAX_Z = 5.8f;

Robot::Robot(const glm::vec3& position)
    : position(position)
    , front(glm::vec3(0.0f, 0.0f, -1.0f))
    , rotation(0.0f)
    , movementSpeed(5.0f)
    , rotationSpeed(90.0f)
    , isMoving(false), isPopupVisible(false), popupVAO(0), popupVBO(0) {
}

Robot::~Robot() {
    glDeleteVertexArrays(1, &popupVAO);
    glDeleteBuffers(1, &popupVBO);
}
    

bool Robot::initialize() {
    if (!robotMesh.initializeMesh("textures/walls/robot.jpg")) {
        
        return false;
    }
    createPopupInterface();
    return true;
}

void Robot::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, robotMesh.getRobotTextureID());
    glUniform1i(glGetUniformLocation(shaderProgram, "robotTexture"), 0);
    
    glBindVertexArray(robotMesh.getRobotVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(robotMesh.getRobotIndicesCount()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    glm::mat4 armModel = model;
    
    armModel = glm::translate(armModel, glm::vec3(0.39f, 0.2f, 0.1f));
      
    
    armModel = glm::translate(armModel, glm::vec3(0.0f, 0.0f, 0.5f));
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(armModel));
    
    glBindVertexArray(robotMesh.getArmVAO());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(robotMesh.getArmIndicesCount()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    if (isPopupVisible) {
        glUseProgram(shaderProgram);
        
        glm::mat4 orthoProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(orthoProjection));
        
        glm::mat4 popupModel = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(popupModel));
        
        glBindVertexArray(popupVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        
    }
}

void Robot::update(float deltaTime, const std::vector<std::unique_ptr<Statue>>& statues) {
    if (isMoving) {
        glm::vec3 direction = glm::normalize(targetPosition - position);
        position += direction * movementSpeed * deltaTime;

        
        position.x = glm::clamp(position.x, MIN_X, MAX_X);
        position.y = glm::clamp(position.y, MIN_Y, MAX_Y);
        position.z = glm::clamp(position.z, MIN_Z, MAX_Z);
        
        float distance = glm::length(targetPosition - position);
        if (distance < 0.1f) {
            isMoving = false;
            hasReachedTarget = true;
        }
    }
    
    const Statue* nearestStatue = findNearestStatue(statues);
    
    static bool isScanning = false;
    static float scanTimer = 0.0f;
    static const float scanDuration = 4.0f;
    
    if (nearestStatue && isNearStatue(*nearestStatue)) {
        showInfoPopup(*nearestStatue);
        
        if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS && !isScanning) {
            isScanning = true;
            scanTimer = 0.0f;
        }
    } else {
        closeInfoPopup();
        
        if (isScanning) {
            isScanning = false;
        }
    }
    
    if (isScanning) {
        scanTimer += deltaTime;
        if (scanTimer >= scanDuration) {
            isScanning = false;
            
        }
    }
    
    if (hasReachedTarget && !isStopped) {
        currentTargetIndex = (currentTargetIndex + 1) % targetPoints.size();
        moveTo(targetPoints[currentTargetIndex]);
        hasReachedTarget = false;
    }
}

void Robot::moveTo(const glm::vec3& targetPosition) {
    this->targetPosition = targetPosition;
    isMoving = true;
}

glm::vec3 Robot::getPosition() const {
    return position;
}

void Robot::setPosition(const glm::vec3& newPosition) {
    position = newPosition;
}

glm::vec3 Robot::getDirection() const {
    return front;
}

void Robot::showInfoPopup(const Statue& statue) {
    isPopupVisible = true;
}

void Robot::closeInfoPopup() {
    isPopupVisible = false;
}

void Robot::handleUserInteraction(int key, int action) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        float randomX = static_cast<float>(rand() % 10 - 5);
        float randomZ = static_cast<float>(rand() % 10 - 5);
        moveTo(glm::vec3(randomX, 0.0f, randomZ));
    }
}

void Robot::moveForward(float speed) {
    glm::vec3 newPosition = position + front * speed * movementSpeed;
    
    newPosition.x = glm::clamp(newPosition.x, MIN_X, MAX_X);
    newPosition.y = glm::clamp(newPosition.y, MIN_Y, MAX_Y);
    newPosition.z = glm::clamp(newPosition.z, MIN_Z, MAX_Z);
    position = newPosition;
}

void Robot::moveBackward(float speed) {
    glm::vec3 newPosition = position - front * speed * movementSpeed;
    
    newPosition.x = glm::clamp(newPosition.x, MIN_X, MAX_X);
    newPosition.y = glm::clamp(newPosition.y, MIN_Y, MAX_Y);
    newPosition.z = glm::clamp(newPosition.z, MIN_Z, MAX_Z);
    position = newPosition;
}

void Robot::moveLeft(float speed) {
    glm::vec3 leftDirection = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 newPosition = position - leftDirection * speed * movementSpeed;
    
    
    newPosition.x = glm::clamp(newPosition.x, MIN_X, MAX_X);
    newPosition.y = glm::clamp(newPosition.y, MIN_Y, MAX_Y);
    newPosition.z = glm::clamp(newPosition.z, MIN_Z, MAX_Z);
    position = newPosition;
}

void Robot::moveRight(float speed) {
    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f))); 
    glm::vec3 newPosition = position + right * speed * movementSpeed;
    

    newPosition.x = glm::clamp(newPosition.x, MIN_X, MAX_X);
    newPosition.y = glm::clamp(newPosition.y, MIN_Y, MAX_Y);
    newPosition.z = glm::clamp(newPosition.z, MIN_Z, MAX_Z);
    position = newPosition;
}



void Robot::moveUp(float speed) {
    glm::vec3 upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 newPosition = position + upDirection * speed * movementSpeed;

    newPosition.x = glm::clamp(newPosition.x, MIN_X, MAX_X);
    newPosition.y = glm::clamp(newPosition.y, MIN_Y, MAX_Y); 
    newPosition.z = glm::clamp(newPosition.z, MIN_Z, MAX_Z);
    position = newPosition;
}

void Robot::moveDown(float speed) {
    glm::vec3 downDirection = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 newPosition = position + downDirection * speed * movementSpeed;

    newPosition.x = glm::clamp(newPosition.x, MIN_X, MAX_X);
    newPosition.y = glm::clamp(newPosition.y, MIN_Y, MAX_Y); 
    newPosition.z = glm::clamp(newPosition.z, MIN_Z, MAX_Z);
    position = newPosition;
}

void Robot::rotate(float angle) {
    rotation += angle * rotationSpeed;
    float radians = glm::radians(rotation);
    front.x = -sin(radians);  
    front.z = -cos(radians);   
    front = glm::normalize(front); 
}




void Robot::createPopupInterface() {
    float vertices[] = {
        300.0f, 350.0f,  
        500.0f, 350.0f,  
        500.0f, 550.0f,  
        300.0f, 550.0f,  
        300.0f, 350.0f,  
        500.0f, 550.0f   
    };
    
    glGenVertexArrays(1, &popupVAO);
    glGenBuffers(1, &popupVBO);
    
    glBindVertexArray(popupVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, popupVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


const Statue* Robot::findNearestStatue(const std::vector<std::unique_ptr<Statue>>& statues) {
    const Statue* nearestStatue = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    
    for (const auto& statue : statues) {
        if (isNearStatue(*statue)) {
            float distance = glm::length(position - statue->getPosition());
            
            if (distance < minDistance) {
                minDistance = distance;
                nearestStatue = statue.get();
            }
        }
    }
    
    return nearestStatue;
}

bool Robot::isNearStatue(const Statue& statue, float threshold) {
    float distance = glm::length(position - statue.getPosition());
    return distance < threshold;
}
