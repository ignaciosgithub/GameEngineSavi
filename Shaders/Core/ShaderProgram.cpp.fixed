#include "ShaderProgram.h"
#include "ShaderError.h"
#include <iostream>

namespace Shaders {

ShaderProgram::ShaderProgram() : handle(0), isLinked(false) {
    // Create program
    handle = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
    // Delete program
    if (handle != 0) {
        glDeleteProgram(handle);
        handle = 0;
    }
}

bool ShaderProgram::AttachShader(Shader* shader) {
    if (!shader) {
        errorLog = "Invalid shader";
        return false;
    }
    
    // Attach shader
    glAttachShader(handle, shader->GetHandle());
    
    // Store shader
    shaders.push_back(shader);
    
    return true;
}

bool ShaderProgram::Link() {
    // Link program
    glLinkProgram(handle);
    
    // Check for errors
    errorLog = ShaderError::HandleLinkError(handle);
    isLinked = errorLog.empty();
    
    // Clear uniform locations cache
    uniformLocations.clear();
    
    return isLinked;
}

void ShaderProgram::Use() {
    if (isLinked) {
        glUseProgram(handle);
    }
}

GLint ShaderProgram::GetUniformLocation(const std::string& name) {
    // Check cache
    auto it = uniformLocations.find(name);
    if (it != uniformLocations.end()) {
        return it->second;
    }
    
    // Get location
    GLint location = glGetUniformLocation(handle, name.c_str());
    
    // Store in cache
    uniformLocations[name] = location;
    
    return location;
}

void ShaderProgram::SetUniform(const std::string& name, float value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void ShaderProgram::SetUniform(const std::string& name, int value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void ShaderProgram::SetUniform(const std::string& name, bool value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value ? 1 : 0);
    }
}

void ShaderProgram::SetUniform(const std::string& name, const Vector3& value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void ShaderProgram::SetUniform(const std::string& name, const Matrix4x4& value) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value.elements[0][0]);
    }
}

void ShaderProgram::SetUniform(const std::string& name, GLuint textureUnit, GLuint textureID) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(location, textureUnit);
    }
}

void ShaderProgram::SetUniformArray(const std::string& name, const float* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1fv(location, count, values);
    }
}

void ShaderProgram::SetUniformArray(const std::string& name, const int* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform1iv(location, count, values);
    }
}

void ShaderProgram::SetUniformArray(const std::string& name, const Vector3* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniform3fv(location, count, reinterpret_cast<const float*>(values));
    }
}

void ShaderProgram::SetUniformArray(const std::string& name, const Matrix4x4* values, int count) {
    GLint location = GetUniformLocation(name);
    if (location != -1) {
        glUniformMatrix4fv(location, count, GL_FALSE, reinterpret_cast<const float*>(values));
    }
}

} // namespace Shaders
