#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Robot.hpp"
#include "Camera.hpp"
#include "StatueManager.hpp"

class InputManager {
public:
    InputManager(GLFWwindow* window, Robot* robot, Camera* camera, StatueManager* statueManager);
    
    ~InputManager();
    
    void setupCallbacks();
    
    void processInput(float deltaTime);
    
    void processKeyPress(int key, int scancode, int action, int mods);
    
    void processMouseMovement(double xpos, double ypos);
    
    void processMouseScroll(double xoffset, double yoffset);
    
private:
    GLFWwindow* window;
    Robot* robot;
    Camera* camera;
    StatueManager* statueManager;
    
    float screenWidth;
    float screenHeight;
    
    float robotSpeed;
    float rotationSpeed;
};

void key_callback_wrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback_wrapper(GLFWwindow* window, double xpos, double ypos);
void scroll_callback_wrapper(GLFWwindow* window, double xoffset, double yoffset);

#endif
