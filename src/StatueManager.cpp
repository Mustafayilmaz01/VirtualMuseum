#include "../include/StatueManager.hpp"

// Heykel yöneticisini kurar
StatueManager::StatueManager(float proximityThreshold, float scanDuration)
    : proximityThreshold(proximityThreshold), scanDuration(scanDuration),
      activeStatueIndex(-1), scanning(false), scanProgress(0.0f),
      showInfoPanel(false), infoDisplayTime(0.0f) {
}

// Kapanırken temizlik yapar
StatueManager::~StatueManager() {
    cleanup();
}

// Heykelleri oluşturur
bool StatueManager::initialize(float roomWidth, float roomHeight, float roomLength) {
    statues = Statue::createStatueCollection(roomWidth, roomHeight, roomLength);
    return !statues.empty();
}

// Robotun yakınındaki heykeli bulur
void StatueManager::updateStatueProximity(const Robot* robot) {
    if (scanning) {
        if (scanProgress >= 1.0f) {
            if (activeStatueIndex >= 0) {
                showInfoPanel = true;
                infoDisplayTime = 0.0f;
            }
            scanning = false;
        }
        return;
    }
    
    if (showInfoPanel) {
        return;
    }
    
    glm::vec3 robotPos = robot->getPosition();
    activeStatueIndex = Statue::checkProximity(statues, robotPos, proximityThreshold);
}

// Heykel taramasını başlatır.
void StatueManager::startScanning() {
    if (activeStatueIndex >= 0 && !scanning && !showInfoPanel) {
        scanning = true;
        scanProgress = 0.0f;
    }
}

// Taramayı iptal eder.
void StatueManager::cancelScanning() {
    if (scanning) {
        scanning = false;
        scanProgress = 0.0f;
    }
}

// Tarama süresini günceller
void StatueManager::updateScanning(float deltaTime) {
    if (scanning) {
        scanProgress += (deltaTime / scanDuration) * 1.5f;
        if (scanProgress >= 1.0f) {
            scanProgress = 1.0f;
        }
    }
    
    if (showInfoPanel) {
        infoDisplayTime += deltaTime;
        if (infoDisplayTime >= INFO_DISPLAY_DURATION) {
            showInfoPanel = false;
        }
    }
}

// Tüm heykelleri silme
void StatueManager::cleanup() {
    for (auto statue : statues) {
        delete statue;
    }
    statues.clear();
    activeStatueIndex = -1;
    scanning = false;
    showInfoPanel = false;
}

const std::vector<Statue*>& StatueManager::getStatues() const {
    return statues;
}

int StatueManager::getActiveStatueIndex() const {
    return activeStatueIndex;
}

bool StatueManager::isScanning() const {
    return scanning;
}

float StatueManager::getScanProgress() const {
    return scanProgress;
}

float StatueManager::getScanDuration() const {
    return scanDuration;
}

bool StatueManager::shouldShowInfoPanel() const {
    return showInfoPanel;
}

void StatueManager::setShowInfoPanel(bool show) {
    showInfoPanel = show;
}
