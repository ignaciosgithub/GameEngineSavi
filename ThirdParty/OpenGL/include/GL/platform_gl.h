// ThirdParty/OpenGL/include/GL/platform_gl.h
#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

#include "../../../platform.h"

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include "gl.h"
    #include "glu.h"
#else
    #include "gl.h"
    #include "glu.h"
    #include "glx.h"
#endif

#include "ThirdParty/OpenGL/include/GL/gl_types.h"

#endif // PLATFORM_GL_H
