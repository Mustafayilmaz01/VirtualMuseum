#ifndef EXHIBITION_AREA_HPP
#define EXHIBITION_AREA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

#include "Statue.hpp"


class ExhibitionArea {
public:
   
    ExhibitionArea();
    
    
    ~ExhibitionArea();
    
    
    bool initialize();
    
    
    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    
    
    void update(float deltaTime);
    
    
    Statue& loadModel(const std::string& filePath, 
                     const glm::vec3& position, 
                     const glm::vec3& scale, 
                     const glm::vec3& rotation);
    
    const std::vector<std::unique_ptr<Statue>>& getStatues() const;

private:
    
    std::vector<std::unique_ptr<Statue>> statues;
  
    GLuint shaderProgram;
    
    
    void createShaders();
   
    void placeObjects();
    
   
    void applyAnimations();
};

#endif 