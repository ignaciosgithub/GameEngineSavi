#pragma once

#include <string>
#include "../../ThirdParty/OpenGL/include/GL/platform_gl.h"

class Shader {
public:
    enum Type {
        VERTEX,
        FRAGMENT
    };
    
    Shader(Type type);
    ~Shader();
    
    bool LoadFromFile(const std::string& filename);
    bool LoadFromString(const std::string& source, GLenum shaderType = 0);
    bool Compile();
    
    GLuint GetHandle() const { return handle; }
    Type GetType() const { return type; }
    
private:
    GLuint handle;
    Type type;
    std::string source;
    
    GLenum GetGLShaderType() const;
};
