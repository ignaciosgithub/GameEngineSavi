#include "ShaderProgram.h"
#include "Shader.h"
#include "ShaderError.h"
#include <iostream>
#include <vector>

// Constructor
ShaderProgram::ShaderProgram() : handle(0) {
    // Create a new shader program
    handle = glCreateProgram();
}

// Destructor
ShaderProgram::~ShaderProgram() {
    if (handle != 0) {
        glDeleteProgram(handle);
        handle = 0;
    }
}

// Attach a shader to the program
bool ShaderProgram::AttachShader(Shader* shader) {
    if (!shader || shader->GetHandle() == 0) {
        std::cerr << "Error: Cannot attach invalid shader to program" << std::endl;
        return false;
    }
    
    // Attach the shader to the program
    glAttachShader(handle, shader->GetHandle());
    
    return true;
}

// Link the program
bool ShaderProgram::Link() {
    // Link the program
    glLinkProgram(handle);
    
    // Check if linking was successful
    GLint success = 0;
    glGetProgramiv(handle, GL_LINK_STATUS, &success);
    
    if (success == GL_FALSE) {
        // Get the error message
        std::string errorMessage = ShaderError::HandleLinkError(handle);
        
        // Print the error message
        std::cerr << "Error linking shader program: " << errorMessage << std::endl;
        
        return false;
    }
    
    return true;
}

// Use the program
void ShaderProgram::Use() const {
    if (handle != 0) {
        glUseProgram(handle);
    }
}

// Get the location of a uniform
GLint ShaderProgram::GetUniformLocation(const std::string& name) {
    // Check if we already have the location cached
    auto it = uniformLocations.find(name);
    if (it != uniformLocations.end()) {
        return it->second;
    }
    
    // Get the location from OpenGL
    GLint location = glGetUniformLocation(handle, name.c_str());
    
    // Cache the location
    uniformLocations[name] = location;
    
    return location;
}

// Set a float uniform
void ShaderProgram::SetUniform(const std::string& name, float value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

// Set an int uniform
void ShaderProgram::SetUniform(const std::string& name, int value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

// Set a bool uniform
void ShaderProgram::SetUniform(const std::string& name, bool value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value ? 1 : 0);
    }
}

// Set a vec3 uniform
void ShaderProgram::SetUniform(const std::string& name, float x, float y, float z) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, x, y, z);
    }
}

// Set a vec4 uniform
void ShaderProgram::SetUniform(const std::string& name, float x, float y, float z, float w) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

// Set a Vector3 uniform
void ShaderProgram::SetUniform(const std::string& name, const Vector3& value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

// Set a Matrix4x4 uniform
void ShaderProgram::SetUniform(const std::string& name, const Matrix4x4& value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value.elements[0][0]);
    }
}

// Set a matrix uniform
void ShaderProgram::SetUniform(const std::string& name, const float* matrix, bool transpose) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, matrix);
    }
}

// Set a texture uniform
void ShaderProgram::SetUniform(const std::string& name, GLuint textureID, GLuint textureUnit) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(location, textureUnit);
    }
}

// Set a float array uniform
void ShaderProgram::SetUniformArray(const std::string& name, const float* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1fv(location, count, values);
    }
}

// Set an int array uniform
void ShaderProgram::SetUniformArray(const std::string& name, const int* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1iv(location, count, values);
    }
}

// Set a Vector3 array uniform
void ShaderProgram::SetUniformArray(const std::string& name, const Vector3* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        // Convert Vector3 array to float array
        float* floatValues = new float[count * 3];
        for (int i = 0; i < count; i++) {
            floatValues[i * 3 + 0] = values[i].x;
            floatValues[i * 3 + 1] = values[i].y;
            floatValues[i * 3 + 2] = values[i].z;
        }
        
        glUniform3fv(location, count, floatValues);
        
        delete[] floatValues;
    }
}

// Set a Matrix4x4 array uniform
void ShaderProgram::SetUniformArray(const std::string& name, const Matrix4x4* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        // Convert Matrix4x4 array to float array
        float* floatValues = new float[count * 16];
        for (int i = 0; i < count; i++) {
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    floatValues[i * 16 + row * 4 + col] = values[i].elements[row][col];
                }
            }
        }
        
        glUniformMatrix4fv(location, count, GL_FALSE, floatValues);
        
        delete[] floatValues;
    }
}
