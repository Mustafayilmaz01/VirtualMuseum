#ifndef ROBOTMESH_HPP
#define ROBOTMESH_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

// Forward declaration if Texture class is in its own header
// class Texture;

class RobotMesh {
public:
    RobotMesh();
    ~RobotMesh();

    bool initializeMesh(const std::string& texturePath);

    GLuint getRobotVAO() const { return robotVAO; }
    GLuint getArmVAO() const { return armVAO; }
    GLuint getRobotTextureID() const { return robotTextureID; }
    size_t getRobotIndicesCount() const { return robotIndicesCount; }
    size_t getArmIndicesCount() const { return armIndicesCount; }

private:
    // OpenGL buffer objects
    GLuint robotVAO, robotVBO, robotEBO;
    GLuint armVAO, armVBO, armEBO;
    GLuint robotTextureID;

    // Vertex and index data
    std::vector<float> bodyVertices;
    std::vector<unsigned int> bodyIndices;
    std::vector<float> armVerticesData; 
    std::vector<unsigned int> armIndicesData;

    size_t robotIndicesCount; 
    size_t armIndicesCount;   

    void createBasicRobotModel();
    void createArmModel();
    void createBuffers();
};

#endif // ROBOTMESH_HPP
