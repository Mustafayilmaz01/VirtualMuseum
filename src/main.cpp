// OpenGL için gerekli kütüphaneler

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/Shader.hpp"
#include <iostream>
#include <vector>
#include <string>
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Projemizin header dosyaları
#include "../include/Statue.hpp"
#include "../include/Robot.hpp"
#include "../include/Room.hpp"
#include "../include/ExhibitionArea.hpp"
#include "../include/Application.hpp"
#include "../include/UIManager.hpp"
#include "../include/Renderer.hpp"
#include "../include/Camera.hpp"
#include "../include/StatueManager.hpp"
#include "../include/InputManager.hpp"

// Pencere boyutları - standart 800x600 kullanıyoruz
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Global değişkenlerimiz
Camera* camera = nullptr;
Renderer* renderer = nullptr;
StatueManager* statueManager = nullptr;
InputManager* inputManager = nullptr;

// Robotun heykele ne kadar yaklaşması gerektiği
const float STATUE_PROXIMITY_THRESHOLD = 1.5f;
const float SCAN_DURATION = 1.0f;

// Fps bağımsız hareketi için
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Robot* robot = nullptr;
std::vector<Statue*> statues;

// Pencere boyutu değişince çağrılacak fonksiyon
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

UIManager* uiManager = nullptr;
Shader* shader = nullptr;

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {

    glfwInit();
    // OpenGL 3.3 kullanıyoruz
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pencereyi oluştur
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Virtual Museum", NULL, NULL);
    if (window == NULL) {
        std::cout << "GLFW penceresi oluşturulamadı!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Mouse'u gizliyoruz ki kamera kontrolü düzgün çalışsın
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD yüklenemedi!\n";
        return -1;
    }


    uiManager = new UIManager();
    if (!uiManager->initialize(window)) {
        std::cerr << "UI Yöneticisi başlatılamadı!" << std::endl;
        return -1;
    }

    // Depth testing  3D görüntü için
    glEnable(GL_DEPTH_TEST);

    // Shader programını yükle
    shader = new Shader();
    if (!shader->initialize()) {
        std::cerr << "Shader programı başlatılamadı!" << std::endl;
        return -1;
    }

    // Kamerayı oluştur - biraz yüksekten bakıyor başlangıçta
    camera = new Camera(glm::vec3(0.0f, 2.0f, 5.0f));
    if (!camera) {
        std::cerr << "Kamera oluşturulamadı!" << std::endl;
        return -1;
    }

    unsigned int shaderProgram = shader->getID();

    // Oda boyutları -
    float roomWidth = 3.0f;
    float roomHeight = 2.0f;
    float roomLength = 6.0f;

    // Heykelleri yönetecek sınıfı başlat
    statueManager = new StatueManager(STATUE_PROXIMITY_THRESHOLD, SCAN_DURATION);
    if (!statueManager->initialize(roomWidth, roomHeight, roomLength)) {
        std::cerr << "Heykel koleksiyonu oluşturulamadı!" << std::endl;
        return -1;
    }

    statues = statueManager->getStatues();

    // Robotu oluştup odanin girisine alma
    robot = new Robot(glm::vec3(0.0f, -1.8f, -2.0f));
    if (!robot->initialize()) {
        std::cerr << "Robot başlatılamadı!" << std::endl;
        return -1;
    }

    // Input manager'ı başlat - klavye ve mouse kontrolü için
    inputManager = new InputManager(window, robot, camera, statueManager);
    inputManager->setupCallbacks();

    // Renderer'ı başlat
    renderer = new Renderer();
    if (!renderer->initialize(roomWidth, roomHeight, roomLength)) {
        std::cerr << "Renderer başlatılamadı!" << std::endl;
        return -1;
    }

    // Odayı oluştur
    Room* room = new Room(roomWidth, roomHeight, roomLength);
    if (!room->initialize()) {
        std::cerr << "Oda başlatılamadı!" << std::endl;
        return -1;
    }

    // Ana döngü - ESC'ye basana kadar çalışır
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input'ları kontrol et
        if (inputManager) {
            inputManager->processInput(deltaTime);
        }

        // Robot heykele yakın mı kontrol
        if (statueManager && robot) {
            statueManager->updateStatueProximity(robot);
            statueManager->updateScanning(deltaTime);
        }

        // Ekranı temizlemek için
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  // Koyu gri arka plan
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // UI'ı hazırla
        uiManager->newFrame();

        // Kamera hesaplamaları
        glm::mat4 projection = camera->getProjectionMatrix((float)SCR_WIDTH/SCR_HEIGHT);
        glm::mat4 view = camera->getViewMatrix();

        //  oda, heykeller, robot çizmek icin
        room->render(view, projection, shader->getID());
        renderer->renderScene(window, statues, robot, uiManager,
                              camera->getPosition(), view, projection,
                              statueManager->getActiveStatueIndex(),
                              statueManager->shouldShowInfoPanel(),
                              statueManager->isScanning(),
                              statueManager->getScanProgress(),
                              statueManager->getScanDuration());

        // ekran senkronizasyonu için
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Hafızayı temizle kodu dolmasın diye
    delete robot;

    if (statueManager) {
        delete statueManager;
        statueManager = nullptr;
    }
    statues.clear();

    if (room) {
        delete room;
        room = nullptr;
    }

    if (uiManager) {
        uiManager->shutdown();
        delete uiManager;
        uiManager = nullptr;
    }

    if (renderer) {
        delete renderer;
        renderer = nullptr;
    }

    if (shader) {
        delete shader;
        shader = nullptr;
    }

    if (camera) {
        delete camera;
        camera = nullptr;
    }

    if (inputManager) {
        delete inputManager;
        inputManager = nullptr;
    }

    glfwTerminate();
    return 0;
}
