#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <gl/gl.h>

// Forward declaration of stbi functions to avoid including the full header here
// The implementation will be in Texture.cpp
unsigned char* stbi_load(char const* filename, int* x, int* y, int* channels_in_file, int desired_channels);
void stbi_image_free(void* retval_from_stbi_load);

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
