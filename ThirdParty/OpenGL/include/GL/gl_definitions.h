#ifndef GAME_ENGINE_SAVI_GL_DEFINITIONS_H
#define GAME_ENGINE_SAVI_GL_DEFINITIONS_H

// Define GLEW_STATIC before including glew.h
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// Define missing types if needed
#ifndef __gl_glext_h_
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync* GLsync;
#endif

// Include GLEW first
#include <GL/glew.h>

// Then include other OpenGL headers
#ifdef _WIN32
    #include <windows.h>
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

#endif // GAME_ENGINE_SAVI_GL_DEFINITIONS_H
