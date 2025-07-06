#ifndef STATUE_HPP
#define STATUE_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Statue {
public:
    Statue(const std::string& modelPath, 
          const glm::vec3& position, 
          const glm::vec3& scale, 
          const glm::vec3& rotation);
          
    static std::vector<Statue*> createStatueCollection(float roomWidth, float roomHeight, float roomLength);
    
    static int checkProximity(const std::vector<Statue*>& statues, const glm::vec3& robotPos, float proximityDistance);
    
    ~Statue();
    
    bool initialize();
    
    void render(const glm::mat4& viewMatrix, 
               const glm::mat4& projectionMatrix, 
               GLuint shaderProgram);
    
    // update fonksiyonu kaldırıldı
    
    glm::vec3 getPosition() const;
    
    glm::vec3 getRotation() const;
    
    
    void setInformation(const std::string& title, 
                       const std::string& description, 
                       const std::string& artist, 
                       int year);
    
    const std::string& getTitle() const;
    
    const std::string& getDescription() const;
    
    const std::string& getArtist() const {
        return artist;
    }
    
    int getYear() const {
        return year;
    }

private:
    std::string modelPath;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    
    std::string title;
    std::string description;
    std::string artist;
    int year;
    
    static const std::string MODEL_PATHS[5];
    static const std::string TITLES[5];
    static const std::string DESCRIPTIONS[5];
    static const std::string ARTISTS[5];
    static const int YEARS[5];
    
    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    
    struct Texture {
        GLuint id;
        std::string type;
        std::string path;
    };
    
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
    
    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        GLuint VAO, VBO, EBO;
    };
    
    std::vector<Mesh> meshes;
    std::vector<Texture> textures_loaded;
    std::string directory;
    
    bool loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void setupMesh(Mesh& mesh);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    GLuint textureFromFile(const std::string& path, const std::string& directory);
    


};

#endif
