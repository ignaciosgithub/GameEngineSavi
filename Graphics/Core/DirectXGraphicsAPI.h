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
    
    // Texture management
    unsigned int CreateTexture() override;
    void BindTexture(unsigned int texture, unsigned int unit) override;
    void DeleteTexture(unsigned int texture) override;
    void TexImage2D(int width, int height, const void* data, bool hasAlpha) override;
    
    // Debug utilities
    void DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) override;
    void DrawDebugAxes() override;
    
    // Get API name for debugging
    const char* GetAPIName() const override { return "DirectX"; }
    
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
    
    // Helper methods
    void CreateDeviceAndSwapChain(HWND hwnd);
    void CreateRenderTargetView();
    void CreateDepthStencilView(int width, int height);
#endif
    
    // Tracking state
    unsigned int currentVAO;
    unsigned int currentBuffer;
};

#endif // DIRECTX_GRAPHICS_API_H
