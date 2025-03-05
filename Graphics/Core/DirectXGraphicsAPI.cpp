#include "DirectXGraphicsAPI.h"
#include "../../Shaders/Core/ShaderProgram.h"
#include <iostream>

DirectXGraphicsAPI::DirectXGraphicsAPI() 
    : currentVAO(0), currentBuffer(0) {
#ifdef PLATFORM_WINDOWS
    device = nullptr;
    context = nullptr;
    swapChain = nullptr;
    renderTargetView = nullptr;
    depthStencilView = nullptr;
    currentVertexBuffer = nullptr;
    currentIndexBuffer = nullptr;
#endif
}

DirectXGraphicsAPI::~DirectXGraphicsAPI() {
    Shutdown();
}

bool DirectXGraphicsAPI::Initialize() {
#ifdef PLATFORM_WINDOWS
    // DirectX initialization would go here
    // For now, just return true as a placeholder
    std::cout << "Initializing DirectX graphics API..." << std::endl;
    
    // Note: In a real implementation, we would initialize DirectX here
    // However, we need a window handle (HWND) to create the device and swap chain
    // This would typically be passed in from the application
    
    return true;
#else
    std::cerr << "DirectX is only supported on Windows" << std::endl;
    return false;
#endif
}

void DirectXGraphicsAPI::Shutdown() {
#ifdef PLATFORM_WINDOWS
    // Release DirectX resources
    if (renderTargetView) renderTargetView->Release();
    if (depthStencilView) depthStencilView->Release();
    if (swapChain) swapChain->Release();
    if (context) context->Release();
    if (device) device->Release();
    
    renderTargetView = nullptr;
    depthStencilView = nullptr;
    swapChain = nullptr;
    context = nullptr;
    device = nullptr;
#endif
}

unsigned int DirectXGraphicsAPI::CreateVertexArray() {
    // DirectX doesn't have VAOs, so we'll use a simple counter
    static unsigned int nextVAO = 1;
    return nextVAO++;
}

void DirectXGraphicsAPI::BindVertexArray(unsigned int vao) {
    currentVAO = vao;
    // No actual binding in DirectX
}

void DirectXGraphicsAPI::DeleteVertexArray(unsigned int vao) {
    if (currentVAO == vao) {
        currentVAO = 0;
    }
    // No actual deletion in DirectX
}

unsigned int DirectXGraphicsAPI::CreateBuffer() {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would create a D3D buffer here
    // For now, just return a placeholder ID
    static unsigned int nextBuffer = 1;
    return nextBuffer++;
#else
    return 0;
#endif
}

void DirectXGraphicsAPI::BindBuffer(BufferType type, unsigned int buffer) {
    currentBuffer = buffer;
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would bind the buffer here
#endif
}

void DirectXGraphicsAPI::DeleteBuffer(unsigned int buffer) {
    if (currentBuffer == buffer) {
        currentBuffer = 0;
    }
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would release the buffer here
#endif
}

void DirectXGraphicsAPI::BufferData(BufferType type, const void* data, size_t size, bool dynamic) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would update the buffer data here
#endif
}

void DirectXGraphicsAPI::EnableVertexAttrib(unsigned int index) {
#ifdef PLATFORM_WINDOWS
    // DirectX handles this differently through input layouts
#endif
}

void DirectXGraphicsAPI::DisableVertexAttrib(unsigned int index) {
#ifdef PLATFORM_WINDOWS
    // DirectX handles this differently through input layouts
#endif
}

void DirectXGraphicsAPI::VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) {
#ifdef PLATFORM_WINDOWS
    // DirectX handles this differently through input layouts
#endif
}

void DirectXGraphicsAPI::DrawArrays(DrawMode mode, int first, int count) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would call Draw here
    if (context) {
        context->Draw(count, first);
    }
#endif
}

void DirectXGraphicsAPI::DrawElements(DrawMode mode, int count, const void* indices) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would call DrawIndexed here
    if (context) {
        context->DrawIndexed(count, 0, 0);
    }
#endif
}

void DirectXGraphicsAPI::SetViewport(int x, int y, int width, int height) {
#ifdef PLATFORM_WINDOWS
    if (context) {
        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = static_cast<float>(x);
        viewport.TopLeftY = static_cast<float>(y);
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        context->RSSetViewports(1, &viewport);
    }
#endif
}

void DirectXGraphicsAPI::Clear(bool colorBuffer, bool depthBuffer) {
#ifdef PLATFORM_WINDOWS
    if (context) {
        // Clear render target
        if (colorBuffer && renderTargetView) {
            float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Black
            context->ClearRenderTargetView(renderTargetView, clearColor);
        }
        
        // Clear depth buffer
        if (depthBuffer && depthStencilView) {
            context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
        }
    }
#endif
}

void DirectXGraphicsAPI::SetClearColor(float r, float g, float b, float a) {
    // Store clear color for later use in Clear()
#ifdef PLATFORM_WINDOWS
    // DirectX sets the clear color during the clear operation
#endif
}

void DirectXGraphicsAPI::UseShaderProgram(ShaderProgram* program) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would set the shader program here
    if (program && context) {
        // Get DirectX shader resources from the program
        // In a real implementation, we would get the vertex shader, pixel shader, and input layout from the program
        // ID3D11VertexShader* vertexShader = program->GetDirectXVertexShader();
        // ID3D11PixelShader* pixelShader = program->GetDirectXPixelShader();
        // ID3D11InputLayout* inputLayout = program->GetDirectXInputLayout();
        
        // Set the shaders and input layout
        // if (vertexShader && pixelShader && inputLayout) {
        //     context->VSSetShader(vertexShader, nullptr, 0);
        //     context->PSSetShader(pixelShader, nullptr, 0);
        //     context->IASetInputLayout(inputLayout);
        // } else {
        //     std::cerr << "Warning: Invalid DirectX shader program resources" << std::endl;
        // }
        
        // For now, just log the shader program usage
        std::cout << "Using shader program: " << program << std::endl;
    }
#endif
}

unsigned int DirectXGraphicsAPI::CreateTexture() {
#ifdef PLATFORM_WINDOWS
    // Create a DirectX texture
    if (device) {
        // Create a texture ID for tracking
        static unsigned int nextTexture = 1;
        unsigned int textureID = nextTexture++;
        
        // Create DirectX texture resource
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = 1; // Default size, will be updated in TexImage2D
        textureDesc.Height = 1; // Default size, will be updated in TexImage2D
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;
        
        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
        
        if (SUCCEEDED(hr)) {
            // Store the texture in a map for later use
            // In a real implementation, we would store this in a map
            // textures[textureID] = texture;
            std::cout << "Created DirectX texture with ID: " << textureID << std::endl;
            return textureID;
        } else {
            std::cerr << "Error: Failed to create DirectX texture" << std::endl;
        }
    }
    return 0;
#else
    return 0;
#endif
}

void DirectXGraphicsAPI::BindTexture(unsigned int texture, unsigned int unit) {
#ifdef PLATFORM_WINDOWS
    // Bind the texture to a shader resource view
    if (context) {
        // In a real implementation, we would get the texture from a map
        // ID3D11Texture2D* textureResource = textures[texture];
        // ID3D11ShaderResourceView* srv = nullptr;
        
        // if (textureResource) {
        //     // Create shader resource view if not already created
        //     device->CreateShaderResourceView(textureResource, nullptr, &srv);
        //     
        //     // Bind to pixel shader
        //     context->PSSetShaderResources(unit, 1, &srv);
        // }
        
        // For now, just log the binding
        std::cout << "Binding texture " << texture << " to unit " << unit << std::endl;
    }
#endif
}

void DirectXGraphicsAPI::DeleteTexture(unsigned int texture) {
#ifdef PLATFORM_WINDOWS
    // Release the DirectX texture resource
    // In a real implementation, we would get the texture from a map and release it
    // ID3D11Texture2D* textureResource = textures[texture];
    // if (textureResource) {
    //     textureResource->Release();
    //     textures.erase(texture);
    // }
    
    // For now, just log the deletion
    std::cout << "Deleting texture " << texture << std::endl;
#endif
}

void DirectXGraphicsAPI::TexImage2D(int width, int height, const void* data, bool hasAlpha) {
#ifdef PLATFORM_WINDOWS
    // Update the texture data
    if (device && context) {
        // In a real implementation, we would get the current bound texture
        // ID3D11Texture2D* textureResource = textures[currentTexture];
        
        // if (textureResource) {
        //     // Update texture description with new dimensions
        //     D3D11_TEXTURE2D_DESC desc;
        //     textureResource->GetDesc(&desc);
        //     
        //     // If dimensions changed, recreate the texture
        //     if (desc.Width != width || desc.Height != height) {
        //         textureResource->Release();
        //         
        //         desc.Width = width;
        //         desc.Height = height;
        //         desc.Format = hasAlpha ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
        //         
        //         device->CreateTexture2D(&desc, nullptr, &textureResource);
        //         textures[currentTexture] = textureResource;
        //     }
        //     
        //     // Update texture data
        //     if (data) {
        //         D3D11_BOX box = {};
        //         box.right = width;
        //         box.bottom = height;
        //         box.back = 1;
        //         
        //         context->UpdateSubresource(textureResource, 0, &box, data, 
        //                                   width * (hasAlpha ? 4 : 3), 0);
        //     }
        // }
        
        // For now, just log the texture update
        std::cout << "Updating texture with dimensions " << width << "x" << height 
                  << " (has alpha: " << (hasAlpha ? "yes" : "no") << ")" << std::endl;
    }
#endif
}

void DirectXGraphicsAPI::DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) {
#ifdef PLATFORM_WINDOWS
    // Draw a debug line using DirectX
    if (context) {
        // In a real implementation, we would:
        // 1. Create a vertex buffer with the line vertices
        // 2. Set up a simple shader for drawing lines
        // 3. Draw the line
        
        // For now, just log the line drawing
        std::cout << "Drawing debug line from (" 
                  << start.x << ", " << start.y << ", " << start.z << ") to ("
                  << end.x << ", " << end.y << ", " << end.z << ") with color ("
                  << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
    }
#endif
}

void DirectXGraphicsAPI::DrawDebugAxes() {
#ifdef PLATFORM_WINDOWS
    // Draw debug axes using DirectX
    if (context) {
        // X axis (red)
        DrawDebugLine(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(1, 0, 0));
        
        // Y axis (green)
        DrawDebugLine(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 1, 0));
        
        // Z axis (blue)
        DrawDebugLine(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 0, 1));
        
        std::cout << "Drawing debug axes" << std::endl;
    }
#endif
}

#ifdef PLATFORM_WINDOWS
void DirectXGraphicsAPI::CreateDeviceAndSwapChain(HWND hwnd) {
    // Create the DirectX device and swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = 0; // Use window width
    swapChainDesc.BufferDesc.Height = 0; // Use window height
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    
    // Create device, context, and swap chain
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    UINT createDeviceFlags = 0;
    
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc,
        &swapChain, &device, nullptr, &context);
    
    if (FAILED(hr)) {
        std::cerr << "Error: Failed to create DirectX device and swap chain" << std::endl;
    } else {
        std::cout << "Successfully created DirectX device and swap chain" << std::endl;
    }
}

void DirectXGraphicsAPI::CreateRenderTargetView() {
    // Create the render target view
    if (device && swapChain) {
        // Get the back buffer
        ID3D11Texture2D* backBuffer = nullptr;
        HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        
        if (SUCCEEDED(hr)) {
            // Create the render target view
            hr = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
            backBuffer->Release();
            
            if (FAILED(hr)) {
                std::cerr << "Error: Failed to create render target view" << std::endl;
            } else {
                std::cout << "Successfully created render target view" << std::endl;
            }
        } else {
            std::cerr << "Error: Failed to get back buffer" << std::endl;
        }
    }
}

void DirectXGraphicsAPI::CreateDepthStencilView(int width, int height) {
    // Create the depth stencil view
    if (device) {
        // Create depth stencil texture
        D3D11_TEXTURE2D_DESC depthStencilDesc = {};
        depthStencilDesc.Width = width;
        depthStencilDesc.Height = height;
        depthStencilDesc.MipLevels = 1;
        depthStencilDesc.ArraySize = 1;
        depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
        depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilDesc.CPUAccessFlags = 0;
        depthStencilDesc.MiscFlags = 0;
        
        ID3D11Texture2D* depthStencilTexture = nullptr;
        HRESULT hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilTexture);
        
        if (SUCCEEDED(hr)) {
            // Create depth stencil view
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
            depthStencilViewDesc.Format = depthStencilDesc.Format;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0;
            
            hr = device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView);
            depthStencilTexture->Release();
            
            if (FAILED(hr)) {
                std::cerr << "Error: Failed to create depth stencil view" << std::endl;
            } else {
                std::cout << "Successfully created depth stencil view" << std::endl;
            }
        } else {
            std::cerr << "Error: Failed to create depth stencil texture" << std::endl;
        }
    }
}
#endif
