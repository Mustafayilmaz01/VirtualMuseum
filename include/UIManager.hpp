#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Statue.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool initialize(GLFWwindow* window);
    void shutdown();

    void newFrame();
    
    void render();

    void renderScanEffect(GLFWwindow* window, const Statue& statue, const glm::mat4& view, const glm::mat4& projection, 
                         float scanProgress, float scanDuration);
    
    void renderInfoPanel(GLFWwindow* window, const Statue& statue, const glm::mat4& view, const glm::mat4& projection);
    
    void renderRobotControlPanel(int screenWidth, int screenHeight, int activeStatueIndex, const std::vector<Statue*>& statues);

private:
    void setupFonts();
};
