#include "../../Model.h"
#include "../../Graphics/Core/GraphicsAPIFactory.h"
#include "../../Vector3.h"
#include "../../Vector2.h"
#include <iostream>
#include <map>

int main() {
    // Initialize graphics API
    auto& factory = GraphicsAPIFactory::GetInstance();
    auto graphics = factory.GetGraphicsAPI();
    
    // Create model
    Model model;
    
    // Load OBJ file
    std::string objPath = "cube_with_materials.obj";
    std::cout << "Loading OBJ file: " << objPath << std::endl;
    
    bool success = model.LoadFromFile(objPath);
    
    if (success) {
        std::cout << "OBJ file loaded successfully!" << std::endl;
        std::cout << "Vertices: " << model.vertices.size() / 3 << std::endl;
        std::cout << "Normals: " << model.normals.size() / 3 << std::endl;
        std::cout << "Texture coordinates: " << model.texCoords.size() / 2 << std::endl;
    } else {
        std::cerr << "Failed to load OBJ file!" << std::endl;
    }
    
    return 0;
}
