#pragma once

#include <string>
#include "../../ThirdParty/OpenGL/include/GL/platform_gl.h"

class ShaderError {
public:
    static std::string HandleCompileError(GLuint shader);
    static std::string HandleLinkError(GLuint program);
};
