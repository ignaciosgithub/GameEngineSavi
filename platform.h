#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__linux__)
    #define PLATFORM_LINUX
#elif defined(__APPLE__)
    #define PLATFORM_MAC
#else
    #error "Unsupported platform"
#endif

// Platform-specific includes
#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <gl/gl.h>
    #include <GL/glu.h>
#elif defined(PLATFORM_LINUX)
    // On Linux, we'll conditionally include OpenGL headers
    // This allows the code to compile even without OpenGL installed
    #ifdef ENABLE_OPENGL
        #include <GL/gl.h>
        #include <GL/glu.h>
        #include <X11/Xlib.h>
        #include <X11/Xutil.h>
        #include <GL/glx.h>
    #else
        // Define OpenGL types for non-OpenGL builds
        typedef unsigned int GLuint;
        typedef float GLfloat;
        typedef unsigned int GLenum;
        #define GL_TRIANGLES 0x0004
        #define GL_RGBA 0x1908
        #define GL_RGB 0x1907
        #define GL_UNSIGNED_BYTE 0x1401
        #define GL_TEXTURE_2D 0x0DE1
        #define GL_TEXTURE_WRAP_S 0x2802
        #define GL_TEXTURE_WRAP_T 0x2803
        #define GL_TEXTURE_MIN_FILTER 0x2801
        #define GL_TEXTURE_MAG_FILTER 0x2800
        #define GL_LINEAR 0x2601
        #define GL_REPEAT 0x2901
        #define GL_BLEND 0x0BE2
        #define GL_SRC_ALPHA 0x0302
        #define GL_ONE_MINUS_SRC_ALPHA 0x0303
        #define GL_CULL_FACE 0x0B44
        #define GL_BACK 0x0405
        #define GL_FLAT 0x1D00
    #endif
#elif defined(PLATFORM_MAC)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#endif

#endif // PLATFORM_H
