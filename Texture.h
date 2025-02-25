#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "platform.h" // Include platform.h for platform-specific macros

// Platform-specific OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#else
#include <GL/gl.h>
#endif

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
