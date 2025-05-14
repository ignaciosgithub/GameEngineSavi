#include "GraphicsAPIFactory.h"
#include "OpenGLGraphicsAPI.h"
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
    // Create the graphics API using OpenGL on all platforms
    try {
        graphicsAPI = std::make_shared<OpenGLGraphicsAPI>();
        if (!graphicsAPI->Initialize()) {
            std::cout << "Failed to initialize OpenGL" << std::endl;
            return false;
        }
        std::cout << "Successfully initialized OpenGL graphics API" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception initializing OpenGL: " << e.what() << std::endl;
        return false;
    }
    
    return true;
}

std::shared_ptr<IGraphicsAPI> GraphicsAPIFactory::GetGraphicsAPI() {
    return graphicsAPI;
}
