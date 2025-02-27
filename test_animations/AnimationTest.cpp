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

// Simple test for the animation system
int main() {
    std::cout << "Animation System Test" << std::endl;
    std::cout << "====================" << std::endl;
    
    // Load animation from file
    std::cout << "Loading animation from file..." << std::endl;
    std::shared_ptr<Animation::Animation> walkAnimation = AnimationLoader::LoadFromFile("test_animations/walk/cube_walk.savanim");
    
    if (!walkAnimation) {
        std::cerr << "Failed to load animation!" << std::endl;
        return 1;
    }
    
    std::cout << "Animation loaded successfully:" << std::endl;
    std::cout << "  Name: " << walkAnimation->GetName() << std::endl;
    std::cout << "  Duration: " << walkAnimation->GetDuration() << std::endl;
    std::cout << "  Looping: " << (walkAnimation->IsLooping() ? "Yes" : "No") << std::endl;
    std::cout << "  Keyframes: " << walkAnimation->GetKeyFrames().size() << std::endl;
    
    // Create a model and game object
    std::cout << "\nCreating model and game object..." << std::endl;
    Model* cubeModel = new Model("test_animations/walk/cube_pose1.obj", Vector3(0, 0, 0), Vector3(0, 0, 0));
    
    GameObject* cubeObject = new GameObject("AnimatedCube");
    cubeObject->AddMesh(cubeModel);
    
    // Create an animation component
    std::cout << "Creating animation component..." << std::endl;
    AnimationComponent* animComponent = new AnimationComponent();
    animComponent->SetTargetModel(cubeModel);
    animComponent->AddAnimation("walk", walkAnimation);
    
    // Add the animation component to the game object
    cubeObject->AddComponent(animComponent);
    
    // Test animation playback
    std::cout << "\nTesting animation playback..." << std::endl;
    animComponent->Play("walk");
    
    // Simulate a few frames of animation
    const int numFrames = 10;
    const float deltaTime = 0.1f;
    
    for (int i = 0; i < numFrames; ++i) {
        float time = i * deltaTime;
        std::cout << "Frame " << i << " (Time: " << time << "s):" << std::endl;
        
        // Update the animation
        animComponent->Update(deltaTime);
        
        // Print the first few vertices to verify animation
        std::cout << "  First vertex: (" 
                  << cubeModel->vertices[0] << ", " 
                  << cubeModel->vertices[1] << ", " 
                  << cubeModel->vertices[2] << ")" << std::endl;
    }
    
    std::cout << "\nAnimation test completed successfully!" << std::endl;
    
    // Clean up
    delete cubeObject;
    delete cubeModel;
    
    return 0;
}
