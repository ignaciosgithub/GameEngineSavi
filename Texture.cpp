#include "Texture.h"
#include "platform.h"

// Platform-specific OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#else
#include <GL/gl.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Debugger.h"
#include <iostream>
#include <stdexcept>

Texture::Texture() : id(0), width(0), height(0), channels(0), tiling_x(1.0f), tiling_y(1.0f) {
}

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

bool Texture::load(const std::string& path) {
    return Debugger::GetInstance().TryImport([&]() -> bool {
        // Load image using stb_image
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Failed to load texture: " + path);
        }
        
        // Generate OpenGL texture
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        
        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // Upload texture data
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        // Free image data
        stbi_image_free(data);
        
        return true;
    }, path, "texture");
}

void Texture::bind() {
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setTiling(float x, float y) {
    tiling_x = x;
    tiling_y = y;
}
