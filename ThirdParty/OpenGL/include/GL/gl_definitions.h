#ifndef GAME_ENGINE_SAVI_GL_DEFINITIONS_H
#define GAME_ENGINE_SAVI_GL_DEFINITIONS_H

// Include standard headers
#include <cstdint>
#include <iostream>
#include "../../../../platform.h"
#include "../../../../Debugger.h"

// Only include OpenGL headers for non-Windows platforms
#ifndef PLATFORM_WINDOWS

// Define GLEW_STATIC before including glew.h
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// Define missing types if needed
#ifndef __gl_glext_h_
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
typedef struct __GLsync* GLsync;
#endif

// Define APIENTRY if not already defined
#ifndef APIENTRY
#ifdef _WIN32
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif
#endif

// Define GL types needed before including glew.h
#ifndef GL_FLOAT
typedef float GLfloat;
typedef double GLdouble;
typedef int GLint;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef int GLenum;
typedef unsigned int GLbitfield;
typedef int GLsizei;
typedef char GLchar;
typedef void GLvoid;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;
typedef unsigned char GLubyte;
typedef short GLshort;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLclampf;
typedef double GLclampd;
#endif

// Include GLEW first
try {
    #include <GL/glew.h>
    std::cout << "Successfully included GL/glew.h" << std::endl;
} catch (...) {
    std::cout << "Unknown error including GL/glew.h" << std::endl;
    Debugger::GetInstance().LogError("Unknown error including GL/glew.h", "gl_definitions.h", "Include GLEW", -1);
}

// Then include other OpenGL headers
try {
    #include <GL/gl.h>
    std::cout << "Successfully included GL/gl.h" << std::endl;
} catch (...) {
    std::cout << "Unknown error including GL/gl.h" << std::endl;
    Debugger::GetInstance().LogError("Unknown error including GL/gl.h", "gl_definitions.h", "Include GL", -1);
}

try {
    #include <GL/glu.h>
    std::cout << "Successfully included GL/glu.h" << std::endl;
} catch (...) {
    std::cout << "Unknown error including GL/glu.h" << std::endl;
    Debugger::GetInstance().LogError("Unknown error including GL/glu.h", "gl_definitions.h", "Include GLU", -1);
}

try {
    #include <GL/glut.h>
    std::cout << "Successfully included GL/glut.h" << std::endl;
} catch (...) {
    std::cout << "Unknown error including GL/glut.h" << std::endl;
    Debugger::GetInstance().LogError("Unknown error including GL/glut.h", "gl_definitions.h", "Include GLUT", -1);
}

// Define missing function pointer types with ARB/EXT suffixes
// These are needed for compatibility with GLEW
try {
    #ifndef PFNGLCLIENTACTIVETEXTUREARBPROC
    typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
    #endif
    std::cout << "Successfully defined PFNGLCLIENTACTIVETEXTUREARBPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLCLIENTACTIVETEXTUREARBPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLCLIENTACTIVETEXTUREARBPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLLOADTRANSPOSEMATRIXDARBPROC
    typedef void (APIENTRY * PFNGLLOADTRANSPOSEMATRIXDARBPROC) (const GLdouble m[16]);
    #endif
    std::cout << "Successfully defined PFNGLLOADTRANSPOSEMATRIXDARBPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLLOADTRANSPOSEMATRIXDARBPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLLOADTRANSPOSEMATRIXDARBPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLLOADTRANSPOSEMATRIXFARBPROC
    typedef void (APIENTRY * PFNGLLOADTRANSPOSEMATRIXFARBPROC) (const GLfloat m[16]);
    #endif
    std::cout << "Successfully defined PFNGLLOADTRANSPOSEMATRIXFARBPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLLOADTRANSPOSEMATRIXFARBPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLLOADTRANSPOSEMATRIXFARBPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLMULTTRANSPOSEMATRIXDARBPROC
    typedef void (APIENTRY * PFNGLMULTTRANSPOSEMATRIXDARBPROC) (const GLdouble m[16]);
    #endif
    std::cout << "Successfully defined PFNGLMULTTRANSPOSEMATRIXDARBPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLMULTTRANSPOSEMATRIXDARBPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLMULTTRANSPOSEMATRIXDARBPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLMULTTRANSPOSEMATRIXFARBPROC
    typedef void (APIENTRY * PFNGLMULTTRANSPOSEMATRIXFARBPROC) (const GLfloat m[16]);
    #endif
    std::cout << "Successfully defined PFNGLMULTTRANSPOSEMATRIXFARBPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLMULTTRANSPOSEMATRIXFARBPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLMULTTRANSPOSEMATRIXFARBPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLCONVOLUTIONPARAMETERFEXTPROC
    typedef void (APIENTRY * PFNGLCONVOLUTIONPARAMETERFEXTPROC) (GLenum target, GLenum pname, GLfloat param);
    #endif
    std::cout << "Successfully defined PFNGLCONVOLUTIONPARAMETERFEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLCONVOLUTIONPARAMETERFEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLCONVOLUTIONPARAMETERFEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLCONVOLUTIONPARAMETERFVEXTPROC
    typedef void (APIENTRY * PFNGLCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat *params);
    #endif
    std::cout << "Successfully defined PFNGLCONVOLUTIONPARAMETERFVEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLCONVOLUTIONPARAMETERFVEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLCONVOLUTIONPARAMETERFVEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLCONVOLUTIONPARAMETERIEXTPROC
    typedef void (APIENTRY * PFNGLCONVOLUTIONPARAMETERIEXTPROC) (GLenum target, GLenum pname, GLint param);
    #endif
    std::cout << "Successfully defined PFNGLCONVOLUTIONPARAMETERIEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLCONVOLUTIONPARAMETERIEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLCONVOLUTIONPARAMETERIEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLCONVOLUTIONPARAMETERIVEXTPROC
    typedef void (APIENTRY * PFNGLCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint *params);
    #endif
    std::cout << "Successfully defined PFNGLCONVOLUTIONPARAMETERIVEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLCONVOLUTIONPARAMETERIVEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLCONVOLUTIONPARAMETERIVEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLGETHISTOGRAMPARAMETERFVEXTPROC
    typedef void (APIENTRY * PFNGLGETHISTOGRAMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat *params);
    #endif
    std::cout << "Successfully defined PFNGLGETHISTOGRAMPARAMETERFVEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLGETHISTOGRAMPARAMETERFVEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLGETHISTOGRAMPARAMETERFVEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLGETHISTOGRAMPARAMETERIVEXTPROC
    typedef void (APIENTRY * PFNGLGETHISTOGRAMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
    #endif
    std::cout << "Successfully defined PFNGLGETHISTOGRAMPARAMETERIVEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLGETHISTOGRAMPARAMETERIVEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLGETHISTOGRAMPARAMETERIVEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLGETMINMAXPARAMETERFVEXTPROC
    typedef void (APIENTRY * PFNGLGETMINMAXPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat *params);
    #endif
    std::cout << "Successfully defined PFNGLGETMINMAXPARAMETERFVEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLGETMINMAXPARAMETERFVEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLGETMINMAXPARAMETERFVEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLGETMINMAXPARAMETERIVEXTPROC
    typedef void (APIENTRY * PFNGLGETMINMAXPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
    #endif
    std::cout << "Successfully defined PFNGLGETMINMAXPARAMETERIVEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLGETMINMAXPARAMETERIVEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLGETMINMAXPARAMETERIVEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLHISTOGRAMEXTPROC
    typedef void (APIENTRY * PFNGLHISTOGRAMEXTPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
    #endif
    std::cout << "Successfully defined PFNGLHISTOGRAMEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLHISTOGRAMEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLHISTOGRAMEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLMINMAXEXTPROC
    typedef void (APIENTRY * PFNGLMINMAXEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink);
    #endif
    std::cout << "Successfully defined PFNGLMINMAXEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLMINMAXEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLMINMAXEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLRESETHISTOGRAMEXTPROC
    typedef void (APIENTRY * PFNGLRESETHISTOGRAMEXTPROC) (GLenum target);
    #endif
    std::cout << "Successfully defined PFNGLRESETHISTOGRAMEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLRESETHISTOGRAMEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLRESETHISTOGRAMEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLRESETMINMAXEXTPROC
    typedef void (APIENTRY * PFNGLRESETMINMAXEXTPROC) (GLenum target);
    #endif
    std::cout << "Successfully defined PFNGLRESETMINMAXEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLRESETMINMAXEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLRESETMINMAXEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLSEPARABLEFILTER2DEXTPROC
    typedef void (APIENTRY * PFNGLSEPARABLEFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *row, const void *column);
    #endif
    std::cout << "Successfully defined PFNGLSEPARABLEFILTER2DEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLSEPARABLEFILTER2DEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLSEPARABLEFILTER2DEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLGETMINMAXEXTPROC
    typedef void (APIENTRY * PFNGLGETMINMAXEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink, GLvoid *values);
    #endif
    std::cout << "Successfully defined PFNGLGETMINMAXEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLGETMINMAXEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLGETMINMAXEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLGETHISTOGRAMEXTPROC
    typedef void (APIENTRY * PFNGLGETHISTOGRAMEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
    #endif
    std::cout << "Successfully defined PFNGLGETHISTOGRAMEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLGETHISTOGRAMEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLGETHISTOGRAMEXTPROC", "gl_definitions.h", "Define Types", -1);
}

try {
    #ifndef PFNGLCOLORTABLEEXTPROC
    typedef void (APIENTRY * PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
    #endif
    std::cout << "Successfully defined PFNGLCOLORTABLEEXTPROC" << std::endl;
} catch (...) {
    std::cout << "Error defining PFNGLCOLORTABLEEXTPROC" << std::endl;
    Debugger::GetInstance().LogError("Error defining PFNGLCOLORTABLEEXTPROC", "gl_definitions.h", "Define Types", -1);
}

#else
// Windows-specific DirectX headers are included in DirectXGraphicsAPI.h
#endif

#endif // GAME_ENGINE_SAVI_GL_DEFINITIONS_H
