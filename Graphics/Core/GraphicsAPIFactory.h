#ifndef GRAPHICS_API_FACTORY_H
#define GRAPHICS_API_FACTORY_H

#include <memory>
#include "IGraphicsAPI.h"

class GraphicsAPIFactory {
public:
    static GraphicsAPIFactory& GetInstance();
    
    // Initialize the appropriate graphics API based on platform
    bool Initialize();
    
    // Get the current graphics API
    std::shared_ptr<IGraphicsAPI> GetGraphicsAPI();
    
private:
    GraphicsAPIFactory();
    ~GraphicsAPIFactory();
    
    // Singleton instance
    static GraphicsAPIFactory* instance;
    
    // Current graphics API
    std::shared_ptr<IGraphicsAPI> graphicsAPI;
};

#endif // GRAPHICS_API_FACTORY_H
