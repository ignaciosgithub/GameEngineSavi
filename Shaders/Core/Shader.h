#pragma once

#include <string>
#include "../../Graphics/Core/IGraphicsAPI.h"

class Shader {
public:
    enum Type {
        VERTEX,
        FRAGMENT
    };
    
    Shader(Type type);
    ~Shader();
    
    bool LoadFromFile(const std::string& filename);
    bool LoadFromString(const std::string& source, int shaderType = 0);
    bool Compile();
    
    unsigned int GetHandle() const { return handle; }
    Type GetType() const { return type; }
    
private:
    unsigned int handle;
    Type type;
    std::string source;
    
    int GetShaderType() const;
};
