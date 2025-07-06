#include "../include/InputManager.hpp"
#include <iostream>

static InputManager* g_inputManager = nullptr;


void key_callback_wrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (g_inputManager) {
        g_inputManager->processKeyPress(key, scancode, action, mods);
    }
}

void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset) {
    if (g_inputManager) {
        g_inputManager->processMouseScroll(xoffset, yoffset);
    }
}

InputManager::InputManager(GLFWwindow* window, Robot* robot, Camera* camera, StatueManager* statueManager)
    : window(window), robot(robot), camera(camera), statueManager(statueManager) {
  
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    screenWidth = static_cast<float>(width);
    screenHeight = static_cast<float>(height);
    
    robotSpeed = 0.6f;
    rotationSpeed = 3.0f; 
    
    
    g_inputManager = this;
}

InputManager::~InputManager() {
    g_inputManager = nullptr;
}

void InputManager::setupCallbacks() {
   
    glfwSetKeyCallback(window, key_callback_wrapper);
    glfwSetScrollCallback(window, scroll_callback_wrapper);
}

void InputManager::processInput(float deltaTime) {
    
    float speed = deltaTime * robotSpeed;
    
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        robot->moveForward(speed);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        robot->moveBackward(speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        robot->moveLeft(speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        robot->moveRight(speed);
    
   
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        robot->moveUp(speed);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        robot->moveDown(speed);
    
   
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        robot->rotate(-deltaTime * rotationSpeed);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        robot->rotate(deltaTime * rotationSpeed);
    
    
    if (camera && robot) {
        camera->updateCameraWithRobot(robot->getPosition(), robot->getRotation());
    }
}

void InputManager::processKeyPress(int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        
        if (statueManager) {
            if (statueManager->shouldShowInfoPanel()) {
                statueManager->setShowInfoPanel(false);
                return;
            }
            if (statueManager->isScanning()) {
                statueManager->cancelScanning();
                return;
            }
        }
        
        glfwSetWindowShouldClose(window, true);
    }
    
    
    if (action == GLFW_PRESS) {
        
        if (key == GLFW_KEY_E && statueManager) {
            statueManager->startScanning();
        }
        
        
        if (key == GLFW_KEY_F && statueManager && statueManager->shouldShowInfoPanel()) {
            statueManager->setShowInfoPanel(false);
        }
    }
}

void InputManager::processMouseScroll(double xoffset, double yoffset) {
    if (camera) {
        camera->handleMouseScroll(window, xoffset, yoffset);
    }
}
