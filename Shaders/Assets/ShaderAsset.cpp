#include "ShaderAsset.h"
#include "../Core/ShaderError.h"
#include "../../Debugger.h"
#include <iostream>
#include <stdexcept>

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
    
    // Use TryImport to handle errors
    ShaderProgram* program = nullptr;
    bool success = Debugger::GetInstance().TryImport([&]() -> bool {
        // Create new shader program
        program = new ShaderProgram();
        
        // Create and compile vertex shader
        Shader* vertexShader = new Shader(Shader::VERTEX);
        if (!vertexShader->LoadFromFile(vertPath)) {
            throw std::runtime_error("Failed to load vertex shader from " + vertPath);
        }
        
        if (!vertexShader->Compile()) {
            throw std::runtime_error("Failed to compile vertex shader: " + vertexShader->GetError());
        }
        
        // Create and compile fragment shader
        Shader* fragmentShader = new Shader(Shader::FRAGMENT);
        if (!fragmentShader->LoadFromFile(fragPath)) {
            delete vertexShader;
            throw std::runtime_error("Failed to load fragment shader from " + fragPath);
        }
        
        if (!fragmentShader->Compile()) {
            delete vertexShader;
            throw std::runtime_error("Failed to compile fragment shader: " + fragmentShader->GetError());
        }
        
        // Attach shaders to program
        program->AttachShader(vertexShader);
        program->AttachShader(fragmentShader);
        
        // Create and compile geometry shader if provided
        Shader* geometryShader = nullptr;
        if (!geomPath.empty()) {
            geometryShader = new Shader(Shader::GEOMETRY);
            if (!geometryShader->LoadFromFile(geomPath)) {
                throw std::runtime_error("Failed to load geometry shader from " + geomPath);
            }
            
            if (!geometryShader->Compile()) {
                throw std::runtime_error("Failed to compile geometry shader: " + geometryShader->GetError());
            }
            
            program->AttachShader(geometryShader);
        }
        
        // Link program
        if (!program->Link()) {
            throw std::runtime_error("Failed to link shader program: " + program->GetError());
        }
        
        // Store in cache
        shaderPrograms[key] = program;
        
        // Log success
        std::cout << "Shader program loaded successfully: " << key << std::endl;
        
        return true;
    }, key, "shader program");
    
    if (!success && program != nullptr) {
        delete program;
        program = nullptr;
    }
    
    return success ? shaderPrograms[key] : nullptr;
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
