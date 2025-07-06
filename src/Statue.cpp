#include "../include/Statue.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../external/stb/stb_image.h"

// Modellerin dosya yolları
const std::string Statue::MODEL_PATHS[5] = {
    "models/_0527182553_texture.obj",
    "models/_0527184454_texture.obj",
    "models/_0527185148_texture.obj",
    "models/_0527185844_texture.obj",
    "models/_0527191229_texture.obj"
};

// Heykellerin isimleri
const std::string Statue::TITLES[5] = {
    "Erkek Heykeli", "Mezar Taşı", "Akhilleus Lahdi",
    "Arabali Tarhunda Heykeli", "Lahit"
};

// Heykellerin açıklamaları burda
const std::string Statue::DESCRIPTIONS[5] = {
    "Tunc malzemesi kullanilmis bir erkek heykeli.\nMalzeme: Tunç\nDonem: Roma Donemi\nYil: MS 1. Yüzyil\nBuluntu Yeri: Adana Karatas",
    "Malzeme: Taş\nDonem: Roma Donemi\nYil: MS 2. Yüzyil",
    "Roma imparatorlugu donemi Attika tipi Akhilleus Lahitlerinin 2.grubuna dahildir.\nLahdin sol kisa yuzu ve on yuzu figurlere ayrilmistir.\nGec Antoninler Donemi, MS 170-190",
    "Bazalt ve kalker malzemesinden yapilmis bir heykel.\nMalzeme: Bazalt, Kalker\nDonem: Gec Hitit Donemi\nYil: MÖ 8. Yüzyil",
    "Mermer"
};

const std::string Statue::ARTISTS[5] = {
    "Roma Sanati", "Roma Sanati", "Roma Sanatci",
    "Gec Hitit Sanatci", "Roma Donemi"
};

const int Statue::YEARS[5] = {100, 200, 180, -800, -100};

// Heykeli oluşturur. Konum, boyut vs. ayarları burada
Statue::Statue(const std::string& modelPath, const glm::vec3& position,
               const glm::vec3& scale, const glm::vec3& rotation)
    : modelPath(modelPath), position(position), scale(scale), rotation(rotation),
      title(""), description(""), artist(""), year(0), VAO(0), VBO(0), EBO(0) {}

// Heykel silinince hafizayı temizler
Statue::~Statue() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    for (auto& mesh : meshes) {
        glDeleteVertexArrays(1, &mesh.VAO);
        glDeleteBuffers(1, &mesh.VBO);
        glDeleteBuffers(1, &mesh.EBO);
    }
}

// Heykelin modelini yükleme yeri
bool Statue::initialize() {
    return loadModel(modelPath);
}

// Heykeli ekrana çizme yeri
void Statue::render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, GLuint shaderProgram) {
    glUseProgram(shaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    GLint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    if (colorLoc != -1) glUniform3f(colorLoc, 1.0f, 0.5f, 0.0f);

    for (const auto& mesh : meshes) {
        if (mesh.VAO == 0) continue;

        for (size_t i = 0; i < mesh.textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
        }

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

glm::vec3 Statue::getPosition() const { return position; }
glm::vec3 Statue::getRotation() const { return rotation; }
// Heykel bilgilerini atama yeri
void Statue::setInformation(const std::string& t, const std::string& d, const std::string& a, int y) {
    title = t; description = d; artist = a; year = y;
}
const std::string& Statue::getTitle() const { return title; }
const std::string& Statue::getDescription() const { return description; }

// Tüm heykelleri oluşturp listeye ekleme
std::vector<Statue*> Statue::createStatueCollection(float roomWidth, float roomHeight, float roomLength) {
    float floorY = -roomHeight / 2.0f;
    std::vector<Statue*> statues;

    struct StatueConfig {
        int idx; glm::vec3 pos; float yOffset; glm::vec3 rot; float scale;
    };

    StatueConfig configs[] = {
        {0, glm::vec3(-1.0f, 0.0f, -1.0f), -0.4f, glm::vec3(0.0f, 225.0f, 0.0f), 0.6f},
        {1, glm::vec3(1.0f, 0.0f, -1.0f), -0.4f, glm::vec3(0.0f, -45.0f, 0.0f), 0.6f},
        {2, glm::vec3(-1.0f, 0.0f, 1.0f), -0.7f, glm::vec3(0.0f, 135.0f, 0.0f), 0.6f},
        {3, glm::vec3(1.0f, 0.0f, 1.0f), -0.37f, glm::vec3(0.0f, -135.0f, 0.0f), 0.6f},
        {4, glm::vec3(0.0f, 0.0f, 0.0f), -0.75f, glm::vec3(0.0f, 0.0f, 0.0f), 0.8f}
    };

    for (const auto& cfg : configs) {
        glm::vec3 pos(cfg.pos.x * roomWidth/2.5f, floorY + cfg.yOffset, cfg.pos.z * roomLength/2.5f);
        auto statue = new Statue(MODEL_PATHS[cfg.idx], pos, glm::vec3(cfg.scale), cfg.rot);
        statue->setInformation(TITLES[cfg.idx], DESCRIPTIONS[cfg.idx], ARTISTS[cfg.idx], YEARS[cfg.idx]);

        if (!statue->initialize()) {
            for (auto* s : statues) delete s;
            statues.clear();
            return statues;
        }
        statues.push_back(statue);
    }
    return statues;
}

// Robotun en yakinindaki heykeli bulur
int Statue::checkProximity(const std::vector<Statue*>& statues, const glm::vec3& robotPos, float proximityDistance) {
    int closest = -1;
    float minDist = proximityDistance;

    for (size_t i = 0; i < statues.size(); i++) {
        float dist = glm::distance(robotPos, statues[i]->getPosition());
        if (dist < minDist) {
            minDist = dist;
            closest = i;
        }
    }
    return closest;
}

// Modeli dosyadan yükler (Assimp)
bool Statue::loadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp hata: " << importer.GetErrorString() << std::endl;
        return false;
    }

    directory = path.substr(0, path.find_last_of('/'));
    if (directory.empty()) directory = path.substr(0, path.find_last_of('\\'));
    if (directory.empty()) directory = ".";

    processNode(scene->mRootNode, scene);
    return true;
}

// Modelin parçalarını  gezer
void Statue::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

// Modelin bir parçasını  işler
Statue::Mesh Statue::processMesh(aiMesh* mesh, const aiScene* scene) {
    Mesh result;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        if (mesh->HasNormals()) {
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        if (mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        result.vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            result.indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        result.textures.insert(result.textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        result.textures.insert(result.textures.end(), specularMaps.begin(), specularMaps.end());
    }

    setupMesh(result);
    return result;
}
// 3D modelleri GPU ya yüklüyor
void Statue::setupMesh(Mesh& mesh) {
    glGenVertexArrays(1, &mesh.VAO);
    glGenBuffers(1, &mesh.VBO);
    glGenBuffers(1, &mesh.EBO);

    glBindVertexArray(mesh.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

std::vector<Statue::Texture> Statue::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (const auto& tex : textures_loaded) {
            if (std::strcmp(tex.path.data(), str.C_Str()) == 0) {
                textures.push_back(tex);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = textureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}

GLuint Statue::textureFromFile(const std::string& path, const std::string& directory) {
    std::string filename = (directory != ".") ? directory + '/' + path : path;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (!data) {
        std::cerr << "Texture yüklenemedi: " << path << std::endl;
        return 0;
    }

    GLenum format = nrComponents == 1 ? GL_RED : nrComponents == 3 ? GL_RGB : GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}
