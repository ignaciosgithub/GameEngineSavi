#include "Shader.h"
#include "ShaderError.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Shaders {

Shader::Shader(Type type) : type(type), handle(0), isCompiled(false) {
    // Create shader
    handle = glCreateShader(GetGLShaderType());
}

Shader::~Shader() {
    // Delete shader
    if (handle != 0) {
        glDeleteShader(handle);
        handle = 0;
    }
}

bool Shader::LoadFromFile(const std::string& path) {
    // Open file
    std::ifstream file(path);
    if (!file.is_open()) {
        errorLog = "Failed to open file: " + path;
        return false;
    }
    
    // Read file
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    
    // Load source
    return LoadFromString(buffer.str());
}

bool Shader::LoadFromString(const std::string& source) {
    // Store source
    this->source = source;
    
    // Set source
    const char* sourcePtr = source.c_str();
    glShaderSource(handle, 1, &sourcePtr, nullptr);
    
    return true;
}

bool Shader::Compile() {
    // Compile shader
    glCompileShader(handle);
    
    // Check for errors
    errorLog = ShaderError::HandleCompileError(handle);
    isCompiled = errorLog.empty();
    
    return isCompiled;
}

GLenum Shader::GetGLShaderType() const {
    switch (type) {
        case VERTEX:
            return GL_VERTEX_SHADER;
        case FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case GEOMETRY:
            return GL_GEOMETRY_SHADER;
        default:
            return GL_VERTEX_SHADER;
    }
}

} // namespace Shaders
