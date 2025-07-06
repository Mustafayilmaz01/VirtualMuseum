#ifndef STATUE_MANAGER_HPP
#define STATUE_MANAGER_HPP

#include <vector>
#include <glm/glm.hpp>
#include "Statue.hpp"
#include "Robot.hpp"

class StatueManager {
public:
    StatueManager(float proximityThreshold = 1.5f, float scanDuration = 4.0f);
    
    ~StatueManager();
    
    bool initialize(float roomWidth, float roomHeight, float roomLength);
    
    void updateStatueProximity(const Robot* robot);
    
    void startScanning();
    
    void cancelScanning();
    
    void updateScanning(float deltaTime);
    
    void cleanup();
    
    const std::vector<Statue*>& getStatues() const;
    
    int getActiveStatueIndex() const;
    
    bool isScanning() const;
    
    float getScanProgress() const;
    
    float getScanDuration() const;
    
    bool shouldShowInfoPanel() const;
    
    void setShowInfoPanel(bool show);

private:
    std::vector<Statue*> statues;
    int activeStatueIndex;
    float proximityThreshold;
    bool scanning;
    float scanProgress;
    float scanDuration;
    bool showInfoPanel;
    float infoDisplayTime;
    const float INFO_DISPLAY_DURATION = 5.0f;
};

#endif
