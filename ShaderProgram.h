#pragma once
#include <string>
#include "Vector3.h"
#include "Matrix4x4.h"

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);
};
