#ifndef GRAPHICS_API_FACTORY_H
#define GRAPHICS_API_FACTORY_H

#include "IGraphicsAPI.h"
#include <memory>

class GraphicsAPIFactory {
public:
    // Get the singleton instance
    static GraphicsAPIFactory& GetInstance();
    
    // Create the appropriate graphics API for the current platform
    std::shared_ptr<IGraphicsAPI> CreateGraphicsAPI();
    
    // Get the current graphics API instance
    std::shared_ptr<IGraphicsAPI> GetGraphicsAPI();
    
private:
    GraphicsAPIFactory() = default;
    ~GraphicsAPIFactory() = default;
    
    // Delete copy constructor and assignment operator
    GraphicsAPIFactory(const GraphicsAPIFactory&) = delete;
    GraphicsAPIFactory& operator=(const GraphicsAPIFactory&) = delete;
    
    // Current graphics API instance
    std::shared_ptr<IGraphicsAPI> currentAPI;
};

#endif // GRAPHICS_API_FACTORY_H
