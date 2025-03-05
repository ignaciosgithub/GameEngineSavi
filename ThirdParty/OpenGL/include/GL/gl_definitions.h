#ifndef GAME_ENGINE_SAVI_GL_DEFINITIONS_H
#define GAME_ENGINE_SAVI_GL_DEFINITIONS_H

// Include standard headers
#include <cstdint>
#include "../../../../platform.h"
#include <iostream>

// Only include OpenGL headers for non-Windows platforms
#ifndef PLATFORM_WINDOWS

// Define GLEW_STATIC before including glew.h, but only if not already defined
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// Include GLEW first, as it must precede other OpenGL headers
#include <GL/glew.h>

// Then include other OpenGL headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#else
// Windows-specific DirectX headers are included in DirectXGraphicsAPI.h
#endif

// Helper function to log OpenGL header inclusion status
inline void LogOpenGLHeaderStatus() {
    std::cout << "OpenGL headers included successfully" << std::endl;
}

#endif // GAME_ENGINE_SAVI_GL_DEFINITIONS_H
