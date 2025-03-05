#include "GraphicsAPIFactory.h"
#include "OpenGLGraphicsAPI.h"
#include "DirectXGraphicsAPI.h"
#include "../../platform.h"
#include <iostream>

// Initialize static instance
GraphicsAPIFactory* GraphicsAPIFactory::instance = nullptr;

GraphicsAPIFactory& GraphicsAPIFactory::GetInstance() {
    if (!instance) {
        instance = new GraphicsAPIFactory();
    }
    return *instance;
}

GraphicsAPIFactory::GraphicsAPIFactory() : graphicsAPI(nullptr) {
}

GraphicsAPIFactory::~GraphicsAPIFactory() {
    if (graphicsAPI) {
        graphicsAPI->Shutdown();
        graphicsAPI = nullptr;
    }
}

bool GraphicsAPIFactory::Initialize() {
    // Create the appropriate graphics API based on platform
#ifdef PLATFORM_WINDOWS
    // Use DirectX on Windows
    try {
        graphicsAPI = std::make_shared<DirectXGraphicsAPI>();
        if (!graphicsAPI->Initialize()) {
            std::cout << "Failed to initialize DirectX, falling back to OpenGL" << std::endl;
            graphicsAPI = nullptr;
        }
    } catch (const std::exception& e) {
        std::cout << "Exception initializing DirectX: " << e.what() << std::endl;
        graphicsAPI = nullptr;
    }
    
    // Fallback to OpenGL if DirectX initialization fails
    if (!graphicsAPI) {
        try {
            graphicsAPI = std::make_shared<OpenGLGraphicsAPI>();
            if (!graphicsAPI->Initialize()) {
                std::cout << "Failed to initialize OpenGL fallback" << std::endl;
                return false;
            }
        } catch (const std::exception& e) {
            std::cout << "Exception initializing OpenGL fallback: " << e.what() << std::endl;
            return false;
        }
    }
#else
    // Use OpenGL on other platforms
    try {
        graphicsAPI = std::make_shared<OpenGLGraphicsAPI>();
        if (!graphicsAPI->Initialize()) {
            std::cout << "Failed to initialize OpenGL" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "Exception initializing OpenGL: " << e.what() << std::endl;
        return false;
    }
#endif

    return true;
}

std::shared_ptr<IGraphicsAPI> GraphicsAPIFactory::GetGraphicsAPI() {
    return graphicsAPI;
}
