#pragma once
#include <string>
#include "Vector3.h"
#include "Matrix4x4.h"
#include <map>
#include "platform.h"

// Forward declaration of needed OpenGL types
#ifndef PLATFORM_WINDOWS
typedef int GLint;
typedef unsigned int GLuint;
#endif

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    
    // Program handle getter/setter
    unsigned int GetHandle() const { return handle; }
    void SetHandle(unsigned int h) { handle = h; }
    void SetProgramId(unsigned int id) { handle = id; }
    
    // Uniform setters
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);
    
private:
    unsigned int handle = 0;
    std::map<std::string, GLint> uniformLocations;
    
    // Get uniform location helper
    GLint GetUniformLocation(const std::string& name);
};
