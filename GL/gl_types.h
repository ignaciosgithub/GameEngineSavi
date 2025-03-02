#ifndef GL_TYPES_H
#define GL_TYPES_H

// Define OpenGL types for compatibility
typedef unsigned int GLuint;
typedef float GLfloat;
typedef unsigned int GLenum;
typedef int GLint;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef short GLshort;
typedef double GLdouble;
typedef unsigned int GLbitfield;
typedef int GLsizei;
typedef void GLvoid;

// Define some common OpenGL constants
#define GL_FALSE 0
#define GL_TRUE 1

// Texture constants
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7

// Shader types
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9

// Buffer types
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893

// Draw modes
#define GL_TRIANGLES 0x0004
#define GL_LINES 0x0001
#define GL_POINTS 0x0000

// Data types
#define GL_FLOAT 0x1406
#define GL_UNSIGNED_INT 0x1405
#define GL_INT 0x1404

#endif // GL_TYPES_H
