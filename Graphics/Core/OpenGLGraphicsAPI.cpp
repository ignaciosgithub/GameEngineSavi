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

unsigned int OpenGLGraphicsAPI::CreateShader(int shaderType) {
    return glCreateShader(shaderType);
}

void OpenGLGraphicsAPI::DeleteShader(unsigned int shader) {
    glDeleteShader(shader);
}

void OpenGLGraphicsAPI::ShaderSource(unsigned int shader, const std::string& source) {
    const GLchar* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, NULL);
}

void OpenGLGraphicsAPI::CompileShader(unsigned int shader) {
    glCompileShader(shader);
}

bool OpenGLGraphicsAPI::GetShaderCompileStatus(unsigned int shader) {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success == GL_TRUE;
}

std::string OpenGLGraphicsAPI::GetShaderInfoLog(unsigned int shader) {
    GLint logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0) {
        GLchar* log = new GLchar[logLength];
        glGetShaderInfoLog(shader, logLength, NULL, log);
        
        std::string errorLog = log;
        delete[] log;
        
        return errorLog;
    }
    
    return "";
}

bool OpenGLGraphicsAPI::GetProgramLinkStatus(unsigned int program) {
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

std::string OpenGLGraphicsAPI::GetProgramInfoLog(unsigned int program) {
    GLint logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength > 0) {
        GLchar* log = new GLchar[logLength];
        glGetProgramInfoLog(program, logLength, NULL, log);
        
        std::string errorLog = log;
        delete[] log;
        
        return errorLog;
    }
    
    return "";
}

void OpenGLGraphicsAPI::AttachShader(unsigned int program, unsigned int shader) {
    glAttachShader(program, shader);
}

void OpenGLGraphicsAPI::LinkProgram(unsigned int program) {
    glLinkProgram(program);
}

unsigned int OpenGLGraphicsAPI::CreateProgram() {
    return glCreateProgram();
}

void OpenGLGraphicsAPI::DeleteProgram(unsigned int program) {
    glDeleteProgram(program);
}

void OpenGLGraphicsAPI::SetUniform1f(unsigned int program, const std::string& name, float value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void OpenGLGraphicsAPI::SetUniform1i(unsigned int program, const std::string& name, int value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void OpenGLGraphicsAPI::SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform3f(location, x, y, z);
    }
}

void OpenGLGraphicsAPI::SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

void OpenGLGraphicsAPI::SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, value);
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

void OpenGLGraphicsAPI::SwapBuffers() {
    // This is a platform-specific operation that needs to be handled by the window system
    // For X11/GLX, this would be glXSwapBuffers(display, window)
    // For GLFW, this would be glfwSwapBuffers(window)
    // Since we don't have direct access to the window system here,
    // this is a placeholder that should be overridden by the platform-specific code
    
    // In EnhancedEmergencyEditor.cpp, we'll still need to call glXSwapBuffers directly
    // but we'll use this method when possible
}

void OpenGLGraphicsAPI::SetDepthTest(bool enable) {
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
}

void OpenGLGraphicsAPI::SetDepthFunc(int func) {
    glDepthFunc(func);
}

void OpenGLGraphicsAPI::SetCullFace(bool enable) {
    if (enable) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
}

void OpenGLGraphicsAPI::SetCullFaceMode(int mode) {
    glCullFace(mode);
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
