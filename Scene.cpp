#include "Scene.h"
#include "MonoBehaviourLike.h"
#include "EngineCondition.h"
#include <thread>
#include <chrono>
#include <iostream>

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

    // Set the appropriate engine state based on context
    if (EngineCondition::IsInEditor()) {
        EngineCondition::EnterPlayMode();
    }

    #ifdef DEBUG_BUILD
    std::cout << "Scene running in " << 
        (EngineCondition::IsInEditorPlaying() ? "editor play mode" : 
         EngineCondition::IsDebugBuild() ? "debug build" : 
         EngineCondition::IsReleaseBuild() ? "release build" : "unknown mode") 
        << std::endl;
    #endif

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

        // Additional debug information in debug builds
        #ifdef DEBUG_BUILD
        if (EngineCondition::IsDebugBuild() && frameCount % 300 == 0) { // Every ~5 seconds at 60 FPS
            std::cout << "Debug info: " << gameObjects.size() << " objects, " 
                      << "Frame time: " << time->DeltaTime() * 1000.0f << "ms" << std::endl;
        }
        frameCount++;
        #endif

        RenderScene(); // Renders all objects in the scene

        // Different frame rates based on build type
        if (EngineCondition::IsDebugBuild()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // 60 FPS in debug
        } else if (EngineCondition::IsReleaseBuild()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(8)); // 120 FPS in release
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Default 60 FPS
        }
    }
}

void Scene::Stop() {
    // Call OnDestroy() on all MonoBehaviourLike components
    for (auto& gameObject : gameObjects) {
        for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
            mb->OnDestroy();
        }
    }

    // If we're in editor, switch back to edit mode
    if (EngineCondition::IsInEditorPlaying()) {
        EngineCondition::EnterEditMode();
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
