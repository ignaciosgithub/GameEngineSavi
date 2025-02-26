#include "Scene.h"
#include "EngineCondition.h"
#include "MonoBehaviourLike.h"
#include <iostream>

void Scene::Load() {
    // Load scene resources
    std::cout << "Scene loaded" << std::endl;
}

void Scene::Run() {
    // Start running the scene
    isRunning = true;
    std::cout << "Scene running" << std::endl;
}

void Scene::Stop() {
    // Stop running the scene
    isRunning = false;
    std::cout << "Scene stopped" << std::endl;
}

void Scene::RenderScene() {
    // Render the scene
    for (auto& gameObject : gameObjects) {
        // Render the game object
        gameObject->Render(std::vector<PointLight>());
    }
}

void Scene::Initialize() {
    // Initialize the scene
    time = std::unique_ptr<EngineTime>(new EngineTime());
    physicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
    physicsSystem->Initialize();
    
    std::cout << "Scene initialized" << std::endl;
}

void Scene::Update(float deltaTime) {
    // Update the scene
    
    // Update physics with fixed timestep
    physicsAccumulator += deltaTime;
    
    while (physicsAccumulator >= physicsTimeStep) {
        // Update physics
        if (physicsSystem && !EngineCondition::IsInEditorEditing() && !EngineCondition::IsInEditorCompiling()) {
            physicsSystem->Update(physicsTimeStep);
        }
        
        physicsAccumulator -= physicsTimeStep;
    }
    
    // Update game objects
    for (auto& gameObject : gameObjects) {
        // Update the game object components
        for (auto& component : gameObject->GetComponents<MonoBehaviourLike>()) {
            // Pass deltaTime to Update method
            component->Update(deltaTime);
        }
    }
    
    // Update frame count
    frameCount++;
}

void Scene::Render() {
    // Render the scene
    RenderScene();
}

void Scene::SetMainCamera(Camera* camera) {
    // Set the main camera for rendering
    // For now, just store the camera pointer
    // In a real implementation, this would set up the camera for rendering
}

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
    // Add a game object to the scene
    gameObjects.push_back(std::move(gameObject));
}

void Scene::AddGameObject(GameObject* gameObject) {
    // Add a game object to the scene (raw pointer version)
    // Create a custom deleter struct
    struct NoDelete {
        void operator()(GameObject* ptr) const {}
    };
    
    // Create a unique_ptr with the custom deleter
    std::unique_ptr<GameObject, NoDelete> ptr(gameObject, NoDelete());
    
    // We need to create a new standard unique_ptr
    gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
}

void Scene::AddCamera(std::unique_ptr<Camera> camera) {
    // Add a camera to the scene
    cameras.push_back(std::move(camera));
}

void Scene::AddCamera(Camera* camera) {
    // Add a camera to the scene (raw pointer version)
    // Create a custom deleter struct
    struct NoDelete {
        void operator()(Camera* ptr) const {}
    };
    
    // Create a unique_ptr with the custom deleter
    std::unique_ptr<Camera, NoDelete> ptr(camera, NoDelete());
    
    // We need to create a new standard unique_ptr
    cameras.push_back(std::unique_ptr<Camera>(camera));
}

void Scene::SetPhysicsSystem(std::unique_ptr<PhysicsSystem> system) {
    // Set the physics system
    physicsSystem = std::move(system);
}

PhysicsSystem* Scene::GetPhysicsSystem() const {
    // Get the physics system
    return physicsSystem.get();
}
