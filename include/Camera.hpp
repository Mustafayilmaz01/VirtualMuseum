#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
           const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f),
           const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));
    
    ~Camera();
    
    void setPosition(const glm::vec3& position);
    
    void setFront(const glm::vec3& front);
    
    void setUp(const glm::vec3& up);
    
    glm::vec3 getPosition() const;
    
    glm::vec3 getFront() const;
    
    glm::vec3 getUp() const;
    
    glm::mat4 getViewMatrix() const;
    

    glm::mat4 getProjectionMatrix(float aspectRatio, float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 100.0f) const;
    
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    void processMouseScroll(float yoffset);
    
    void moveForward(float distance);
    
    void moveBackward(float distance);
    
    void moveLeft(float distance);
    
    void moveRight(float distance);
    
    void moveUp(float distance);
    
    void moveDown(float distance);
    
    void updateCameraVectors();
    
    void setFOV(float fov);
    
    float getFOV() const;
    
    void setCameraDistance(float distance);
    
    float getCameraDistance() const;
    
    void setCameraHeight(float height);
    
    float getCameraHeight() const;
    
    void updateCameraWithRobot(const glm::vec3& robotPosition, float robotRotation);
    
    void handleMouseMovement(GLFWwindow* window, double xpos, double ypos, float screenWidth, float screenHeight);
    
    void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {};

private:
    glm::vec3 position; 
    glm::vec3 front;    
    glm::vec3 up;       
    glm::vec3 right;    
    glm::vec3 worldUp;  
    
    float yaw;          
    float pitch;        
    float fov;          
    
    float cameraDistance; 
    float cameraHeight;   
    
    float movementSpeed;
    float mouseSensitivity;
    
    float lastX;        
    float lastY;        
    bool firstMouse;    
};

#endif
