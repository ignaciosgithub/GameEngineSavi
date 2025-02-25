#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include "../Animation/Animation.h"
#include "../Animation/AnimationComponent.h"
#include "../Animation/AnimationLoader.h"
#include "../Model.h"
#include "../GameObject.h"
#include "../RigidBody.h"
#include "../Scene.h"
#include "../CollisionSystem.h"
#include "../Vector3.h"
#include "../platform.h"

int main() {
    std::cout << "Animation Collision Test" << std::endl;
    std::cout << "=======================" << std::endl;
    
    // Create scene
    Scene scene;
    scene.Initialize();
    
    // Create animated cube
    auto* animatedCube = new GameObject("AnimatedCube");
    auto* animatedModel = new Model("test_animations/walk/cube_pose1.obj", 
                                  Vector3(0,0,0), Vector3(-2,0,0));
    animatedCube->AddMesh(animatedModel);
    
    // Add animation component
    auto* animComponent = new AnimationComponent();
    animComponent->SetTargetModel(animatedModel);
    
    // Load animation from file
    std::shared_ptr<Animation> pushAnimation = AnimationLoader::LoadFromFile("test_animations/walk/cube_walk.savanim");
    if (!pushAnimation) {
        std::cerr << "Failed to load animation!" << std::endl;
        return 1;
    }
    
    // Add animation to component
    animComponent->AddAnimation("push", pushAnimation);
    animatedCube->AddComponent(animComponent);
    
    // Create physics cube
    auto* physicsCube = new GameObject("PhysicsCube");
    auto* physicsModel = new Model("test_animations/walk/cube_pose1.obj",
                                 Vector3(0,0,0), Vector3(2,0,0));
    physicsCube->AddMesh(physicsModel);
    
    // Add rigid body with no gravity
    auto* rigidBody = new RigidBody();
    rigidBody->EnableGravity(false);
    rigidBody->SetMass(1.0f);
    rigidBody->SetFrictionCoefficient(0.5f);
    physicsCube->AddComponent(rigidBody);
    
    // Add objects to scene
    scene.AddGameObject(animatedCube);
    scene.AddGameObject(physicsCube);
    
    // Set up collision system
    CollisionSystem* collisionSystem = new CollisionSystem();
    
    // Register the animated model for collision updates
    animatedModel->onVerticesUpdated = [collisionSystem](const Model* model) {
        collisionSystem->UpdateModelCollisionData(model);
    };
    
    // Start animation
    animComponent->Play("push");
    
    // Run simulation for a few frames
    const int numFrames = 60;
    const float deltaTime = 1.0f / 60.0f;
    
    for (int i = 0; i < numFrames; ++i) {
        float time = i * deltaTime;
        std::cout << "Frame " << i << " (Time: " << time << "s):" << std::endl;
        
        // Update animation
        animComponent->Update(deltaTime);
        
        // Check for collision
        CollisionInfo info;
        if (collisionSystem->CheckCollision(rigidBody, nullptr, info)) {
            std::cout << "  Collision detected!" << std::endl;
            collisionSystem->ResolveCollision(rigidBody, nullptr, info);
        }
        
        // Print physics cube velocity
        Vector3 vel = rigidBody->GetVelocity();
        std::cout << "  Physics cube velocity: (" << vel.x << ", " << vel.y << ", " << vel.z << ")" << std::endl;
    }
    
    std::cout << "\nAnimation collision test completed!" << std::endl;
    
    // Clean up
    delete collisionSystem;
    
    return 0;
}
