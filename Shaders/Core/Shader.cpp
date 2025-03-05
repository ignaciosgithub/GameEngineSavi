#include "Shader.h"
#include "ShaderError.h"
#include "../../Debugger.h"
#include "../../Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Constructor
Shader::Shader(Type type) : type(type), handle(0) {
    // Create a new shader
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        handle = graphics->CreateShader(GetShaderType());
    }
}

// Destructor
Shader::~Shader() {
    if (handle != 0) {
        auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
        if (graphics) {
            graphics->DeleteShader(handle);
            handle = 0;
        }
    }
}

// Load shader from file
bool Shader::LoadFromFile(const std::string& filename) {
    return Debugger::GetInstance().TryImport([&]() -> bool {
        // Open the file
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open shader file: " + filename);
        }
        
        // Read the file contents
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        // Close the file
        file.close();
        
        // Load the shader from the string
        return LoadFromString(buffer.str());
    }, filename, "shader");
}

// Load shader from string
bool Shader::LoadFromString(const std::string& source, int shaderType) {
    // Store the source
    this->source = source;
    
    // Set the source
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        graphics->ShaderSource(handle, source);
    }
    
    return true;
}

// Compile the shader
bool Shader::Compile() {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return false;
    }
    
    // Compile the shader
    graphics->CompileShader(handle);
    
    // Check if compilation was successful
    if (!graphics->GetShaderCompileStatus(handle)) {
        // Get the error message
        std::string errorMessage = graphics->GetShaderInfoLog(handle);
        
        // Print the error message
        std::cerr << "Error compiling shader: " << errorMessage << std::endl;
        
        return false;
    }
    
    return true;
}

// Get the shader type
int Shader::GetShaderType() const {
    switch (type) {
        case VERTEX:
            #ifndef PLATFORM_WINDOWS
            return 0x8B31; // GL_VERTEX_SHADER
            #else
            return 1; // D3D11_VERTEX_SHADER
            #endif
        case FRAGMENT:
            #ifndef PLATFORM_WINDOWS
            return 0x8B30; // GL_FRAGMENT_SHADER
            #else
            return 2; // D3D11_PIXEL_SHADER
            #endif
        default:
            #ifndef PLATFORM_WINDOWS
            return 0x8B31; // GL_VERTEX_SHADER
            #else
            return 1; // D3D11_VERTEX_SHADER
            #endif
    }
}
