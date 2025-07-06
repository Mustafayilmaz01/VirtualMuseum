#ifndef LIGHTING_SYSTEM_HPP
#define LIGHTING_SYSTEM_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


class LightingSystem {
public:
    
    LightingSystem();
    
   
    ~LightingSystem();
    
   
    bool initialize();
    
    void apply(const glm::mat4& viewMatrix, 
              const glm::mat4& projectionMatrix, 
              GLuint shaderProgram);
    
    void update(float deltaTime);
    
    
    int addAmbientLight(const glm::vec3& position, 
                       const glm::vec3& color, 
                       float intensity);
    
    int addSpotLight(const glm::vec3& position, 
                    const glm::vec3& direction, 
                    const glm::vec3& color, 
                    float intensity, 
                    float cutOff, 
                    float outerCutOff);
   
    int addDirectionalLight(const glm::vec3& direction, 
                           const glm::vec3& color, 
                           float intensity);

private:
    
    struct AmbientLight {
        glm::vec3 position;
        glm::vec3 color;
        float intensity;
    };
    
    struct SpotLight {
        glm::vec3 position;
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
        float cutOff;
        float outerCutOff;
    };
    
    struct DirectionalLight {
        glm::vec3 direction;
        glm::vec3 color;
        float intensity;
    };
    
  
    std::vector<AmbientLight> ambientLights;
    std::vector<SpotLight> spotLights;
    std::vector<DirectionalLight> directionalLights;
    
   
    GLuint lightVAO, lightVBO;
    GLuint shaderProgram;
    
   
    void createShaders();
    
    
    void renderLightSources(const glm::mat4& viewMatrix, 
                           const glm::mat4& projectionMatrix);
};

#endif 