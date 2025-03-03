#include "ShaderError.h"
#include <iostream>

// Handle shader compilation errors
std::string ShaderError::HandleCompileError(GLuint shader) {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    
    if (success == GL_FALSE) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0) {
            GLchar* log = new GLchar[logLength];
            glGetShaderInfoLog(shader, logLength, NULL, log);
            
            std::string errorLog = log;
            delete[] log;
            
            return errorLog;
        }
    }
    
    return "";
}

// Handle shader program linking errors
std::string ShaderError::HandleLinkError(GLuint program) {
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    
    if (success == GL_FALSE) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        
        if (logLength > 0) {
            GLchar* log = new GLchar[logLength];
            glGetProgramInfoLog(program, logLength, NULL, log);
            
            std::string errorLog = log;
            delete[] log;
            
            return errorLog;
        }
    }
    
    return "";
}
