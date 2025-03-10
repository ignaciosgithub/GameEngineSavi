#ifndef DIRECTX_GRAPHICS_API_H
#define DIRECTX_GRAPHICS_API_H

#include "IGraphicsAPI.h"
#include "../../platform.h"

#ifdef PLATFORM_WINDOWS
#include <d3d11.h>
#include <directxmath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#endif

class DirectXGraphicsAPI : public IGraphicsAPI {
public:
    DirectXGraphicsAPI();
    ~DirectXGraphicsAPI() override;
    
    // Initialization and cleanup
    bool Initialize() override;
    void Shutdown() override;
    
    // Buffer management
    unsigned int CreateVertexArray() override;
    void BindVertexArray(unsigned int vao) override;
    void DeleteVertexArray(unsigned int vao) override;
    
    unsigned int CreateBuffer() override;
    void BindBuffer(BufferType type, unsigned int buffer) override;
    void DeleteBuffer(unsigned int buffer) override;
    void BufferData(BufferType type, const void* data, size_t size, bool dynamic = false) override;
    
    // Attribute configuration
    void EnableVertexAttrib(unsigned int index) override;
    void DisableVertexAttrib(unsigned int index) override;
    void VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) override;
    
    // Drawing
    void DrawArrays(DrawMode mode, int first, int count) override;
    void DrawElements(DrawMode mode, int count, const void* indices) override;
    
    // Viewport and clear
    void SetViewport(int x, int y, int width, int height) override;
    void Clear(bool colorBuffer, bool depthBuffer) override;
    void SetClearColor(float r, float g, float b, float a) override;
    
    // Shader management
    void UseShaderProgram(ShaderProgram* program) override;
    unsigned int CreateShader(int shaderType) override;
    void DeleteShader(unsigned int shader) override;
    void ShaderSource(unsigned int shader, const std::string& source) override;
    void CompileShader(unsigned int shader) override;
    bool GetShaderCompileStatus(unsigned int shader) override;
    std::string GetShaderInfoLog(unsigned int shader) override;
    bool GetProgramLinkStatus(unsigned int program) override;
    std::string GetProgramInfoLog(unsigned int program) override;
    void AttachShader(unsigned int program, unsigned int shader) override;
    void LinkProgram(unsigned int program) override;
    unsigned int CreateProgram() override;
    void DeleteProgram(unsigned int program) override;
    
    // Uniform setters
    void SetUniform1f(unsigned int program, const std::string& name, float value) override;
    void SetUniform1i(unsigned int program, const std::string& name, int value) override;
    void SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) override;
    void SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) override;
    void SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose = false) override;
    
    // Additional uniform setters for arrays
    void SetUniformFloatArray(unsigned int program, const std::string& name, const float* values, int count) override;
    void SetUniformIntArray(unsigned int program, const std::string& name, const int* values, int count) override;
    void SetUniformVec3Array(unsigned int program, const std::string& name, const float* values, int count) override;
    void SetUniformMatrix4Array(unsigned int program, const std::string& name, const float* values, int count, bool transpose = false) override;
    int GetUniformLocation(unsigned int program, const std::string& name) override;
    
    // Texture management
    unsigned int CreateTexture() override;
    void BindTexture(unsigned int texture, unsigned int unit) override;
    void DeleteTexture(unsigned int texture) override;
    void TexImage2D(int width, int height, const void* data, bool hasAlpha) override;
    
    // Debug utilities
    void DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) override;
    void DrawDebugAxes() override;
    
    // Framebuffer operations
    void ReadPixels(int x, int y, int width, int height, unsigned char* pixels) override;
    void GetViewport(int* viewport) override;
    
    // Window management
    bool CreateWindow(int width, int height, const char* title) override;
    void DestroyWindow() override;
    void MakeContextCurrent() override;
    bool IsWindowOpen() override;
    void PollEvents() override;
    
    // Platform-specific operations
    void SwapBuffers() override;
    
    // Get API name for debugging
    const char* GetAPIName() const override { return "DirectX"; }
    
    // Default shader for models with missing shaders/textures
    void UseDefaultRedShader() override;
    
    // Matrix operations
    void SetProjectionMatrix(const Matrix4x4& matrix) override;
    void SetViewMatrix(const Matrix4x4& matrix) override;
    void SetModelMatrix(const Matrix4x4& matrix) override;
    bool SupportsMatrixOperations() const override { return true; }
    
private:
#ifdef PLATFORM_WINDOWS
    // DirectX device and context
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11DepthStencilView* depthStencilView;
    
    // DirectX state tracking
    ID3D11Buffer* currentVertexBuffer;
    ID3D11Buffer* currentIndexBuffer;
    
    // Window management
    HWND hWnd;
    
    // Helper methods
    void CreateDeviceAndSwapChain(HWND hwnd);
    void CreateRenderTargetView();
    void CreateDepthStencilView(int width, int height);
#endif

    
    // Tracking state
    unsigned int currentVAO;
    unsigned int currentBuffer;
    bool windowOpen;
};

#endif // DIRECTX_GRAPHICS_API_H
