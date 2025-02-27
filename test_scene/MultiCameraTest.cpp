#include "../Scene.h"
#include "../Camera.h"
#include "../GameObject.h"
#include "../Model.h"
#include "../PointLight.h"
#include "../Vector3.h"
#include <memory>
#include <iostream>

int main() {
    // Create scene
    Scene scene;
    
    // Initialize scene
    scene.Initialize();
    
    // Create main camera
    auto mainCamera = std::make_unique<Camera>(
        Vector3(0, 5, -10),  // Position
        Vector3(0, -0.5f, 1), // Look direction
        60.0f                // Field of view
    );
    
    // Create minimap camera (top-down view)
    auto minimapCamera = std::make_unique<Camera>(
        Vector3(0, 20, 0),   // Position
        Vector3(0, -1, 0),   // Look direction (straight down)
        90.0f                // Field of view
    );
    
    // Configure minimap camera viewport (bottom-left corner, 25% of screen)
    minimapCamera->SetViewport(0.0f, 0.0f, 0.25f, 0.25f);
    
    // Set cameras in scene
    scene.SetMainCamera(mainCamera.get());
    scene.SetMinimapCamera(minimapCamera.get());
    
    // Allow resolution changes during gameplay
    scene.SetResolutionChangeAllowed(true);
    
    // Create a point light
    PointLight light;
    light.position = Vector3(5, 10, 5);
    light.color = Vector3(1, 1, 1);
    light.intensity = 1.0f;
    light.range = 50.0f;
    scene.AddPointLight(light);
    
    // Create a ground plane
    auto ground = std::make_unique<GameObject>();
    ground->position = Vector3(0, 0, 0);
    ground->scale = Vector3(20, 0.1f, 20);
    // Add a model component (in a real implementation)
    // ground->AddComponent<Model>("ground.obj");
    scene.AddGameObject(std::move(ground));
    
    // Create some objects to view from different cameras
    for (int i = 0; i < 5; i++) {
        auto cube = std::make_unique<GameObject>();
        cube->position = Vector3(i * 2 - 4, 1, i * 2 - 4);
        cube->scale = Vector3(1, 1, 1);
        // Add a model component (in a real implementation)
        // cube->AddComponent<Model>("cube.obj");
        scene.AddGameObject(std::move(cube));
    }
    
    // Create a player object (visible in both cameras)
    auto player = std::make_unique<GameObject>();
    player->position = Vector3(0, 1, 0);
    player->scale = Vector3(1, 2, 1);
    // Add a model component (in a real implementation)
    // player->AddComponent<Model>("player.obj");
    scene.AddGameObject(std::move(player));
    
    std::cout << "Scene setup complete with multiple cameras" << std::endl;
    std::cout << "Main camera: position=" << mainCamera->GetPosition().toString() << std::endl;
    std::cout << "Minimap camera: position=" << minimapCamera->GetPosition().toString() << std::endl;
    std::cout << "Press 'R' to toggle resolution change allowed" << std::endl;
    std::cout << "Press 'M' to toggle minimap visibility" << std::endl;
    
    // Run the scene
    scene.Run();
    
    return 0;
}
