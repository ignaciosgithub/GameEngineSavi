#include "OpenGLGraphicsAPI.h"
#include "../../Shaders/Core/ShaderProgram.h"
#include <iostream>

#ifndef PLATFORM_WINDOWS

OpenGLGraphicsAPI::OpenGLGraphicsAPI() 
    : currentVAO(0), currentBuffer(0), currentShader(0) {
}

OpenGLGraphicsAPI::~OpenGLGraphicsAPI() {
    Shutdown();
}

bool OpenGLGraphicsAPI::Initialize() {
    // Initialize OpenGL (GLEW is already initialized in gl_definitions.h)
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    return true;
}

void OpenGLGraphicsAPI::Shutdown() {
    // Nothing to do here for OpenGL
}

unsigned int OpenGLGraphicsAPI::CreateVertexArray() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void OpenGLGraphicsAPI::BindVertexArray(unsigned int vao) {
    if (currentVAO != vao) {
        glBindVertexArray(vao);
        currentVAO = vao;
    }
}

void OpenGLGraphicsAPI::DeleteVertexArray(unsigned int vao) {
    if (currentVAO == vao) {
        currentVAO = 0;
    }
    glDeleteVertexArrays(1, &vao);
}

unsigned int OpenGLGraphicsAPI::CreateBuffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void OpenGLGraphicsAPI::BindBuffer(BufferType type, unsigned int buffer) {
    GLenum glType = ConvertBufferType(type);
    glBindBuffer(glType, buffer);
    currentBuffer = buffer;
}

void OpenGLGraphicsAPI::DeleteBuffer(unsigned int buffer) {
    if (currentBuffer == buffer) {
        currentBuffer = 0;
    }
    glDeleteBuffers(1, &buffer);
}

void OpenGLGraphicsAPI::BufferData(BufferType type, const void* data, size_t size, bool dynamic) {
    GLenum glType = ConvertBufferType(type);
    GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    glBufferData(glType, size, data, usage);
}

void OpenGLGraphicsAPI::EnableVertexAttrib(unsigned int index) {
    glEnableVertexAttribArray(index);
}

void OpenGLGraphicsAPI::DisableVertexAttrib(unsigned int index) {
    glDisableVertexAttribArray(index);
}

void OpenGLGraphicsAPI::VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) {
    glVertexAttribPointer(index, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, stride, pointer);
}

void OpenGLGraphicsAPI::DrawArrays(DrawMode mode, int first, int count) {
    GLenum glMode = ConvertDrawMode(mode);
    glDrawArrays(glMode, first, count);
}

void OpenGLGraphicsAPI::DrawElements(DrawMode mode, int count, const void* indices) {
    GLenum glMode = ConvertDrawMode(mode);
    glDrawElements(glMode, count, GL_UNSIGNED_INT, indices);
}

void OpenGLGraphicsAPI::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void OpenGLGraphicsAPI::Clear(bool colorBuffer, bool depthBuffer) {
    GLbitfield mask = 0;
    if (colorBuffer) mask |= GL_COLOR_BUFFER_BIT;
    if (depthBuffer) mask |= GL_DEPTH_BUFFER_BIT;
    glClear(mask);
}

void OpenGLGraphicsAPI::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLGraphicsAPI::UseShaderProgram(ShaderProgram* program) {
    if (program) {
        program->Use();
        currentShader = program->GetHandle();
    } else {
        glUseProgram(0);
        currentShader = 0;
    }
}

unsigned int OpenGLGraphicsAPI::CreateTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
}

void OpenGLGraphicsAPI::BindTexture(unsigned int texture, unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void OpenGLGraphicsAPI::DeleteTexture(unsigned int texture) {
    glDeleteTextures(1, &texture);
}

void OpenGLGraphicsAPI::TexImage2D(int width, int height, const void* data, bool hasAlpha) {
    GLint format = hasAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void OpenGLGraphicsAPI::DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) {
    glBegin(GL_LINES);
    glColor3f(color.x, color.y, color.z);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}

void OpenGLGraphicsAPI::DrawDebugAxes() {
    glBegin(GL_LINES);
    
    // X axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    
    // Y axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    
    // Z axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    
    glEnd();
}

GLenum OpenGLGraphicsAPI::ConvertBufferType(BufferType type) const {
    switch (type) {
        case BufferType::VERTEX_BUFFER: return GL_ARRAY_BUFFER;
        case BufferType::INDEX_BUFFER: return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::TEXTURE_COORD_BUFFER: return GL_ARRAY_BUFFER;
        case BufferType::NORMAL_BUFFER: return GL_ARRAY_BUFFER;
        default: return GL_ARRAY_BUFFER;
    }
}

GLenum OpenGLGraphicsAPI::ConvertDrawMode(DrawMode mode) const {
    switch (mode) {
        case DrawMode::TRIANGLES: return GL_TRIANGLES;
        case DrawMode::LINES: return GL_LINES;
        case DrawMode::POINTS: return GL_POINTS;
        default: return GL_TRIANGLES;
    }
}

#endif // !PLATFORM_WINDOWS
