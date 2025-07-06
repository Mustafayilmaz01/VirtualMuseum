#include "../include/RobotMesh.hpp"
#include "../include/Texture.hpp" 
#include <iostream>

RobotMesh::RobotMesh()
    : robotVAO(0), robotVBO(0), robotEBO(0),
      armVAO(0), armVBO(0), armEBO(0),
      robotTextureID(0),
      robotIndicesCount(0), armIndicesCount(0) {
}

RobotMesh::~RobotMesh() {
    glDeleteVertexArrays(1, &robotVAO);
    glDeleteBuffers(1, &robotVBO);
    glDeleteBuffers(1, &robotEBO);

    glDeleteVertexArrays(1, &armVAO);
    glDeleteBuffers(1, &armVBO);
    glDeleteBuffers(1, &armEBO);

    if (robotTextureID != 0) {
        glDeleteTextures(1, &robotTextureID);
    }
}

bool RobotMesh::initializeMesh(const std::string& texturePath) {
    robotTextureID = Texture::loadTexture(texturePath.c_str());
    if (robotTextureID == 0) {
        std::cerr << "HATA::ROBOTMESH::DOKU_YUKLENEMEDI: " << texturePath << std::endl;
        return false;
    }

    createBasicRobotModel();
    createArmModel();       
    createBuffers();        

    return true;
}

void RobotMesh::createBasicRobotModel() {
    this->bodyVertices.clear();
    this->bodyIndices.clear();
    
    float bodyWidth = 0.3f;
    float bodyHeight = 0.4f;
    float bodyDepth = 0.2f;
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(-bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-bodyWidth); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(bodyDepth);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    int bodyStartIndex = 0;
    this->bodyIndices.push_back(bodyStartIndex + 0); this->bodyIndices.push_back(bodyStartIndex + 1); this->bodyIndices.push_back(bodyStartIndex + 2);
    this->bodyIndices.push_back(bodyStartIndex + 2); this->bodyIndices.push_back(bodyStartIndex + 3); this->bodyIndices.push_back(bodyStartIndex + 0);
    
    this->bodyIndices.push_back(bodyStartIndex + 4); this->bodyIndices.push_back(bodyStartIndex + 5); this->bodyIndices.push_back(bodyStartIndex + 6);
    this->bodyIndices.push_back(bodyStartIndex + 6); this->bodyIndices.push_back(bodyStartIndex + 7); this->bodyIndices.push_back(bodyStartIndex + 4);
    
    this->bodyIndices.push_back(bodyStartIndex + 8); this->bodyIndices.push_back(bodyStartIndex + 9); this->bodyIndices.push_back(bodyStartIndex + 10);
    this->bodyIndices.push_back(bodyStartIndex + 10); this->bodyIndices.push_back(bodyStartIndex + 11); this->bodyIndices.push_back(bodyStartIndex + 8);
    
    this->bodyIndices.push_back(bodyStartIndex + 12); this->bodyIndices.push_back(bodyStartIndex + 13); this->bodyIndices.push_back(bodyStartIndex + 14);
    this->bodyIndices.push_back(bodyStartIndex + 14); this->bodyIndices.push_back(bodyStartIndex + 15); this->bodyIndices.push_back(bodyStartIndex + 12);
    
    this->bodyIndices.push_back(bodyStartIndex + 16); this->bodyIndices.push_back(bodyStartIndex + 17); this->bodyIndices.push_back(bodyStartIndex + 18);
    this->bodyIndices.push_back(bodyStartIndex + 18); this->bodyIndices.push_back(bodyStartIndex + 19); this->bodyIndices.push_back(bodyStartIndex + 16);
    
    this->bodyIndices.push_back(bodyStartIndex + 20); this->bodyIndices.push_back(bodyStartIndex + 21); this->bodyIndices.push_back(bodyStartIndex + 22);
    this->bodyIndices.push_back(bodyStartIndex + 22); this->bodyIndices.push_back(bodyStartIndex + 23); this->bodyIndices.push_back(bodyStartIndex + 20);
    
    float headSize = 0.2f;
    float headY = bodyHeight + headSize; 
    
    int headStartIndex = static_cast<int>(this->bodyVertices.size() / 8); 
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-headSize); 
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f); 
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    
    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);
    
    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(bodyHeight); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(-1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);

    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(-headSize); 
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f); 
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(0.0f);

    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(-headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);

    this->bodyVertices.push_back(headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(1.0f);

    this->bodyVertices.push_back(-headSize); this->bodyVertices.push_back(headY); this->bodyVertices.push_back(headSize);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f); this->bodyVertices.push_back(0.0f);
    this->bodyVertices.push_back(0.0f); this->bodyVertices.push_back(1.0f);

    for (int i = 0; i < 6; i++) {
        int faceStartIndex = headStartIndex + i * 4;
        this->bodyIndices.push_back(faceStartIndex + 0); this->bodyIndices.push_back(faceStartIndex + 1); this->bodyIndices.push_back(faceStartIndex + 2);
        this->bodyIndices.push_back(faceStartIndex + 2); this->bodyIndices.push_back(faceStartIndex + 3); this->bodyIndices.push_back(faceStartIndex + 0);
    }
}

void RobotMesh::createArmModel() {
    float armWidth = 0.1f;
    float armHeight = 0.2f;
    float armDepth = 0.5f;
    
    this->armVerticesData.clear();
    this->armIndicesData.clear();
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(armHeight); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(0.0f);
    
    this->armVerticesData.push_back(armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(1.0f); this->armVerticesData.push_back(1.0f);
    
    this->armVerticesData.push_back(-armWidth); this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(armDepth);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(-1.0f); this->armVerticesData.push_back(0.0f);
    this->armVerticesData.push_back(0.0f); this->armVerticesData.push_back(1.0f);
    
    this->armIndicesData.push_back(0); this->armIndicesData.push_back(1); this->armIndicesData.push_back(2);
    this->armIndicesData.push_back(2); this->armIndicesData.push_back(3); this->armIndicesData.push_back(0);
    
    this->armIndicesData.push_back(4); this->armIndicesData.push_back(5); this->armIndicesData.push_back(6);
    this->armIndicesData.push_back(6); this->armIndicesData.push_back(7); this->armIndicesData.push_back(4);
    
    this->armIndicesData.push_back(8); this->armIndicesData.push_back(9); this->armIndicesData.push_back(10);
    this->armIndicesData.push_back(10); this->armIndicesData.push_back(11); this->armIndicesData.push_back(8);
    
    this->armIndicesData.push_back(12); this->armIndicesData.push_back(13); this->armIndicesData.push_back(14);
    this->armIndicesData.push_back(14); this->armIndicesData.push_back(15); this->armIndicesData.push_back(12);
    
    this->armIndicesData.push_back(16); this->armIndicesData.push_back(17); this->armIndicesData.push_back(18);
    this->armIndicesData.push_back(18); this->armIndicesData.push_back(19); this->armIndicesData.push_back(16);
    
    this->armIndicesData.push_back(20); this->armIndicesData.push_back(21); this->armIndicesData.push_back(22);
    this->armIndicesData.push_back(22); this->armIndicesData.push_back(23); this->armIndicesData.push_back(20);
}

void RobotMesh::createBuffers() {
   
    glGenVertexArrays(1, &this->robotVAO);
    glGenBuffers(1, &this->robotVBO);
    glGenBuffers(1, &this->robotEBO);

    glBindVertexArray(this->robotVAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->robotVBO);
    glBufferData(GL_ARRAY_BUFFER, this->bodyVertices.size() * sizeof(float), &this->bodyVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->robotEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->bodyIndices.size() * sizeof(unsigned int), &this->bodyIndices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->robotIndicesCount = this->bodyIndices.size();

    glGenVertexArrays(1, &this->armVAO);
    glGenBuffers(1, &this->armVBO);
    glGenBuffers(1, &this->armEBO);
    
    glBindVertexArray(this->armVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->armVBO);
    glBufferData(GL_ARRAY_BUFFER, this->armVerticesData.size() * sizeof(float), &this->armVerticesData[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->armEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->armIndicesData.size() * sizeof(unsigned int), &this->armIndicesData[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    this->armIndicesCount = this->armIndicesData.size();
}
