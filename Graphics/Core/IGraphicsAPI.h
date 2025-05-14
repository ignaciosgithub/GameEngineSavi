#ifndef IGRAPHICS_API_H
#define IGRAPHICS_API_H

#include <vector>
#include <string>
#include "../../Vector3.h"
#include "../../Vector2.h"
#include "../../Matrix4x4.h"

// Forward declarations
class ShaderProgram;
class Texture;

// Buffer types
enum class BufferType {
    VERTEX_BUFFER,
    INDEX_BUFFER,
    TEXTURE_COORD_BUFFER,
    NORMAL_BUFFER
};

// Draw modes
enum class DrawMode {
    TRIANGLES,
    LINES,
    POINTS
};

// Graphics API interface
class IGraphicsAPI {
public:
    virtual ~IGraphicsAPI() = default;
    
    // Initialization and cleanup
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    
    // Buffer management
    virtual unsigned int CreateVertexArray() = 0;
    virtual void BindVertexArray(unsigned int vao) = 0;
    virtual void DeleteVertexArray(unsigned int vao) = 0;
    
    virtual unsigned int CreateBuffer() = 0;
    virtual void BindBuffer(BufferType type, unsigned int buffer) = 0;
    virtual void DeleteBuffer(unsigned int buffer) = 0;
    virtual void BufferData(BufferType type, const void* data, size_t size, bool dynamic = false) = 0;
    
    // Attribute configuration
    virtual void EnableVertexAttrib(unsigned int index) = 0;
    virtual void DisableVertexAttrib(unsigned int index) = 0;
    virtual void VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) = 0;
    
    // Drawing
    virtual void DrawArrays(DrawMode mode, int first, int count) = 0;
    virtual void DrawElements(DrawMode mode, int count, const void* indices) = 0;
    
    // Viewport and clear
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void Begin2D() = 0; // Set up 2D GUI rendering
    virtual void End2D() = 0;   // Restore 3D rendering state
    virtual void Clear(bool colorBuffer, bool depthBuffer) = 0;
    virtual void SetClearColor(float r, float g, float b, float a) = 0;
    
    // Depth and culling
    virtual void SetDepthTest(bool enable) = 0;
    virtual void SetDepthFunc(int func) = 0;
    virtual void SetCullFace(bool enable) = 0;
    virtual void SetCullFaceMode(int mode) = 0;
    
    // Shader management
    virtual void UseShaderProgram(ShaderProgram* program) = 0;
    virtual unsigned int CreateShader(int shaderType) = 0;
    virtual void DeleteShader(unsigned int shader) = 0;
    virtual void ShaderSource(unsigned int shader, const std::string& source) = 0;
    virtual void CompileShader(unsigned int shader) = 0;
    virtual bool GetShaderCompileStatus(unsigned int shader) = 0;
    virtual std::string GetShaderInfoLog(unsigned int shader) = 0;
    virtual bool GetProgramLinkStatus(unsigned int program) = 0;
    virtual std::string GetProgramInfoLog(unsigned int program) = 0;
    virtual void AttachShader(unsigned int program, unsigned int shader) = 0;
    virtual void LinkProgram(unsigned int program) = 0;
    virtual unsigned int CreateProgram() = 0;
    virtual void DeleteProgram(unsigned int program) = 0;
    
    // Uniform setters
    virtual void SetUniform1f(unsigned int program, const std::string& name, float value) = 0;
    virtual void SetUniform1i(unsigned int program, const std::string& name, int value) = 0;
    virtual void SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) = 0;
    virtual void SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) = 0;
    virtual void SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose = false) = 0;
    
    // Additional uniform setters for arrays
    virtual void SetUniformFloatArray(unsigned int program, const std::string& name, const float* values, int count) = 0;
    virtual void SetUniformIntArray(unsigned int program, const std::string& name, const int* values, int count) = 0;
    virtual void SetUniformVec3Array(unsigned int program, const std::string& name, const float* values, int count) = 0;
    virtual void SetUniformMatrix4Array(unsigned int program, const std::string& name, const float* values, int count, bool transpose = false) = 0;
    virtual int GetUniformLocation(unsigned int program, const std::string& name) = 0;
    
    // Texture management
    virtual unsigned int CreateTexture() = 0;
    virtual void BindTexture(unsigned int texture, unsigned int unit) = 0;
    virtual void DeleteTexture(unsigned int texture) = 0;
    virtual void TexImage2D(int width, int height, const void* data, bool hasAlpha) = 0;
    
    // Debug utilities
    virtual void DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) = 0;
    virtual void DrawDebugAxes() = 0;
    
    // Framebuffer operations
    virtual void ReadPixels(int x, int y, int width, int height, unsigned char* pixels) = 0;
    virtual void GetViewport(int* viewport) = 0;
    
    // Window management
    virtual bool CreateWindow(int width, int height, const char* title) = 0;
    virtual void DestroyWindow() = 0;
    virtual void MakeContextCurrent() = 0;
    virtual bool IsWindowOpen() = 0;
    virtual void PollEvents() = 0;
    
    // Platform-specific operations
    virtual void SwapBuffers() = 0;
    
    // Get API name for debugging
    virtual const char* GetAPIName() const = 0;
    
    // Default shader for models with missing shaders/textures
    virtual void UseDefaultRedShader() = 0;
    
    // Matrix operations
    virtual void SetProjectionMatrix(const Matrix4x4& matrix) = 0;
    virtual void SetViewMatrix(const Matrix4x4& matrix) = 0;
    virtual void SetModelMatrix(const Matrix4x4& matrix) = 0;
    virtual bool SupportsMatrixOperations() const = 0;
    
    // Input handling
    virtual void GetMousePosition(int& x, int& y) = 0;
    virtual bool IsMouseButtonPressed(int button) = 0;
};

#endif // IGRAPHICS_API_H
