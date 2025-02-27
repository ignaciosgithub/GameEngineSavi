#include "ShaderAsset.h"
#include "../Core/ShaderError.h"
#include <iostream>

namespace Shaders {

// Initialize static member
std::unordered_map<std::string, ShaderProgram*> ShaderAsset::shaderPrograms;

ShaderProgram* ShaderAsset::LoadProgram(const std::string& vertPath, 
                                       const std::string& fragPath,
                                       const std::string& geomPath) {
    // Create a unique key for this shader combination
    std::string key = vertPath + ":" + fragPath;
    if (!geomPath.empty()) {
        key += ":" + geomPath;
    }
    
    // Check if we already have this shader program
    auto it = shaderPrograms.find(key);
    if (it != shaderPrograms.end()) {
        return it->second;
    }
    
    // Create new shader program
    ShaderProgram* program = new ShaderProgram();
    
    // Create and compile vertex shader
    Shader* vertexShader = new Shader(Shader::VERTEX);
    if (!vertexShader->LoadFromFile(vertPath)) {
        std::cerr << "Failed to load vertex shader from " << vertPath << std::endl;
        delete vertexShader;
        delete program;
        return nullptr;
    }
    
    if (!vertexShader->Compile()) {
        std::cerr << "Failed to compile vertex shader: " << vertexShader->GetError() << std::endl;
        delete vertexShader;
        delete program;
        return nullptr;
    }
    
    // Create and compile fragment shader
    Shader* fragmentShader = new Shader(Shader::FRAGMENT);
    if (!fragmentShader->LoadFromFile(fragPath)) {
        std::cerr << "Failed to load fragment shader from " << fragPath << std::endl;
        delete vertexShader;
        delete fragmentShader;
        delete program;
        return nullptr;
    }
    
    if (!fragmentShader->Compile()) {
        std::cerr << "Failed to compile fragment shader: " << fragmentShader->GetError() << std::endl;
        delete vertexShader;
        delete fragmentShader;
        delete program;
        return nullptr;
    }
    
    // Attach shaders to program
    program->AttachShader(vertexShader);
    program->AttachShader(fragmentShader);
    
    // Create and compile geometry shader if provided
    Shader* geometryShader = nullptr;
    if (!geomPath.empty()) {
        geometryShader = new Shader(Shader::GEOMETRY);
        if (!geometryShader->LoadFromFile(geomPath)) {
            std::cerr << "Failed to load geometry shader from " << geomPath << std::endl;
            delete vertexShader;
            delete fragmentShader;
            delete geometryShader;
            delete program;
            return nullptr;
        }
        
        if (!geometryShader->Compile()) {
            std::cerr << "Failed to compile geometry shader: " << geometryShader->GetError() << std::endl;
            delete vertexShader;
            delete fragmentShader;
            delete geometryShader;
            delete program;
            return nullptr;
        }
        
        program->AttachShader(geometryShader);
    }
    
    // Link program
    if (!program->Link()) {
        std::cerr << "Failed to link shader program: " << program->GetError() << std::endl;
        delete vertexShader;
        delete fragmentShader;
        if (geometryShader) delete geometryShader;
        delete program;
        return nullptr;
    }
    
    // Store in cache
    shaderPrograms[key] = program;
    
    // Log success
    std::cout << "Shader program loaded successfully: " << key << std::endl;
    
    return program;
}

ShaderProgram* ShaderAsset::GetProgram(const std::string& name) {
    auto it = shaderPrograms.find(name);
    if (it != shaderPrograms.end()) {
        return it->second;
    }
    return nullptr;
}

void ShaderAsset::Cleanup() {
    for (auto& pair : shaderPrograms) {
        delete pair.second;
    }
    shaderPrograms.clear();
}

} // namespace Shaders
