#ifndef SHADER_ASSET_H
#define SHADER_ASSET_H

#include "../Core/ShaderProgram.h"
#include <string>
#include <unordered_map>

namespace Shaders {

/**
 * ShaderAsset class
 * Manages shader assets and provides loading functionality
 */
class ShaderAsset {
public:
    /**
     * Load a shader program from files
     * @param vertPath Path to the vertex shader file
     * @param fragPath Path to the fragment shader file
     * @param geomPath Path to the geometry shader file (optional)
     * @return Pointer to the loaded shader program, or nullptr if loading failed
     */
    static ShaderProgram* LoadProgram(const std::string& vertPath, 
                                     const std::string& fragPath,
                                     const std::string& geomPath = "");
    
    /**
     * Get a shader program by name
     * @param name Name of the shader program
     * @return Pointer to the shader program, or nullptr if not found
     */
    static ShaderProgram* GetProgram(const std::string& name);
    
    /**
     * Clean up all shader programs
     */
    static void Cleanup();
    
private:
    // Map of shader programs
    static std::unordered_map<std::string, ShaderProgram*> shaderPrograms;
};

} // namespace Shaders

#endif // SHADER_ASSET_H
