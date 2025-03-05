#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

// Define GLEW_STATIC before including glew.h
#define GLEW_STATIC

// Include GLEW first
#include <GL/glew.h>

// Platform-specific includes
#ifdef _WIN32
    #include <windows.h>
#endif

// Standard OpenGL includes
#include <GL/gl.h>

#endif // PLATFORM_GL_H
