#ifndef SHADER_ERROR_H
#define SHADER_ERROR_H

#include "../../platform.h"
#include <string>

namespace Shaders {

/**
 * ShaderError class
 * Provides static methods for handling shader errors
 */
class ShaderError {
public:
    /**
     * Log an error message
     * @param message The error message
     */
    static void LogError(const std::string& message);
    
    /**
     * Handle a shader compilation error
     * @param shader The shader handle
     * @return The error message
     */
    static std::string HandleCompileError(GLuint shader);
    
    /**
     * Handle a shader program link error
     * @param program The program handle
     * @return The error message
     */
    static std::string HandleLinkError(GLuint program);
};

} // namespace Shaders

#endif // SHADER_ERROR_H
