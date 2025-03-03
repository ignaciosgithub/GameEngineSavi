#pragma once

#include "gl_types.h"

#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <gl/gl.h>
    #include <gl/glu.h>
#else
    // Include system OpenGL headers if available
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glext.h>
#endif

// OpenGL function declarations
// These are defined here to ensure they're available across the codebase
#ifndef GL_VERSION_2_0
    // Define GL_VERSION_2_0 constants if not already defined
    #ifndef GL_FRAGMENT_SHADER
        #define GL_FRAGMENT_SHADER 0x8B30
    #endif
    #ifndef GL_VERTEX_SHADER
        #define GL_VERTEX_SHADER 0x8B31
    #endif
    #ifndef GL_COMPILE_STATUS
        #define GL_COMPILE_STATUS 0x8B81
    #endif
    #ifndef GL_LINK_STATUS
        #define GL_LINK_STATUS 0x8B82
    #endif
    #ifndef GL_INFO_LOG_LENGTH
        #define GL_INFO_LOG_LENGTH 0x8B84
    #endif
#endif

// Ensure these OpenGL functions are available
// We're using direct function declarations rather than function pointers
// to simplify the implementation
#ifdef __cplusplus
extern "C" {
#endif

// These declarations will only be used if the system headers don't provide them
#ifndef GL_VERSION_2_0
    // Program functions
    GLuint glCreateProgram(void);
    void glDeleteProgram(GLuint program);
    void glUseProgram(GLuint program);
    void glAttachShader(GLuint program, GLuint shader);
    void glLinkProgram(GLuint program);
    void glGetProgramiv(GLuint program, GLenum pname, GLint *params);
    void glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

    // Shader functions
    GLuint glCreateShader(GLenum shaderType);
    void glDeleteShader(GLuint shader);
    void glShaderSource(GLuint shader, GLsizei count, const GLchar* const* string, const GLint *length);
    void glCompileShader(GLuint shader);
    void glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
    void glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);

    // Uniform functions
    GLint glGetUniformLocation(GLuint program, const GLchar *name);
    void glUniform1f(GLint location, GLfloat v0);
    void glUniform1i(GLint location, GLint v0);
    void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
    void glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
    void glUniform1fv(GLint location, GLsizei count, const GLfloat *value);
    void glUniform1iv(GLint location, GLsizei count, const GLint *value);
    void glUniform3fv(GLint location, GLsizei count, const GLfloat *value);

    // Texture functions
    void glActiveTexture(GLenum texture);
#endif

// These are core OpenGL functions that should be available in all versions
void glBindTexture(GLenum target, GLuint texture);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glGetIntegerv(GLenum pname, GLint *params);
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
GLenum glGetError(void);

#ifdef __cplusplus
}
#endif
