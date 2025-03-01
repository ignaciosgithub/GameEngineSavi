#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "Shader.h"
#include <string>
#include <vector>
#include <map>

class ShaderProgram {
private:
    GLuint programID;
    std::vector<Shader> shaders;
    std::map<std::string, GLint> uniformLocations;
    
    bool isLinked;
    
    // Private methods
    void DetachShaders();
    void LinkProgram();
    
public:
    ShaderProgram();
    ~ShaderProgram();
    
    // Add a shader to the program
    void AddShader(const Shader& shader);
    
    // Link the program
    bool Link();
    
    // Use the program
    void Use() const;
    
    // Get the program ID
    GLuint GetProgramID() const;
    
    // Check if the program is linked
    bool IsLinked() const;
    
    // Get the location of a uniform variable
    GLint GetUniformLocation(const std::string& name);
    
    // Set uniform variables
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, float x, float y, float z);
    void SetUniform(const std::string& name, float x, float y, float z, float w);
    void SetUniform(const std::string& name, const float* matrix, bool transpose = false);
};

#endif // SHADER_PROGRAM_H
