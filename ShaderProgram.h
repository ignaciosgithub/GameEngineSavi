#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "platform.h"
#include <string>
#include <vector>
#include <map>
#include "Vector3.h"
#include "Matrix4x4.h"

// OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

// Forward declarations
class Shader;

// ShaderProgram class
class ShaderProgram {
private:
    GLuint programID;
    std::vector<Shader*> shaders;
    std::map<std::string, GLint> uniformLocations;
    
public:
    ShaderProgram();
    ~ShaderProgram();
    
    // Create and link program
    bool Link();
    
    // Use program
    void Use();
    
    // Attach shader
    void AttachShader(Shader* shader);
    
    // Get program ID
    GLuint GetProgramID() const;
    
    // Uniform setters
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);
    
    // Get uniform location
    GLint GetUniformLocation(const std::string& name);
};

#endif // SHADER_PROGRAM_H
