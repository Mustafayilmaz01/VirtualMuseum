#include "../include/UIManager.hpp"
#include <iostream>

UIManager::UIManager() {
}

UIManager::~UIManager() {
}

bool UIManager::initialize(GLFWwindow* window) {
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
    
  
    setupFonts();
    
  
    ImGui::StyleColorsDark(); 
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    return true;
}

void UIManager::setupFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    
    static const ImWchar ranges[] = {
        0x0020, 0x00FF, 
        0x0100, 0x017F,
        0x0180, 0x024F, 
        0x0300, 0x036F, 
        0x00A0, 0x00FF, 
        0,
    };
    
    ImFontConfig config;
    config.MergeMode = true;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 14.0f, &config, ranges);
    io.Fonts->Build();
}

void UIManager::shutdown() {
  
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::newFrame() {
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UIManager::render() {
  
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::renderScanEffect(GLFWwindow* window, const Statue& statue, const glm::mat4& view, const glm::mat4& projection, 
                               float scanProgress, float scanDuration) {
    
    glm::vec3 statuePos = statue.getPosition();
    
    
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.9f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(300, 80), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.7f); 
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | 
                                   ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    
    ImGui::Begin("ScanningEffect", nullptr, window_flags);
    
  
    float progress = scanProgress / scanDuration;
    
    
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Heykel Taraması").x) * 0.5f);
    ImGui::Text("Heykel Taraması");
    
 
    ImGui::ProgressBar(progress, ImVec2(-1, 0), "");
    
  
    char progressText[32];
    sprintf(progressText, "%d%%", (int)(progress * 100));
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(progressText).x) * 0.5f);
    ImGui::Text("%s", progressText);
  
    if (scanProgress >= scanDuration) {
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Tarama Tamamlandı!").x) * 0.5f);
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Tarama Tamamlandı!");
    }
    
    ImGui::End();
}

void UIManager::renderInfoPanel(GLFWwindow* window, const Statue& statue, const glm::mat4& view, const glm::mat4& projection) {
    
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.9f); 
    
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse | 
                                   ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    
    std::string windowTitle = "Heykel Bilgileri: " + statue.getTitle();
    ImGui::Begin(windowTitle.c_str(), nullptr, window_flags);
    
 
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(statue.getTitle().c_str()).x) * 0.5f);
    ImGui::Text("%s", statue.getTitle().c_str());
    ImGui::Separator();
    
    ImGui::TextWrapped("%s", statue.getDescription().c_str());
    ImGui::Separator();
    
    
    ImGui::Text("Sanatçı: %s", statue.getArtist().c_str());
    
    
    int year = statue.getYear();
    if (year < 0) {
        ImGui::Text("Tarih: MÖ %d", -year);
    } else {
        ImGui::Text("Tarih: MS %d", year);
    }
    
    ImGui::Separator();
    
    
    if (ImGui::Button("Kapat (F)", ImVec2(100, 30))) {
        
    }
    
    ImGui::End();
}

void UIManager::renderRobotControlPanel(int screenWidth, int screenHeight, int activeStatueIndex, const std::vector<Statue*>& statues) {
   
    ImGui::SetNextWindowPos(ImVec2(screenWidth - 250, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(240, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Robot Kontrol Paneli");
    

    ImGui::Text("Kontrol Tuslari:");
    ImGui::BulletText("W: Ileri");
    ImGui::BulletText("S: Geri");
    ImGui::BulletText("A: Sola");
    ImGui::BulletText("D: Saga");
    ImGui::BulletText("C: Yukari");
    ImGui::BulletText("Z: Asagi");
    ImGui::BulletText("E: Tarama baslat/durdur");
    ImGui::BulletText("F: Bilgi paneli goster/gizle");
    ImGui::BulletText("ESC: Cikis/Kapat");
    ImGui::Separator();
    
   
    ImGui::Separator();
    if (activeStatueIndex >= 0 && activeStatueIndex < statues.size()) {
        ImGui::Text("Aktif Heykel: %s", statues[activeStatueIndex]->getTitle().c_str());
    } else {
        ImGui::Text("Aktif Heykel: Yok");
    }
    
    ImGui::End();
}
