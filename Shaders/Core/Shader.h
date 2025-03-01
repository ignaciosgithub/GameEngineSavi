#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include "../../GL/gl_types.h"

class Shader {
public:
    // Constructor
    Shader();
    
    // Destructor
    ~Shader();
    
    // Load shader from file
    bool LoadFromFile(const std::string& filename, GLenum shaderType);
    
    // Load shader from string
    bool LoadFromString(const std::string& source, GLenum shaderType);
    
    // Compile the shader
    bool Compile();
    
    // Check if the shader is compiled
    bool IsCompiled() const;
    
    // Get the shader handle
    GLuint GetHandle() const { return handle; }
    
    // Get the shader type
    GLenum GetType() const;
    
    // Get the shader source
    const std::string& GetSource() const;
    
    // Get the shader info log
    std::string GetInfoLog() const;
    
private:
    std::string source;      // Shader source code
    GLenum type;             // Shader type (vertex, fragment, etc.)
    bool compiled;           // Is the shader compiled?
    GLuint handle;          // OpenGL shader handle
    
    // Get the OpenGL shader type from the file extension
    GLenum GetShaderTypeFromExtension(const std::string& filename);
    
    // Get the OpenGL shader type from the shader type
    GLenum GetGLShaderType() const;
};

#endif // SHADER_H
