#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    Shader();
    
    ~Shader();
    
    bool initialize();
    
    void use() const;
    
    unsigned int getID() const;
    
    void setBool(const std::string &name, bool value) const;
    
    void setInt(const std::string &name, int value) const;
    
    void setFloat(const std::string &name, float value) const;
    
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    
    void setVec3(const std::string &name, float x, float y, float z) const;
    
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
    void setVec3Array(const std::string &name, const glm::vec3 *values, int count) const;

private:
    unsigned int ID;
    
    static const char* vertexShaderSource;
    static const char* fragmentShaderSource;
    
    unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);
};

#endif
