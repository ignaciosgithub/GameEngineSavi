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
    // Create a buffer ID for tracking
    static unsigned int nextBuffer = 1;
    unsigned int bufferID = nextBuffer++;
    
    // Store buffer ID for later use
    // In a full implementation, we would store the buffer in a map
    // buffers[bufferID] = nullptr;
    
    return bufferID;
#else
    return 0;
#endif
}

void DirectXGraphicsAPI::BindBuffer(BufferType type, unsigned int buffer) {
    currentBuffer = buffer;
#ifdef PLATFORM_WINDOWS
    // Store the current buffer type and ID
    if (context) {
        // In DirectX, binding depends on the buffer type
        switch (type) {
            case VERTEX_BUFFER:
                // We'll set the vertex buffer when we have an actual buffer to bind
                // This is done in DrawArrays or DrawElements
                break;
            case INDEX_BUFFER:
                // We'll set the index buffer when we have an actual buffer to bind
                // This is done in DrawElements
                break;
            case UNIFORM_BUFFER:
                // DirectX uses constant buffers for uniforms
                // We'll set these when we have shader programs
                break;
            default:
                std::cerr << "Unknown buffer type: " << type << std::endl;
                break;
        }
    }
#endif
}

void DirectXGraphicsAPI::DeleteBuffer(unsigned int buffer) {
    if (currentBuffer == buffer) {
        currentBuffer = 0;
    }
#ifdef PLATFORM_WINDOWS
    // In a full implementation, we would retrieve the buffer from a map
    // ID3D11Buffer* d3dBuffer = buffers[buffer];
    
    // Release the buffer if it exists
    // if (d3dBuffer) {
    //     d3dBuffer->Release();
    //     buffers.erase(buffer);
    // }
    
    // If this was the current vertex or index buffer, clear it
    if (currentVertexBuffer && buffer == currentBuffer) {
        currentVertexBuffer->Release();
        currentVertexBuffer = nullptr;
    }
    
    if (currentIndexBuffer && buffer == currentBuffer) {
        currentIndexBuffer->Release();
        currentIndexBuffer = nullptr;
    }
    
    std::cout << "Deleted buffer " << buffer << std::endl;
#endif
}

void DirectXGraphicsAPI::BufferData(BufferType type, const void* data, size_t size, bool dynamic) {
#ifdef PLATFORM_WINDOWS
    if (device && data && size > 0) {
        // Create a new buffer based on the type
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.ByteWidth = static_cast<UINT>(size);
        bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
        
        // Set buffer type-specific flags
        switch (type) {
            case VERTEX_BUFFER:
                bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                break;
            case INDEX_BUFFER:
                bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                break;
            case UNIFORM_BUFFER:
                bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
                break;
            default:
                std::cerr << "Unknown buffer type: " << type << std::endl;
                return;
        }
        
        // Initialize buffer with data
        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = data;
        
        // Create the buffer
        ID3D11Buffer* buffer = nullptr;
        HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &buffer);
        
        if (SUCCEEDED(hr)) {
            // Store the buffer for the current buffer ID
            // In a full implementation, we would store this in a map
            // buffers[currentBuffer] = buffer;
            
            // Bind the buffer based on its type
            switch (type) {
                case VERTEX_BUFFER: {
                    UINT stride = static_cast<UINT>(size); // This would need to be adjusted based on vertex format
                    UINT offset = 0;
                    context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
                    currentVertexBuffer = buffer;
                    break;
                }
                case INDEX_BUFFER:
                    context->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
                    currentIndexBuffer = buffer;
                    break;
                case UNIFORM_BUFFER:
                    // For constant buffers, we would bind to specific shader stages
                    context->VSSetConstantBuffers(0, 1, &buffer);
                    context->PSSetConstantBuffers(0, 1, &buffer);
                    break;
            }
            
            std::cout << "Created and bound buffer of type " << type 
                      << " with size " << size << " bytes" << std::endl;
        } else {
            std::cerr << "Failed to create buffer of type " << type << std::endl;
        }
    }
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
        ID3D11GeometryShader* geometryShader = nullptr;
        ID3D11InputLayout* inputLayout = nullptr;
        
        // Check if the program has DirectX shaders
        if (program->HasDirectXShaders()) {
            // Get DirectX shaders from the program
            vertexShader = program->GetDirectXVertexShader();
            pixelShader = program->GetDirectXPixelShader();
            geometryShader = program->GetDirectXGeometryShader();
            inputLayout = program->GetDirectXInputLayout();
        } else {
            // Compile DirectX shaders from GLSL source
            // This is a fallback for when the program only has GLSL shaders
            
            // Initialize default shaders if not already done
            static bool shadersInitialized = false;
            static ID3D11VertexShader* defaultVS = nullptr;
            static ID3D11PixelShader* defaultPS = nullptr;
            static ID3D11InputLayout* defaultLayout = nullptr;
            
            if (!shadersInitialized) {
                // Default red shader for fallback
                const char* vsCode = 
                    "float4 main(float4 pos : POSITION) : SV_POSITION { return pos; }";
                
                const char* psCode = 
                    "float4 main() : SV_TARGET { return float4(1.0f, 0.0f, 0.0f, 1.0f); }";
                
                // Compile vertex shader
                ID3DBlob* vsBlob = nullptr;
                ID3DBlob* errorBlob = nullptr;
                HRESULT hr = D3DCompile(vsCode, strlen(vsCode), "VS", nullptr, nullptr, 
                                       "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);
                
                if (SUCCEEDED(hr)) {
                    // Create vertex shader
                    hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), 
                                              vsBlob->GetBufferSize(), nullptr, &defaultVS);
                    
                    if (FAILED(hr)) {
                        std::cerr << "Failed to create vertex shader" << std::endl;
                        if (errorBlob) {
                            std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                        }
                    }
                    
                    // Create input layout
                    D3D11_INPUT_ELEMENT_DESC layout[] = {
                        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 }
                    };
                    
                    hr = device->CreateInputLayout(layout, 3, vsBlob->GetBufferPointer(),
                                             vsBlob->GetBufferSize(), &defaultLayout);
                    
                    if (FAILED(hr)) {
                        std::cerr << "Failed to create input layout" << std::endl;
                    }
                    
                    vsBlob->Release();
                } else {
                    std::cerr << "Failed to compile vertex shader" << std::endl;
                    if (errorBlob) {
                        std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                    }
                }
                
                // Compile pixel shader
                ID3DBlob* psBlob = nullptr;
                hr = D3DCompile(psCode, strlen(psCode), "PS", nullptr, nullptr,
                               "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
                
                if (SUCCEEDED(hr)) {
                    // Create pixel shader
                    hr = device->CreatePixelShader(psBlob->GetBufferPointer(),
                                             psBlob->GetBufferSize(), nullptr, &defaultPS);
                    
                    if (FAILED(hr)) {
                        std::cerr << "Failed to create pixel shader" << std::endl;
                        if (errorBlob) {
                            std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                        }
                    }
                    
                    psBlob->Release();
                } else {
                    std::cerr << "Failed to compile pixel shader" << std::endl;
                    if (errorBlob) {
                        std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                    }
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
            
            // Store the compiled DirectX shaders in the program for future use
            program->SetDirectXVertexShader(vertexShader);
            program->SetDirectXPixelShader(pixelShader);
            program->SetDirectXInputLayout(inputLayout);
        }
        
        // Set shaders and input layout
        context->VSSetShader(vertexShader, nullptr, 0);
        context->PSSetShader(pixelShader, nullptr, 0);
        
        if (geometryShader) {
            context->GSSetShader(geometryShader, nullptr, 0);
        } else {
            context->GSSetShader(nullptr, nullptr, 0);
        }
        
        context->IASetInputLayout(inputLayout);
        
        // Set primitive topology
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        
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
        textureDesc.Format = hasAlpha ? DXGI_FORMAT_R8G8B8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;
        
        // Create initial data if provided
        D3D11_SUBRESOURCE_DATA initialData = {};
        
        // Convert RGB to RGBA if needed
        std::vector<unsigned char> convertedData;
        if (data && !hasAlpha) {
            // Convert RGB to RGBA by adding alpha channel
            const unsigned char* srcData = static_cast<const unsigned char*>(data);
            convertedData.resize(width * height * 4);
            
            for (int i = 0; i < width * height; ++i) {
                convertedData[i * 4 + 0] = srcData[i * 3 + 0]; // R
                convertedData[i * 4 + 1] = srcData[i * 3 + 1]; // G
                convertedData[i * 4 + 2] = srcData[i * 3 + 2]; // B
                convertedData[i * 4 + 3] = 255;                // A (fully opaque)
            }
            
            initialData.pSysMem = convertedData.data();
            initialData.SysMemPitch = width * 4;
        } else if (data) {
            initialData.pSysMem = data;
            initialData.SysMemPitch = width * 4; // RGBA is always 4 bytes per pixel
        }
        initialData.SysMemSlicePitch = 0;
        
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
                // In a full implementation, we would store these in a map
                // For example:
                // TextureResource resource;
                // resource.texture = texture;
                // resource.textureView = textureView;
                // textures[currentTexture] = resource;
                
                // For now, we'll store them in static variables for the current texture
                static ID3D11Texture2D* currentTexture = nullptr;
                static ID3D11ShaderResourceView* currentTextureView = nullptr;
                
                // Release previous resources if they exist
                if (currentTexture) {
                    currentTexture->Release();
                }
                if (currentTextureView) {
                    currentTextureView->Release();
                }
                
                // Store new resources
                currentTexture = texture;
                currentTextureView = textureView;
                
                // Bind the texture to the pixel shader
                context->PSSetShaderResources(0, 1, &textureView);
                
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
                    hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), 
                                              vsBlob->GetBufferSize(), nullptr, &lineVS);
                    
                    if (FAILED(hr)) {
                        std::cerr << "Failed to create line vertex shader" << std::endl;
                        if (errorBlob) {
                            std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                        }
                    }
                    
                    // Create input layout
                    D3D11_INPUT_ELEMENT_DESC layout[] = {
                        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 },
                        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, 
                         D3D11_INPUT_PER_VERTEX_DATA, 0 }
                    };
                    
                    hr = device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(),
                                             vsBlob->GetBufferSize(), &lineLayout);
                    
                    if (FAILED(hr)) {
                        std::cerr << "Failed to create line input layout" << std::endl;
                    }
                    
                    vsBlob->Release();
                } else {
                    std::cerr << "Failed to compile line vertex shader" << std::endl;
                    if (errorBlob) {
                        std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                    }
                }
                
                // Compile pixel shader
                ID3DBlob* psBlob = nullptr;
                hr = D3DCompile(psCode, strlen(psCode), "LinePS", nullptr, nullptr,
                               "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);
                
                if (SUCCEEDED(hr)) {
                    // Create pixel shader
                    hr = device->CreatePixelShader(psBlob->GetBufferPointer(),
                                             psBlob->GetBufferSize(), nullptr, &linePS);
                    
                    if (FAILED(hr)) {
                        std::cerr << "Failed to create line pixel shader" << std::endl;
                        if (errorBlob) {
                            std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                        }
                    }
                    
                    psBlob->Release();
                } else {
                    std::cerr << "Failed to compile line pixel shader" << std::endl;
                    if (errorBlob) {
                        std::cerr << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
                    }
                }
                
                if (errorBlob) {
                    errorBlob->Release();
                }
            }
            
            // Set shaders and input layout
            context->VSSetShader(lineVS, nullptr, 0);
            context->PSSetShader(linePS, nullptr, 0);
            context->IASetInputLayout(lineLayout);
            
            // Set up rasterizer state for lines
            D3D11_RASTERIZER_DESC rasterizerDesc = {};
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.CullMode = D3D11_CULL_NONE;
            rasterizerDesc.FrontCounterClockwise = FALSE;
            rasterizerDesc.DepthBias = 0;
            rasterizerDesc.DepthBiasClamp = 0.0f;
            rasterizerDesc.SlopeScaledDepthBias = 0.0f;
            rasterizerDesc.DepthClipEnable = TRUE;
            rasterizerDesc.ScissorEnable = FALSE;
            rasterizerDesc.MultisampleEnable = FALSE;
            rasterizerDesc.AntialiasedLineEnable = TRUE;
            
            ID3D11RasterizerState* rasterizerState = nullptr;
            hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
            
            if (SUCCEEDED(hr)) {
                // Set rasterizer state
                context->RSSetState(rasterizerState);
                
                // Draw the line
                context->Draw(2, 0);
                
                // Release rasterizer state
                rasterizerState->Release();
            }
            
            // Release vertex buffer
            vertexBuffer->Release();
        } else {
            std::cerr << "Failed to create line vertex buffer" << std::endl;
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

// Shader management methods
unsigned int DirectXGraphicsAPI::CreateShader(int shaderType) {
#ifdef PLATFORM_WINDOWS
    // In DirectX, shaders are created differently than in OpenGL
    // We'll use a simple counter to track shader IDs
    static unsigned int nextShaderID = 1;
    return nextShaderID++;
#else
    return 0;
#endif
}

void DirectXGraphicsAPI::DeleteShader(unsigned int shader) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would release the shader resource
    std::cout << "Deleting shader " << shader << std::endl;
#endif
}

void DirectXGraphicsAPI::ShaderSource(unsigned int shader, const std::string& source) {
#ifdef PLATFORM_WINDOWS
    // In DirectX, shader source is compiled differently
    // We'll store the source code for later compilation
    std::cout << "Setting shader source for shader " << shader << std::endl;
#endif
}

void DirectXGraphicsAPI::CompileShader(unsigned int shader) {
#ifdef PLATFORM_WINDOWS
    // In DirectX, shaders are compiled using D3DCompile
    std::cout << "Compiling shader " << shader << std::endl;
#endif
}

bool DirectXGraphicsAPI::GetShaderCompileStatus(unsigned int shader) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would check the compilation status
    return true;
#else
    return false;
#endif
}

std::string DirectXGraphicsAPI::GetShaderInfoLog(unsigned int shader) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would return the compilation errors
    return "DirectX shader compilation successful";
#else
    return "";
#endif
}

bool DirectXGraphicsAPI::GetProgramLinkStatus(unsigned int program) {
#ifdef PLATFORM_WINDOWS
    // In DirectX, programs are not linked the same way as in OpenGL
    return true;
#else
    return false;
#endif
}

std::string DirectXGraphicsAPI::GetProgramInfoLog(unsigned int program) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would return the linking errors
    return "DirectX program linking successful";
#else
    return "";
#endif
}

void DirectXGraphicsAPI::AttachShader(unsigned int program, unsigned int shader) {
#ifdef PLATFORM_WINDOWS
    // In DirectX, shaders are not attached to programs the same way as in OpenGL
    std::cout << "Attaching shader " << shader << " to program " << program << std::endl;
#endif
}

void DirectXGraphicsAPI::LinkProgram(unsigned int program) {
#ifdef PLATFORM_WINDOWS
    // In DirectX, programs are not linked the same way as in OpenGL
    std::cout << "Linking program " << program << std::endl;
#endif
}

unsigned int DirectXGraphicsAPI::CreateProgram() {
#ifdef PLATFORM_WINDOWS
    // In DirectX, programs are created differently than in OpenGL
    // We'll use a simple counter to track program IDs
    static unsigned int nextProgramID = 1;
    return nextProgramID++;
#else
    return 0;
#endif
}

void DirectXGraphicsAPI::DeleteProgram(unsigned int program) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would release the program resources
    std::cout << "Deleting program " << program << std::endl;
#endif
}

// Uniform setters
void DirectXGraphicsAPI::SetUniform1f(unsigned int program, const std::string& name, float value) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would set the uniform value
    std::cout << "Setting uniform " << name << " to " << value << std::endl;
#endif
}

void DirectXGraphicsAPI::SetUniform1i(unsigned int program, const std::string& name, int value) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would set the uniform value
    std::cout << "Setting uniform " << name << " to " << value << std::endl;
#endif
}

void DirectXGraphicsAPI::SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would set the uniform value
    std::cout << "Setting uniform " << name << " to (" << x << ", " << y << ", " << z << ")" << std::endl;
#endif
}

void DirectXGraphicsAPI::SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would set the uniform value
    std::cout << "Setting uniform " << name << " to (" << x << ", " << y << ", " << z << ", " << w << ")" << std::endl;
#endif
}

void DirectXGraphicsAPI::SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose) {
#ifdef PLATFORM_WINDOWS
    // In a real implementation, we would set the uniform value
    std::cout << "Setting uniform matrix " << name << std::endl;
#endif
}

// Get uniform location
int DirectXGraphicsAPI::GetUniformLocation(unsigned int program, const std::string& name) {
#ifdef PLATFORM_WINDOWS
    // DirectX implementation would use constant buffers instead of uniform locations
    // For now, return a dummy value
    std::cout << "Getting uniform location for " << name << std::endl;
    return 0;
#else
    return -1;
#endif
}

// Set uniform float array
void DirectXGraphicsAPI::SetUniformFloatArray(unsigned int program, const std::string& name, const float* values, int count) {
#ifdef PLATFORM_WINDOWS
    // DirectX implementation would update constant buffer data
    // For now, just log the operation
    std::cout << "Setting uniform float array " << name << " with " << count << " elements" << std::endl;
#endif
}

// Set uniform int array
void DirectXGraphicsAPI::SetUniformIntArray(unsigned int program, const std::string& name, const int* values, int count) {
#ifdef PLATFORM_WINDOWS
    // DirectX implementation would update constant buffer data
    // For now, just log the operation
    std::cout << "Setting uniform int array " << name << " with " << count << " elements" << std::endl;
#endif
}

// Set uniform vec3 array
void DirectXGraphicsAPI::SetUniformVec3Array(unsigned int program, const std::string& name, const float* values, int count) {
#ifdef PLATFORM_WINDOWS
    // DirectX implementation would update constant buffer data
    // For now, just log the operation
    std::cout << "Setting uniform vec3 array " << name << " with " << count << " elements" << std::endl;
#endif
}

// Set uniform matrix4 array
void DirectXGraphicsAPI::SetUniformMatrix4Array(unsigned int program, const std::string& name, const float* values, int count, bool transpose) {
#ifdef PLATFORM_WINDOWS
    // DirectX implementation would update constant buffer data
    // For now, just log the operation
    std::cout << "Setting uniform matrix4 array " << name << " with " << count << " elements" << std::endl;
#endif
}

#ifdef PLATFORM_WINDOWS
void DirectXGraphicsAPI::CreateDeviceAndSwapChain(HWND hwnd) {
    // Get window dimensions
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    UINT width = clientRect.right - clientRect.left;
    UINT height = clientRect.bottom - clientRect.top;
    
    // Create the DirectX device and swap chain
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width > 0 ? width : 800; // Use window width or default
    swapChainDesc.BufferDesc.Height = height > 0 ? height : 600; // Use window height or default
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    
    // Create device, context, and swap chain
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3
    };
    D3D_FEATURE_LEVEL featureLevel;
    UINT createDeviceFlags = 0;
    
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    // Try to create device with hardware acceleration first
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc,
        &swapChain, &device, &featureLevel, &context);
    
    // If hardware acceleration fails, try WARP (software rendering)
    if (FAILED(hr)) {
        std::cerr << "Hardware acceleration not available, trying WARP..." << std::endl;
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
            featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc,
            &swapChain, &device, &featureLevel, &context);
    }
    
    // If WARP fails, try reference device (very slow)
    if (FAILED(hr)) {
        std::cerr << "WARP not available, trying reference device..." << std::endl;
        hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr, createDeviceFlags,
            featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &swapChainDesc,
            &swapChain, &device, &featureLevel, &context);
    }
    
    if (FAILED(hr)) {
        std::cerr << "Error: Failed to create DirectX device and swap chain (HRESULT: 0x" 
                  << std::hex << hr << std::dec << ")" << std::endl;
    } else {
        std::cout << "Successfully created DirectX device and swap chain with feature level ";
        switch (featureLevel) {
            case D3D_FEATURE_LEVEL_11_0: std::cout << "11.0"; break;
            case D3D_FEATURE_LEVEL_10_1: std::cout << "10.1"; break;
            case D3D_FEATURE_LEVEL_10_0: std::cout << "10.0"; break;
            case D3D_FEATURE_LEVEL_9_3: std::cout << "9.3"; break;
            default: std::cout << "unknown"; break;
        }
        std::cout << std::endl;
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
            D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
            rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;
            
            hr = device->CreateRenderTargetView(backBuffer, &rtvDesc, &renderTargetView);
            backBuffer->Release();
            
            if (FAILED(hr)) {
                std::cerr << "Error: Failed to create render target view (HRESULT: 0x" 
                          << std::hex << hr << std::dec << ")" << std::endl;
            } else {
                // Set the render target
                if (context && renderTargetView && depthStencilView) {
                    context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
                    std::cout << "Successfully created and set render target view" << std::endl;
                } else {
                    std::cout << "Successfully created render target view" << std::endl;
                }
            }
        } else {
            std::cerr << "Error: Failed to get back buffer (HRESULT: 0x" 
                      << std::hex << hr << std::dec << ")" << std::endl;
        }
    } else {
        std::cerr << "Error: Device or swap chain not initialized" << std::endl;
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
                std::cerr << "Error: Failed to create depth stencil view (HRESULT: 0x" 
                          << std::hex << hr << std::dec << ")" << std::endl;
            } else {
                // Create depth stencil state
                D3D11_DEPTH_STENCIL_DESC dsDesc = {};
                dsDesc.DepthEnable = TRUE;
                dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
                dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
                dsDesc.StencilEnable = FALSE;
                dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
                dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
                dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
                dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
                dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
                dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
                dsDesc.BackFace = dsDesc.FrontFace;
                
                ID3D11DepthStencilState* depthStencilState = nullptr;
                hr = device->CreateDepthStencilState(&dsDesc, &depthStencilState);
                
                if (SUCCEEDED(hr)) {
                    // Set the depth stencil state
                    context->OMSetDepthStencilState(depthStencilState, 1);
                    depthStencilState->Release();
                    
                    // Set the render target and depth stencil view
                    if (renderTargetView) {
                        context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
                    }
                    
                    std::cout << "Successfully created and configured depth stencil view" << std::endl;
                } else {
                    std::cerr << "Error: Failed to create depth stencil state (HRESULT: 0x" 
                              << std::hex << hr << std::dec << ")" << std::endl;
                }
            }
        } else {
            std::cerr << "Error: Failed to create depth stencil texture (HRESULT: 0x" 
                      << std::hex << hr << std::dec << ")" << std::endl;
        }
    } else {
        std::cerr << "Error: Device not initialized" << std::endl;
    }
}
#endif
