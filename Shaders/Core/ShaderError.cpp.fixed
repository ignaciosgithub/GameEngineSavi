#include "ShaderError.h"
#include <iostream>
#include <vector>

namespace Shaders {

void ShaderError::LogError(const std::string& message) {
    std::cerr << "Shader Error: " << message << std::endl;
}

std::string ShaderError::HandleCompileError(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        
        std::vector<GLchar> log(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, log.data());
        
        std::string errorMessage(log.begin(), log.end());
        LogError("Shader compilation failed: " + errorMessage);
        
        return errorMessage;
    }
    
    return "";
}

std::string ShaderError::HandleLinkError(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if (!success) {
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        
        std::vector<GLchar> log(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, log.data());
        
        std::string errorMessage(log.begin(), log.end());
        LogError("Shader program linking failed: " + errorMessage);
        
        return errorMessage;
    }
    
    return "";
}

} // namespace Shaders
