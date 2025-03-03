#pragma once

#include <string>
#include <unordered_map>
#include "../../ThirdParty/OpenGL/include/GL/platform_gl.h"
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
    
    GLuint GetHandle() const { return handle; }
    
    // Uniform setters
    GLint GetUniformLocation(const std::string& name);
    
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, float x, float y, float z);
    void SetUniform(const std::string& name, float x, float y, float z, float w);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);
    void SetUniform(const std::string& name, const float* matrix, bool transpose = false);
    void SetUniform(const std::string& name, GLuint textureID, GLuint textureUnit);
    
    // Array uniform setters
    void SetUniformArray(const std::string& name, const float* values, int count);
    void SetUniformArray(const std::string& name, const int* values, int count);
    void SetUniformArray(const std::string& name, const Vector3* values, int count);
    void SetUniformArray(const std::string& name, const Matrix4x4* values, int count);
    
private:
    GLuint handle;
    std::unordered_map<std::string, GLint> uniformLocations;
};
