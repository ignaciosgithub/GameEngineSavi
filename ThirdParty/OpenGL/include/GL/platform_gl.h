#ifndef PLATFORM_GL_H
#define PLATFORM_GL_H

// Platform-specific includes
#ifdef _WIN32
    #include <windows.h>
    #include <GL/gl.h>
#else
    #include <GL/gl.h>
#endif

#endif // PLATFORM_GL_H
