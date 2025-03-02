#include "Scene.h"
#include "GameObject.h"
#include "NavMesh.h"
#include "NavMeshManager.h"
#include "AIEntity.h"
#include "Raycast.h"
#include "RaycastHit.h"
#include "Model.h"
#include "Camera.h"
#include "TimeManager.h"
#include <iostream>

int main() {
    // Create a scene
    Scene scene;
    scene.Initialize();
    
    // Create a camera
    Camera* camera = new Camera();
    camera->position = Vector3(0, 5, -10);
    // Set camera direction instead of rotation
    camera->LookAt(Vector3(0, 0, 0));
    scene.AddCamera(std::unique_ptr<Camera>(camera));
    scene.SetMainCamera(camera);
    
    // Create a floor
    GameObject* floor = new GameObject("Floor");
    floor->position = Vector3(0, 0, 0);
    floor->size = Vector3(10, 0.1f, 10);
    Model* floorModel = new Model();
    // Set up floor model
    floor->AddMesh(floorModel);
    scene.AddGameObject(std::unique_ptr<GameObject>(floor));
    
    // Create some obstacles
    GameObject* obstacle1 = new GameObject("Obstacle1");
    obstacle1->position = Vector3(2, 1, 2);
    obstacle1->size = Vector3(1, 2, 1);
    Model* obstacle1Model = new Model();
    // Set up obstacle1 model
    obstacle1->AddMesh(obstacle1Model);
    scene.AddGameObject(std::unique_ptr<GameObject>(obstacle1));
    
    GameObject* obstacle2 = new GameObject("Obstacle2");
    obstacle2->position = Vector3(-2, 1, -2);
    obstacle2->size = Vector3(1, 2, 1);
    Model* obstacle2Model = new Model();
    // Set up obstacle2 model
    obstacle2->AddMesh(obstacle2Model);
    scene.AddGameObject(std::unique_ptr<GameObject>(obstacle2));
    
    // Create an AI entity
    GameObject* aiGameObject = new GameObject("AIEntity");
    aiGameObject->position = Vector3(0, 1, 0);
    aiGameObject->size = Vector3(0.5f, 1, 0.5f);
    Model* aiModel = new Model();
    // Set up AI model
    aiGameObject->AddMesh(aiModel);
    scene.AddGameObject(std::unique_ptr<GameObject>(aiGameObject));
    
    AIEntity* aiEntity = new AIEntity();
    aiEntity->SetGameObject(aiGameObject);
    aiEntity->SetAIPosition(Vector3(0, 1, 0));
    aiEntity->SetAIOrientation(Vector3(0, 0, 1));
    aiEntity->SetMaxAngleDiff(45.0f);
    aiEntity->SetMaxDist(2.0f);
    
    // Initialize the navigation mesh manager
    NavMeshManager& navMeshManager = NavMeshManager::GetInstance();
    navMeshManager.Initialize();
    navMeshManager.SetRefreshRate(10.0f);
    navMeshManager.AddAIEntity(aiEntity);
    
    // Refresh the navigation mesh
    navMeshManager.RefreshNavMesh();
    
    // Main loop
    TimeManager& timeManager = TimeManager::GetInstance();
    bool running = true;
    
    while (running) {
        timeManager.Update();
        float deltaTime = timeManager.DeltaTime();
        
        // Update the navigation mesh manager
        navMeshManager.Update(deltaTime);
        
        // Test raycasting
        Vector3 direction = (camera->lookDirection).normalized();
        Raycast ray(camera->position, direction);
        RaycastHit hit;
        
        if (ray.Cast(hit, &scene)) {
            std::cout << "Hit object: " << hit.gameObject->GetName() << std::endl;
            std::cout << "Hit point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ")" << std::endl;
            std::cout << "Hit normal: (" << hit.normal.x << ", " << hit.normal.y << ", " << hit.normal.z << ")" << std::endl;
            std::cout << "Hit distance: " << hit.distance << std::endl;
        }
        
        // Update the scene
        scene.Update(deltaTime);
        
        // Render the scene
        scene.Render();
        
        // Exit condition (simplified for demo)
        // In a real implementation, you would check for user input
        // running = !exitCondition;
    }
    
    return 0;
}
