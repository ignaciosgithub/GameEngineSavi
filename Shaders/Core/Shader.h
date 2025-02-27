#ifndef SHADER_H
#define SHADER_H

#include "../../platform.h"
#include <string>

namespace Shaders {

/**
 * Shader class
 * Represents a single shader stage (vertex, fragment, geometry, etc.)
 */
class Shader {
public:
    /**
     * Shader types
     */
    enum Type {
        VERTEX,
        FRAGMENT,
        GEOMETRY
    };
    
    /**
     * Constructor
     * @param type The shader type
     */
    Shader(Type type);
    
    /**
     * Destructor
     */
    ~Shader();
    
    /**
     * Load shader source from a file
     * @param path The path to the shader file
     * @return True if successful, false otherwise
     */
    bool LoadFromFile(const std::string& path);
    
    /**
     * Load shader source from a string
     * @param source The shader source code
     * @return True if successful, false otherwise
     */
    bool LoadFromString(const std::string& source);
    
    /**
     * Compile the shader
     * @return True if successful, false otherwise
     */
    bool Compile();
    
    /**
     * Get the shader handle
     * @return The OpenGL shader handle
     */
    GLuint GetHandle() const { return handle; }
    
    /**
     * Get the shader type
     * @return The shader type
     */
    Type GetType() const { return type; }
    
    /**
     * Get the last error message
     * @return The error message
     */
    const std::string& GetError() const { return errorLog; }
    
private:
    GLuint handle;          // OpenGL shader handle
    Type type;              // Shader type
    std::string source;     // Shader source code
    std::string errorLog;   // Error log
    bool isCompiled;        // Is the shader compiled?
    
    /**
     * Get the OpenGL shader type
     * @return The OpenGL shader type
     */
    GLenum GetGLShaderType() const;
};

} // namespace Shaders

#endif // SHADER_H
