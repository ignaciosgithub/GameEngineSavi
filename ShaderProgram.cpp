#include "ShaderProgram.h"
#include "Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>
#include <map>

ShaderProgram::ShaderProgram() : handle(0) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        handle = graphics->CreateProgram();
    }
}

ShaderProgram::~ShaderProgram() {
    if (handle != 0) {
        auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
        if (graphics) {
            graphics->DeleteProgram(handle);
            handle = 0;
        }
    }
}

GLint ShaderProgram::GetUniformLocation(const std::string& name) {
    auto it = uniformLocations.find(name);
    if (it != uniformLocations.end()) {
        return it->second;
    }
    
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    GLint location = -1;
    
    if (graphics) {
        location = graphics->GetUniformLocation(handle, name);
    }
    
    uniformLocations[name] = location;
    
    return location;
}

void ShaderProgram::SetUniform(const std::string& name, int value) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        GLint location = GetUniformLocation(name);
        std::cout << "ShaderProgram::SetUniform - Setting int uniform '" << name << "' to value: " << value << " at location: " << location << std::endl;
        graphics->SetUniform1i(handle, name, value);
    }
}

void ShaderProgram::SetUniform(const std::string& name, float value) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        GLint location = GetUniformLocation(name);
        std::cout << "ShaderProgram::SetUniform - Setting float uniform '" << name << "' to value: " << value << " at location: " << location << std::endl;
        graphics->SetUniform1f(handle, name, value);
    }
}

void ShaderProgram::SetUniform(const std::string& name, const Vector3& value) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        GLint location = GetUniformLocation(name);
        std::cout << "ShaderProgram::SetUniform - Setting Vector3 uniform '" << name << "' to value: (" 
                  << value.x << ", " << value.y << ", " << value.z << ") at location: " << location << std::endl;
        graphics->SetUniform3f(handle, name, value.x, value.y, value.z);
    }
}

void ShaderProgram::SetUniform(const std::string& name, const Matrix4x4& value) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        GLint location = GetUniformLocation(name);
        std::cout << "ShaderProgram::SetUniform - Setting Matrix4x4 uniform '" << name << "' at location: " << location << std::endl;
        graphics->SetUniformMatrix4fv(handle, name, &value.elements[0][0]);
    }
}
