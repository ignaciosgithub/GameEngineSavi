#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram() {
    std::cout << "ShaderProgram initialized" << std::endl;
}

ShaderProgram::~ShaderProgram() {
    std::cout << "ShaderProgram destroyed" << std::endl;
}

void ShaderProgram::SetUniform(const std::string& name, int value) {
    // Stub implementation
}

void ShaderProgram::SetUniform(const std::string& name, float value) {
    // Stub implementation
}

void ShaderProgram::SetUniform(const std::string& name, const Vector3& value) {
    // Stub implementation
}

void ShaderProgram::SetUniform(const std::string& name, const Matrix4x4& value) {
    // Stub implementation
}
