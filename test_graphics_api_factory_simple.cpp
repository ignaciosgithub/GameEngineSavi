#include "Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>

// Mock ShaderProgram implementation for testing
class ShaderProgram {
public:
    void Use() const {}
};

int main() {
    std::cout << "Testing GraphicsAPIFactory with DirectX-to-OpenGL fallback..." << std::endl;
    
    // Get the GraphicsAPIFactory instance
    GraphicsAPIFactory& factory = GraphicsAPIFactory::GetInstance();
    
    // Create the graphics API
    std::shared_ptr<IGraphicsAPI> api = factory.CreateGraphicsAPI();
    
    if (api) {
        std::cout << "Successfully created and initialized graphics API" << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to create and initialize graphics API" << std::endl;
        return 1;
    }
}
