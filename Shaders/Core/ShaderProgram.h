#pragma once

#include <string>
#include <unordered_map>
#include "../../Graphics/Core/IGraphicsAPI.h"
#include "../../Vector3.h"
#include "../../Matrix4x4.h"

class Shader;

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();
    
    bool AttachShader(Shader* shader);
    bool Link();
    void Use() const;
    
    unsigned int GetHandle() const { return handle; }
    
    // Uniform setters
    int GetUniformLocation(const std::string& name);
    
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, float x, float y, float z);
    void SetUniform(const std::string& name, float x, float y, float z, float w);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);
    void SetUniform(const std::string& name, const float* matrix, bool transpose = false);
    void SetUniform(const std::string& name, unsigned int textureID, unsigned int textureUnit);
    
    // Array uniform setters
    void SetUniformArray(const std::string& name, const float* values, int count);
    void SetUniformArray(const std::string& name, const int* values, int count);
    void SetUniformArray(const std::string& name, const Vector3* values, int count);
    void SetUniformArray(const std::string& name, const Matrix4x4* values, int count);
    
private:
    unsigned int handle;
    std::unordered_map<std::string, int> uniformLocations;
};
