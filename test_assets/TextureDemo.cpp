#include <windows.h>
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#include <GL/glu.h>
#include <iostream>
#include "../Vector3.h"
#include "../Model.h"
#include "../PointLight.h"
#include "../Texture.h"

// Example usage of texture support in GameEngineSavi
int main() {
    // Create a model
    Model* cube = new Model("cube.obj", Vector3(0, 0, 0), Vector3(0, 0, 0));
    
    // Load textures
    cube->loadTexture("albedo.png", "albedo");     // Color/diffuse texture
    cube->loadTexture("normal.png", "normal");     // Normal map for enhanced lighting
    cube->loadTexture("opacity.png", "opacity");   // Opacity/transparency map
    
    // Set texture tiling (repeat texture 2x horizontally, 1x vertically)
    cube->setTiling(2.0f, 1.0f);
    
    // Create a light
    std::vector<PointLight> lights;
    PointLight light;
    light.position = Vector3(5, 5, 5);
    light.color = Vector3(1.0f, 1.0f, 1.0f);
    lights.push_back(light);
    
    // Render the model with textures and lighting
    cube->Render(lights);
    
    // Clean up
    delete cube;
    
    return 0;
}
