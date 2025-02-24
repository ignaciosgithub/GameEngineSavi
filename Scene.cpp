#include "Scene.h"
#include "MonoBehaviourLike.h"
#include <thread>
#include <chrono>

void Scene::Load() {
    // Load a scene here (e.g., create GameObjects and Cameras)
    // For simplicity, this just initializes the Time class
    time = std::make_unique<Time>();
}

void Scene::Run() {
    // Initialize objects and components
    for (auto& gameObject : gameObjects) {
        for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
            mb->Awake();
            mb->Start();
        }
    }

    isRunning = true;

    while (isRunning) {
        time->Update(); // Update the Time class to get the deltaTime
        
        // Update physics if we have a physics system
        if (physicsSystem) {
            physicsSystem->Update(time->DeltaTime());
        }

        // Call Update() on all MonoBehaviourLike components in the scene
        for (auto& gameObject : gameObjects) {
            for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
                mb->Update();
            }
        }

        RenderScene(); // Renders all objects in the scene

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Simulate frame rate (60 FPS)
    }
}

void Scene::Stop() {
    // Call OnDestroy() on all MonoBehaviourLike components
    for (auto& gameObject : gameObjects) {
        for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
            mb->OnDestroy();
        }
    }

    gameObjects.clear();
    cameras.clear();
    isRunning = false;
}

void Scene::RenderScene() {
    // Render each GameObject in the scene
    for (const auto& gameObject : gameObjects) {
        gameObject->Render(gameObject->lights);
    }

    // Render each Camera view in the scene
    for (const auto& camera : cameras) {
        //camera->Render();
    }
}

// Add GameObjects and Cameras to the scene
void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
    gameObjects.emplace_back(std::move(gameObject));
}

void Scene::AddCamera(std::unique_ptr<Camera> camera) {
    cameras.emplace_back(std::move(camera));
}

// Physics system management
void Scene::SetPhysicsSystem(std::unique_ptr<PhysicsSystem> system) {
    physicsSystem = std::move(system);
}

PhysicsSystem* Scene::GetPhysicsSystem() const {
    return physicsSystem.get();
}
