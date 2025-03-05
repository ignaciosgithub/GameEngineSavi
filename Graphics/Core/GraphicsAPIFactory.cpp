#include "GraphicsAPIFactory.h"
#include "OpenGLGraphicsAPI.h"
#include "DirectXGraphicsAPI.h"
#include "../../platform.h"
#include <iostream>

GraphicsAPIFactory& GraphicsAPIFactory::GetInstance() {
    static GraphicsAPIFactory instance;
    return instance;
}

std::shared_ptr<IGraphicsAPI> GraphicsAPIFactory::CreateGraphicsAPI() {
#ifdef PLATFORM_WINDOWS
    currentAPI = std::make_shared<DirectXGraphicsAPI>();
    std::cout << "Created DirectX graphics API" << std::endl;
#else
    currentAPI = std::make_shared<OpenGLGraphicsAPI>();
    std::cout << "Created OpenGL graphics API" << std::endl;
#endif
    
    if (!currentAPI->Initialize()) {
        std::cerr << "Failed to initialize graphics API" << std::endl;
        currentAPI.reset();
    }
    
    return currentAPI;
}

std::shared_ptr<IGraphicsAPI> GraphicsAPIFactory::GetGraphicsAPI() {
    if (!currentAPI) {
        return CreateGraphicsAPI();
    }
    return currentAPI;
}
