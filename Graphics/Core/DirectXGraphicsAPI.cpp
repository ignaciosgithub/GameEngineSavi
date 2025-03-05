#include "DirectXGraphicsAPI.h"
#include "../../Shaders/Core/ShaderProgram.h"
#include <iostream>
#include <map>
#include <string>

#ifdef PLATFORM_WINDOWS
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#endif

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
    // Create a temporary window for DirectX initialization
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "DirectXInitWindow";
    
    if (!RegisterClassEx(&wc)) {
        std::cerr << "Error: Failed to register window class for DirectX initialization" << std::endl;
        return false;
    }
    
    // Create a temporary window
    HWND hwnd = CreateWindow(
        "DirectXInitWindow", "DirectX Initialization",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, GetModuleHandle(NULL), NULL
    );
    
    if (!hwnd) {
        std::cerr << "Error: Failed to create window for DirectX initialization" << std::endl;
        return false;
    }
    
    // Create DirectX device and swap chain
    CreateDeviceAndSwapChain(hwnd);
    
    // Create render target view
    CreateRenderTargetView();
    
    // Create depth stencil view
    CreateDepthStencilView(800, 600);
    
    // Set up the viewport
    D3D11_VIEWPORT viewport = {};
    viewport.Width = 800;
    viewport.Height = 600;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    
    if (context) {
        context->RSSetViewports(1, &viewport);
    }
    
    // Clean up the temporary window
    DestroyWindow(hwnd);
    UnregisterClass("DirectXInitWindow", GetModuleHandle(NULL));
    
    std::cout << "DirectX initialization complete" << std::endl;
    return (device != nullptr && context != nullptr);
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
    if (program && context) {
        // Get shader resources from the program
        ID3D11VertexShader* vertexShader = nullptr;
        ID3D11PixelShader* pixelShader = nullptr;
        ID3D11InputLayout* inputLayout = nullptr;
        
        // In a real implementation, we would get these from the program
        // For now, we'll use static variables to simulate shader compilation
        static bool shadersInitialized = false;
        static ID3D11VertexShader* defaultVS = nullptr;
        static ID3D11PixelShader* defaultPS = nullptr;
        static ID3D11InputLayout* defaultLayout = nullptr;
        
        if (!shadersInitialized) {
            // Compile default shaders
            const char* vsCode = 
                "float4 main(float4 pos : POSITION) : SV_POSITION { return pos; }";
            
            const char* psCode = 
                "float4 main() : SV_TARGET { return float4(1.0f, 1.0f, 1.0f, 1.0f); }";
            
            // Compile vertex shader
            ID3DBlob* vsBlob = nullptr;
            ID3DBlob* errorBlob = nullptr;
            HRESULT hr = D3DCompile(vsCode, strlen(vsCode), "VS", nullptr, nullptr, 
                                   "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
            
            if (SUCCEEDED(hr)) {
                // Create vertex shader
                device->CreateVertexShader(vsBlob->GetBufferPointer(), 
                                          vsBlob->GetBufferSize(), nullptr, &defaultVS);
                
                // Create input layout
                D3D11_INPUT_ELEMENT_DESC layout[] = {
                    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
                     D3D11_INPUT_PER_VERTEX_DATA, 0 }
                };
                
                device->CreateInputLayout(layout, 1, vsBlob->GetBufferPointer(),
                                         vsBlob->GetBufferSize(), &defaultLayout);
                
                vsBlob->Release();
            }
            
            // Compile pixel shader
            ID3DBlob* psBlob = nullptr;
            hr = D3DCompile(psCode, strlen(psCode), "PS", nullptr, nullptr,
                           "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
            
            if (SUCCEEDED(hr)) {
                // Create pixel shader
                device->CreatePixelShader(psBlob->GetBufferPointer(),
                                         psBlob->GetBufferSize(), nullptr, &defaultPS);
                
                psBlob->Release();
            }
            
            if (errorBlob) {
                errorBlob->Release();
            }
            
            shadersInitialized = true;
        }
        
        // Use default shaders if program doesn't provide its own
        vertexShader = defaultVS;
        pixelShader = defaultPS;
        inputLayout = defaultLayout;
        
        // Set shaders and input layout
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);
        context->IASetInputLayout(inputLayout);
        
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
        // Here we'll create a simple texture resource view and sampler state
        
        // Create a shader resource view if it doesn't exist
        static ID3D11ShaderResourceView* textureView = nullptr;
        static ID3D11SamplerState* samplerState = nullptr;
        
        if (!textureView || !samplerState) {
            // Create a default texture if none exists
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = 1;
            textureDesc.Height = 1;
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1;
            textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;
            
            // Create white texture data
            UINT32 whitePixel = 0xFFFFFFFF;
            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = &whitePixel;
            initData.SysMemPitch = sizeof(UINT32);
            initData.SysMemSlicePitch = 0;
            
            ID3D11Texture2D* defaultTexture = nullptr;
            HRESULT hr = device->CreateTexture2D(&textureDesc, &initData, &defaultTexture);
            
            if (SUCCEEDED(hr)) {
                // Create shader resource view
                D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
                srvDesc.Format = textureDesc.Format;
                srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                srvDesc.Texture2D.MostDetailedMip = 0;
                srvDesc.Texture2D.MipLevels = 1;
                
                hr = device->CreateShaderResourceView(defaultTexture, &srvDesc, &textureView);
                defaultTexture->Release();
                
                if (FAILED(hr)) {
                    std::cerr << "Error: Failed to create shader resource view" << std::endl;
                }
                
                // Create sampler state
                D3D11_SAMPLER_DESC samplerDesc = {};
                samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
                samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
                samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
                samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
                samplerDesc.MinLOD = 0;
                samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
                
                hr = device->CreateSamplerState(&samplerDesc, &samplerState);
                
                if (FAILED(hr)) {
                    std::cerr << "Error: Failed to create sampler state" << std::endl;
                }
            }
        }
        
        // Bind texture and sampler to pixel shader
        if (textureView && samplerState) {
            context->PSSetShaderResources(unit, 1, &textureView);
            context->PSSetSamplers(unit, 1, &samplerState);
        }
        
        std::cout << "Binding texture " << texture << " to unit " << unit << std::endl;
    }
#endif
}

void DirectXGraphicsAPI::DeleteTexture(unsigned int texture) {
#ifdef PLATFORM_WINDOWS
    // Release the DirectX texture resource
    if (device) {
        // In a real implementation, we would get the texture from a map
        // Here we'll just log the deletion
        
        // Find and release texture resources
        // ID3D11Texture2D* textureResource = nullptr;
        // ID3D11ShaderResourceView* textureView = nullptr;
        // ID3D11SamplerState* samplerState = nullptr;
        
        // if (textureResource) {
        //     textureResource->Release();
        // }
        
        // if (textureView) {
        //     textureView->Release();
        // }
        
        // if (samplerState) {
        //     samplerState->Release();
        // }
        
        // Remove from texture map
        // textures.erase(texture);
        
        std::cout << "Deleted texture " << texture << std::endl;
    }
#endif
}

void DirectXGraphicsAPI::TexImage2D(int width, int height, const void* data, bool hasAlpha) {
#ifdef PLATFORM_WINDOWS
    // Update the texture data
    if (device && context) {
        // Create a new texture with the specified dimensions
        D3D11_TEXTURE2D_DESC textureDesc = {};
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = hasAlpha ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;
        
        // Create initial data if provided
        D3D11_SUBRESOURCE_DATA initialData = {};
        if (data) {
            initialData.pSysMem = data;
            initialData.SysMemPitch = width * (hasAlpha ? 4 : 3);
            initialData.SysMemSlicePitch = 0;
        }
        
        // Create the texture
        ID3D11Texture2D* texture = nullptr;
        HRESULT hr = device->CreateTexture2D(&textureDesc, data ? &initialData : nullptr, &texture);
        
        if (SUCCEEDED(hr)) {
            // Create shader resource view
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = textureDesc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MostDetailedMip = 0;
            srvDesc.Texture2D.MipLevels = 1;
            
            ID3D11ShaderResourceView* textureView = nullptr;
            hr = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
            
            if (SUCCEEDED(hr)) {
                // Store the texture and view for later use
                // In a real implementation, we would store these in a map
                // textures[currentTexture].texture = texture;
                // textures[currentTexture].textureView = textureView;
                
                std::cout << "Created texture with dimensions " << width << "x" << height 
                          << " (has alpha: " << (hasAlpha ? "yes" : "no") << ")" << std::endl;
            } else {
                std::cerr << "Error: Failed to create shader resource view" << std::endl;
                texture->Release();
            }
        } else {
            std::cerr << "Error: Failed to create texture" << std::endl;
        }
    }
#endif
}

void DirectXGraphicsAPI::DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) {
#ifdef PLATFORM_WINDOWS
    // Draw a debug line using DirectX
    if (device && context) {
        // Define line vertices
        struct LineVertex {
            float position[3];
            float color[3];
        };
        
        LineVertex vertices[2] = {
            { { start.x, start.y, start.z }, { color.x, color.y, color.z } },
            { { end.x, end.y, end.z }, { color.x, color.y, color.z } }
        };
        
        // Create vertex buffer
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(LineVertex) * 2;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0;
        
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices;
        
        ID3D11Buffer* vertexBuffer = nullptr;
        HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);
        
        if (SUCCEEDED(hr)) {
            // Set vertex buffer
            UINT stride = sizeof(LineVertex);
            UINT offset = 0;
            context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
            
            // Set primitive topology
            context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
            
            // Create and set a simple shader for lines if not already created
            static ID3D11VertexShader* lineVS = nullptr;
            static ID3D11PixelShader* linePS = nullptr;
            static ID3D11InputLayout* lineLayout = nullptr;
            
            if (!lineVS || !linePS || !lineLayout) {
                // Simple vertex shader for lines
                const char* vsCode = 
                    "struct VS_INPUT { float3 Position : POSITION; float3 Color : COLOR; };\n"
                    "struct VS_OUTPUT { float4 Position : SV_POSITION; float3 Color : COLOR; };\n"
                    "VS_OUTPUT main(VS_INPUT input) {\n"
                    "    VS_OUTPUT output;\n"
                    "    output.Position = float4(input.Position, 1.0f);\n"
                    "    output.Color = input.Color;\n"
                    "    return output;\n"
                    "}\n";
                
                // Simple pixel shader for lines
                const char* psCode = 
                    "struct PS_INPUT { float4 Position : SV_POSITION; float3 Color : COLOR; };\n"
                    "float4 main(PS_INPUT input) : SV_TARGET {\n"
                    "    return float4(input.Color, 1.0f);\n"
                    "}\n";
                
                // Compile vertex shader
                ID3DBlob* vsBlob = nullptr;
                ID3DBlob* errorBlob = nullptr;
                hr = D3DCompile(vsCode, strlen(vsCode), "LineVS", nullptr, nullptr, 
                               "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
                
                if (SUCCEEDED(hr)) {
                    // Create vertex shader
                    device->CreateVertexShader(vsBlob->GetBufferPointer(), 
                                              vsBlob->GetBufferSize(), nullptr, &lineVS);
                    
                    // Create input layout
                    D3D11_INPUT_ELEMENT_DESC layout[] = {
                        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 }
                    };
                    
                    device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(),
                                             vsBlob->GetBufferSize(), &lineLayout);
                    
                    vsBlob->Release();
                }
                
                // Compile pixel shader
                ID3DBlob* psBlob = nullptr;
                hr = D3DCompile(psCode, strlen(psCode), "LinePS", nullptr, nullptr,
                               "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
                
                if (SUCCEEDED(hr)) {
                    // Create pixel shader
                    device->CreatePixelShader(psBlob->GetBufferPointer(),
                                             psBlob->GetBufferSize(), nullptr, &linePS);
                    
                    psBlob->Release();
                }
                
                if (errorBlob) {
                    errorBlob->Release();
                }
            }
            
            // Set shaders and input layout
            context->VSSetShader(lineVS, nullptr, 0);
            context->PSSetShader(linePS, nullptr, 0);
            context->IASetInputLayout(lineLayout);
            
            // Draw the line
            context->Draw(2, 0);
            
            // Release vertex buffer
            vertexBuffer->Release();
        }
        
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
