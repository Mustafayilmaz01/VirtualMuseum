#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

#include "Room.hpp"
#include "ExhibitionArea.hpp"
#include "LightingSystem.hpp"
#include "Robot.hpp"
#include "Shader.hpp"


class Application {
public:
    
    Application(int width, int height, const std::string& title);
    
    
    ~Application();
    
    
    bool initialize();
    
    
    void run();
    
  
    void shutdown();

private:
    GLFWwindow* window;
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
    
    std::unique_ptr<Room> room;
    std::unique_ptr<ExhibitionArea> exhibitionArea;
    std::unique_ptr<LightingSystem> lightingSystem;
    std::unique_ptr<Robot> robot;
    std::unique_ptr<Shader> shader;
    
   
    void processInput();
    
    
    void render();
};

#endif 
