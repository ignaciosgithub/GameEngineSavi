#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "Shader.h"
#include "../../Vector3.h"
#include "../../Matrix4x4.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace Shaders {

/**
 * ShaderProgram class
 * Manages an OpenGL shader program with multiple shader stages
 */
class ShaderProgram {
public:
    /**
     * Constructor
     */
    ShaderProgram();
    
    /**
     * Destructor
     */
    ~ShaderProgram();
    
    /**
     * Attach a shader to the program
     * @param shader The shader to attach
     * @return True if successful, false otherwise
     */
    bool AttachShader(Shader* shader);
    
    /**
     * Link the shader program
     * @return True if successful, false otherwise
     */
    bool Link();
    
    /**
     * Use the shader program
     */
    void Use();
    
    /**
     * Get the program handle
     * @return The OpenGL program handle
     */
    GLuint GetHandle() const { return handle; }
    
    /**
     * Check if the program is linked
     * @return True if linked, false otherwise
     */
    bool IsLinked() const { return isLinked; }
    
    /**
     * Get the last error message
     * @return The error message
     */
    const std::string& GetError() const { return errorLog; }
    
    /**
     * Set a uniform value
     * @param name The uniform name
     * @param value The value to set
     */
    void SetUniform(const std::string& name, float value);
    void SetUniform(const std::string& name, int value);
    void SetUniform(const std::string& name, bool value);
    void SetUniform(const std::string& name, const Vector3& value);
    void SetUniform(const std::string& name, const Matrix4x4& value);
    void SetUniform(const std::string& name, GLuint textureUnit, GLuint textureID);
    
    /**
     * Set an array of uniform values
     * @param name The uniform name
     * @param values The array of values
     * @param count The number of elements in the array
     */
    void SetUniformArray(const std::string& name, const float* values, int count);
    void SetUniformArray(const std::string& name, const int* values, int count);
    void SetUniformArray(const std::string& name, const Vector3* values, int count);
    void SetUniformArray(const std::string& name, const Matrix4x4* values, int count);
    
private:
    GLuint handle;                  // OpenGL program handle
    bool isLinked;                  // Is the program linked?
    std::string errorLog;           // Error log
    std::vector<Shader*> shaders;   // Attached shaders
    
    // Cache for uniform locations
    std::unordered_map<std::string, GLint> uniformLocations;
    
    /**
     * Get the location of a uniform
     * @param name The uniform name
     * @return The uniform location
     */
    GLint GetUniformLocation(const std::string& name);
};

} // namespace Shaders

#endif // SHADER_PROGRAM_H
