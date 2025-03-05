#include "Scene_minimal.h"
#include "GameObject.h"
#include "PointLight.h"
#include <iostream>

// Simplified implementation of CreateDefaultObjects
void Scene::CreateDefaultObjects() {
    std::cout << "Creating default objects in scene..." << std::endl;
    
    // Create a default light
    PointLight light;
    light.SetPosition(Vector3(0, 5, 0));
    light.SetColor(Vector3(1, 1, 1));
    light.SetIntensity(1.0f);
    light.SetRange(10.0f);
    
    // Add the light to the scene
    pointLights.push_back(light);
    
    std::cout << "Default objects created" << std::endl;
}
