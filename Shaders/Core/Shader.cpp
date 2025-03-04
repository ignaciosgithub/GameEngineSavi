#include "Shader.h"
#include "ShaderError.h"
#include "../../Debugger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Constructor
Shader::Shader(Type type) : type(type), handle(0) {
    // Create a new shader
    handle = glCreateShader(GetGLShaderType());
}

// Destructor
Shader::~Shader() {
    if (handle != 0) {
        glDeleteShader(handle);
        handle = 0;
    }
}

// Load shader from file
bool Shader::LoadFromFile(const std::string& filename) {
    return Debugger::GetInstance().TryImport([&]() -> bool {
        // Open the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open shader file: " + filename);
        }
        
        // Read the file contents
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        // Close the file
        file.close();
        
        // Load the shader from the string
        return LoadFromString(buffer.str());
    }, filename, "shader");
}

// Load shader from string
bool Shader::LoadFromString(const std::string& source, GLenum shaderType) {
    // Store the source
    this->source = source;
    
    // Set the source
    const GLchar* sourcePtr = source.c_str();
    glShaderSource(handle, 1, &sourcePtr, NULL);
    
    return true;
}

// Compile the shader
bool Shader::Compile() {
    // Compile the shader
    glCompileShader(handle);
    
    // Check if compilation was successful
    GLint success = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
    
    if (success == GL_FALSE) {
        // Get the error message
        std::string errorMessage = ShaderError::HandleCompileError(handle);
        
        // Print the error message
        std::cerr << "Error compiling shader: " << errorMessage << std::endl;
        
        return false;
    }
    
    return true;
}

// Get the OpenGL shader type
GLenum Shader::GetGLShaderType() const {
    switch (type) {
        case VERTEX:
            return GL_VERTEX_SHADER;
        case FRAGMENT:
            return GL_FRAGMENT_SHADER;
        default:
            return GL_VERTEX_SHADER;
    }
}
