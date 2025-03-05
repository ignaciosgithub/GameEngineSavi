#include "GraphicsAPIFactory.h"
#include "OpenGLGraphicsAPI.h"
#include "DirectXGraphicsAPI.h"
#include "../../platform.h"
#include <iostream>
#include <stdexcept>

GraphicsAPIFactory& GraphicsAPIFactory::GetInstance() {
    static GraphicsAPIFactory instance;
    return instance;
}

std::shared_ptr<IGraphicsAPI> GraphicsAPIFactory::CreateGraphicsAPI() {
#ifdef PLATFORM_WINDOWS
    // Try to create DirectX graphics API first
    try {
        currentAPI = std::make_shared<DirectXGraphicsAPI>();
        std::cout << "Created DirectX graphics API" << std::endl;
        
        if (!currentAPI->Initialize()) {
            std::cerr << "Failed to initialize DirectX graphics API, falling back to OpenGL" << std::endl;
            throw std::runtime_error("DirectX initialization failed");
        }
    } catch (const std::exception& e) {
        std::cerr << "DirectX error: " << e.what() << std::endl;
        std::cerr << "Falling back to OpenGL graphics API" << std::endl;
        currentAPI = std::make_shared<OpenGLGraphicsAPI>();
        
        if (!currentAPI->Initialize()) {
            std::cerr << "Failed to initialize OpenGL graphics API" << std::endl;
            currentAPI.reset();
        }
    }
#else
    currentAPI = std::make_shared<OpenGLGraphicsAPI>();
    std::cout << "Created OpenGL graphics API" << std::endl;
    
    if (!currentAPI->Initialize()) {
        std::cerr << "Failed to initialize graphics API" << std::endl;
        currentAPI.reset();
    }
#endif
    
    return currentAPI;
}

std::shared_ptr<IGraphicsAPI> GraphicsAPIFactory::GetGraphicsAPI() {
    if (!currentAPI) {
        return CreateGraphicsAPI();
    }
    return currentAPI;
}
