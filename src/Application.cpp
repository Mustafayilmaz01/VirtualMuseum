#include "../include/Application.hpp"
#include <iostream>
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Application::Application(int width, int height, const std::string& title)
    : windowWidth(width), windowHeight(height), windowTitle(title), window(nullptr) {
}

Application::~Application() {
    shutdown();
}

bool Application::initialize() {
   
    if (!glfwInit()) {
        std::cerr << "GLFW başlatılamadı!" << std::endl;
        return false;
    }
    
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "GLFW penceresi oluşturulamadı!" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD başlatılamadı!" << std::endl;
        return false;
    }
    
 
    glViewport(0, 0, windowWidth, windowHeight);
    
    
    glEnable(GL_DEPTH_TEST);
    
   
    room = std::make_unique<Room>();
    if (!room->initialize()) {
        std::cerr << "Oda başlatılamadı!" << std::endl;
        return false;
    }
    
    exhibitionArea = std::make_unique<ExhibitionArea>();
    if (!exhibitionArea->initialize()) {
        std::cerr << "Sergi alanı başlatılamadı!" << std::endl;
        return false;
    }
    
    lightingSystem = std::make_unique<LightingSystem>();
    if (!lightingSystem->initialize()) {
        std::cerr << "Aydınlatma sistemi başlatılamadı!" << std::endl;
        return false;
    }
    
    robot = std::make_unique<Robot>();
    if (!robot->initialize()) {
        std::cerr << "Robot başlatılamadı!" << std::endl;
        return false;
    }
    
  
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    
    shader = std::make_unique<Shader>();
    if (!shader->initialize()) {
        std::cerr << "Shader programı başlatılamadı!" << std::endl;
        return false;
    }
    
    return true;
}

void Application::run() {
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (!glfwWindowShouldClose(window)) {
        
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        
        processInput();
        
       
        room->update(deltaTime);
        room->controlCamera(deltaTime);
        exhibitionArea->update(deltaTime);
        lightingSystem->update(deltaTime);
        robot->update(deltaTime, exhibitionArea->getStatues());
        
       
        render();
        
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::shutdown() {
    
    robot.reset();
    lightingSystem.reset();
    exhibitionArea.reset();
    room.reset();
    
    
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void Application::processInput() {
   
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        robot->handleUserInteraction(GLFW_KEY_R, GLFW_PRESS);
    }
    
   
}

void Application::render() {
   
    glClearColor(0.2f, 0.3f, 0.4f, 1.0f); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
   
    glm::mat4 viewMatrix = glm::lookAt(
        room->getCameraPosition(),
        room->getCameraPosition() + room->getCameraDirection(),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    glm::mat4 projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        static_cast<float>(windowWidth) / static_cast<float>(windowHeight),
        0.1f,
        100.0f
    );
    
   
    room->render(viewMatrix, projectionMatrix, shader->getID());
    exhibitionArea->render(viewMatrix, projectionMatrix);
    robot->render(viewMatrix, projectionMatrix, shader->getID());
}
