#ifndef GAME_ENGINE_SAVI_OPENGL_GRAPHICS_API_H
#define GAME_ENGINE_SAVI_OPENGL_GRAPHICS_API_H

#include "IGraphicsAPI.h"
#include "../../ThirdParty/OpenGL/include/GL/gl_definitions.h"
#include "../../Shaders/Core/ShaderProgram.h"
#include <iostream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
// X11 and GLX includes for Linux
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>
#endif

// OpenGL implementation of the IGraphicsAPI interface
// This is used on Linux platforms
class OpenGLGraphicsAPI : public IGraphicsAPI {
public:
    OpenGLGraphicsAPI();
    virtual ~OpenGLGraphicsAPI();

    // Initialization and cleanup
    virtual bool Initialize() override;
    virtual void Shutdown() override;
    
    // Buffer management
    virtual unsigned int CreateVertexArray() override;
    virtual void BindVertexArray(unsigned int vao) override;
    virtual void DeleteVertexArray(unsigned int vao) override;
    
    virtual unsigned int CreateBuffer() override;
    virtual void BindBuffer(BufferType type, unsigned int buffer) override;
    virtual void DeleteBuffer(unsigned int buffer) override;
    virtual void BufferData(BufferType type, const void* data, size_t size, bool dynamic = false) override;
    
    // Attribute configuration
    virtual void EnableVertexAttrib(unsigned int index) override;
    virtual void DisableVertexAttrib(unsigned int index) override;
    virtual void VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) override;
    
    // Drawing
    virtual void DrawArrays(DrawMode mode, int first, int count) override;
    virtual void DrawElements(DrawMode mode, int count, const void* indices) override;
    
    // Viewport and clear
    virtual void SetViewport(int x, int y, int width, int height) override;
    virtual void Clear(bool colorBuffer, bool depthBuffer) override;
    virtual void SetClearColor(float r, float g, float b, float a) override;
    
    // Depth and culling
    virtual void SetDepthTest(bool enable) override;
    virtual void SetDepthFunc(int func) override;
    virtual void SetCullFace(bool enable) override;
    virtual void SetCullFaceMode(int mode) override;
    
    // Shader management
    virtual void UseShaderProgram(ShaderProgram* program) override;
    virtual unsigned int CreateShader(int shaderType) override;
    virtual void DeleteShader(unsigned int shader) override;
    virtual void ShaderSource(unsigned int shader, const std::string& source) override;
    virtual void CompileShader(unsigned int shader) override;
    virtual bool GetShaderCompileStatus(unsigned int shader) override;
    virtual std::string GetShaderInfoLog(unsigned int shader) override;
    virtual bool GetProgramLinkStatus(unsigned int program) override;
    virtual std::string GetProgramInfoLog(unsigned int program) override;
    virtual void AttachShader(unsigned int program, unsigned int shader) override;
    virtual void LinkProgram(unsigned int program) override;
    virtual unsigned int CreateProgram() override;
    virtual void DeleteProgram(unsigned int program) override;
    
    // Uniform setters
    virtual void SetUniform1f(unsigned int program, const std::string& name, float value) override;
    virtual void SetUniform1i(unsigned int program, const std::string& name, int value) override;
    virtual void SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) override;
    virtual void SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) override;
    virtual void SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose = false) override;
    
    // Additional uniform setters for arrays
    virtual void SetUniformFloatArray(unsigned int program, const std::string& name, const float* values, int count) override;
    virtual void SetUniformIntArray(unsigned int program, const std::string& name, const int* values, int count) override;
    virtual void SetUniformVec3Array(unsigned int program, const std::string& name, const float* values, int count) override;
    virtual void SetUniformMatrix4Array(unsigned int program, const std::string& name, const float* values, int count, bool transpose = false) override;
    virtual int GetUniformLocation(unsigned int program, const std::string& name) override;
    
    // Texture management
    virtual unsigned int CreateTexture() override;
    virtual void BindTexture(unsigned int texture, unsigned int unit) override;
    virtual void DeleteTexture(unsigned int texture) override;
    virtual void TexImage2D(int width, int height, const void* data, bool hasAlpha) override;
    
    // Debug utilities
    virtual void DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) override;
    virtual void DrawDebugAxes() override;
    
    // Framebuffer operations
    virtual void ReadPixels(int x, int y, int width, int height, unsigned char* pixels) override;
    virtual void GetViewport(int* viewport) override;
    
    // Window management
    virtual bool CreateWindow(int width, int height, const char* title) override;
    virtual void DestroyWindow() override;
    virtual void MakeContextCurrent() override;
    virtual bool IsWindowOpen() override;
    virtual void PollEvents() override;
    
    // Platform-specific operations
    virtual void SwapBuffers() override;
    
    // Get API name for debugging
    virtual const char* GetAPIName() const override { return "OpenGL"; }
    
    // Default shader for models with missing shaders/textures
    virtual void UseDefaultRedShader() override;
    
    // Matrix operations
    virtual void SetProjectionMatrix(const Matrix4x4& matrix) override;
    virtual void SetViewMatrix(const Matrix4x4& matrix) override;
    virtual void SetModelMatrix(const Matrix4x4& matrix) override;
    virtual bool SupportsMatrixOperations() const override { return true; }
    
    // Input handling
    virtual void GetMousePosition(int& x, int& y) override;
    virtual bool IsMouseButtonPressed(int button) override;

    // Conversion methods
    GLenum ConvertBufferType(BufferType type);
    GLenum ConvertDrawMode(DrawMode mode);
    GLenum ConvertDataType(int type);
    GLenum ConvertBufferUsage(bool dynamic);
    GLenum ConvertTextureType(unsigned int unit);
    GLenum ConvertTextureParameter(int param);
    GLenum ConvertPixelFormat(bool hasAlpha);

private:
    // Track current state to avoid redundant state changes
    unsigned int currentVAO;
    unsigned int currentBuffer;
    unsigned int currentShader;
    unsigned int currentTexture;
    
    // Window management
    #ifdef PLATFORM_WINDOWS
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
    #else
    Display* display;
    Window window;
    GLXContext context;
    #endif
    bool windowOpen;
};

#endif // GAME_ENGINE_SAVI_OPENGL_GRAPHICS_API_H
