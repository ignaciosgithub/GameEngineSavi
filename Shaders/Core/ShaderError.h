#pragma once

#include <string>
#include "../../Graphics/Core/IGraphicsAPI.h"

class ShaderError {
public:
    static std::string HandleCompileError(unsigned int shader);
    static std::string HandleLinkError(unsigned int program);
};
