#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../external/stb/stb_image.h"

class Texture {
public:
    unsigned int id;
    std::string type;
    std::string path;

    Texture() : id(0), type(""), path("") {}

    static unsigned int loadTexture(const char* path) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        
       
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        
        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            else {
                format = GL_RGB; 
                std::cout << "Uyarı: Bilinmeyen kanal sayısı: " << nrChannels << std::endl;
            }
            
           
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            std::cout << "Doku yüklendi: " << path << " (" << width << "x" << height << ", " << nrChannels << " kanal)" << std::endl;
            stbi_image_free(data);
        } else {
            std::cout << "Doku yüklenemedi: " << path << std::endl;
            stbi_image_free(data); 
        }

        return textureID;
    }
};

#endif 
