#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../Animation/Animation.h"
#include "../Animation/AnimationComponent.h"
#include "../Animation/AnimationLoader.h"
#include "../Model.h"
#include "../GameObject.h"
#include "../Vector3.h"
#include "../CollisionSystem.h"
#include "../RigidBody.h"

// Test for animation and collision integration
int main() {
    std::cout << "Animation Collision Test" << std::endl;
    std::cout << "=======================" << std::endl;
    
    // Load animation from file
    std::cout << "Loading animation from file..." << std::endl;
    std::shared_ptr<Animation::Animation> pushAnimation = AnimationLoader::LoadFromFile("test_animations/walk/cube_walk.savanim");
    
    if (!pushAnimation) {
        std::cerr << "Failed to load animation!" << std::endl;
        return 1;
    }
    
    // Create collision system
    CollisionSystem collisionSystem;
    
    // Create animated object
    GameObject* animatedObject = new GameObject("AnimatedCube");
    animatedObject->position = Vector3(0, 1, 0);
    animatedObject->scale = Vector3(1, 1, 1);
    
    // Add model component
    Model* cubeModel = new Model("test_animations/walk/cube_pose1.obj", Vector3(0, 0, 0), Vector3(0, 0, 0));
    animatedObject->AddMesh(cubeModel);
    
    // Add animation component
    AnimationComponent* animComponent = new AnimationComponent();
    animComponent->SetTargetModel(cubeModel);
    animComponent->AddAnimation("push", pushAnimation);
    animatedObject->AddComponent(animComponent);
    
    // Add rigid body component
    RigidBody* animatedRb = new RigidBody();
    animatedRb->mass = 1.0f;
    animatedRb->useGravity = false;
    animatedObject->AddComponent(animatedRb);
    
    // Create target object
    GameObject* targetObject = new GameObject("TargetCube");
    targetObject->position = Vector3(2, 1, 0);
    targetObject->scale = Vector3(1, 1, 1);
    
    // Add model component
    Model* targetModel = new Model("test_animations/walk/cube_pose1.obj", Vector3(0, 0, 0), Vector3(0, 0, 0));
    targetObject->AddMesh(targetModel);
    
    // Add rigid body component
    RigidBody* targetRb = new RigidBody();
    targetRb->mass = 0.5f;
    targetRb->useGravity = false;
    targetObject->AddComponent(targetRb);
    
    // Register objects with collision system
    collisionSystem.RegisterObject(animatedObject);
    collisionSystem.RegisterObject(targetObject);
    
    // Play animation
    animComponent->Play("push");
    
    // Simulate a few frames
    const int numFrames = 20;
    const float deltaTime = 0.1f;
    
    for (int i = 0; i < numFrames; ++i) {
        float time = i * deltaTime;
        std::cout << "Frame " << i << " (Time: " << time << "s):" << std::endl;
        
        // Update animation
        animComponent->Update(deltaTime);
        
        // Update collision system
        collisionSystem.Update(deltaTime);
        
        // Print object positions
        std::cout << "  Animated object position: " << animatedObject->position.toString() << std::endl;
        std::cout << "  Target object position: " << targetObject->position.toString() << std::endl;
    }
    
    std::cout << "\nAnimation collision test completed!" << std::endl;
    
    // Clean up
    delete animatedObject;
    delete targetObject;
    
    return 0;
}
