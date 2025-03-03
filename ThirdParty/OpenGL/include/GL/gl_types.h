#pragma once

// OpenGL type definitions
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

// OpenGL constants
#define GL_FALSE                          0
#define GL_TRUE                           1

// Shader constants
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84

// Texture constants
#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE_2D                     0x0DE1

// Viewport constants
#define GL_VIEWPORT                       0x0BA2

// Pixel format constants
#define GL_RGBA                           0x1908
#define GL_UNSIGNED_BYTE                  0x1401

// Error constants
#define GL_NO_ERROR                       0

// Matrix constants
#define GL_FALSE                          0
#define GL_TRUE                           1
