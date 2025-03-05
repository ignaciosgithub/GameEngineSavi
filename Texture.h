#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "platform.h" // Include platform.h for platform-specific macros
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h" // Use our centralized GL definitions

class Texture {
public:
    GLuint id;
    int width, height, channels;
    float tiling_x, tiling_y;
    
    Texture();
    ~Texture();
    
    bool load(const std::string& path);
    void bind();
    void unbind();
    void setTiling(float x, float y);
};

#endif // TEXTURE_H
