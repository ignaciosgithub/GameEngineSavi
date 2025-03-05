#include "ShaderError.h"
#include "../../Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>

// Handle shader compilation errors
std::string ShaderError::HandleCompileError(GLuint shader) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return "Graphics API not available";
    }
    
    if (!graphics->GetShaderCompileStatus(shader)) {
        return graphics->GetShaderInfoLog(shader);
    }
    
    return "";
}

// Handle shader program linking errors
std::string ShaderError::HandleLinkError(GLuint program) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return "Graphics API not available";
    }
    
    if (!graphics->GetProgramLinkStatus(program)) {
        return graphics->GetProgramInfoLog(program);
    }
    
    return "";
}
