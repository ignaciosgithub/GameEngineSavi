#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Vector3.h"
#include "PhysicsSystem.h"

// Simple physics demo that doesn't require OpenGL or other dependencies
int main() {
    std::cout << "Physics System Demo" << std::endl;
    std::cout << "===================" << std::endl;
    
    // Create physics system
    PhysicsSystem physicsSystem;
    
    // Set gravity
    physicsSystem.SetGravity(-9.81f);
    
    // Create two objects
    Vector3 topPosition(0, 2, 0);
    Vector3 bottomPosition(0, 0, 0);
    
    // Print initial positions
    std::cout << "Initial positions:" << std::endl;
    std::cout << "Top object: " << topPosition.x << ", " << topPosition.y << ", " << topPosition.z << std::endl;
    std::cout << "Bottom object: " << bottomPosition.x << ", " << bottomPosition.y << ", " << bottomPosition.z << std::endl;
    std::cout << std::endl;
    
    // Simulate physics for 10 seconds
    float deltaTime = 0.016f; // 60 FPS
    float gravity = physicsSystem.GetGravity();
    float velocity = 0.0f;
    
    for (int i = 0; i < 600; i++) {
        // Apply gravity to velocity
        velocity += gravity * deltaTime;
        
        // Update position
        topPosition.y += velocity * deltaTime;
        
        // Check for collision with bottom object
        if (topPosition.y <= bottomPosition.y + 0.5f) {
            // Collision detected
            topPosition.y = bottomPosition.y + 0.5f;
            
            // Apply collision response
            velocity = -velocity * 0.5f; // Bounce with energy loss
            
            // Apply force to bottom object
            bottomPosition.y -= 0.01f; // Simple approximation
        }
        
        // Print positions every 60 frames (1 second)
        if (i % 60 == 0) {
            std::cout << "Time: " << i * deltaTime << "s" << std::endl;
            std::cout << "Top object: " << topPosition.x << ", " << topPosition.y << ", " << topPosition.z << std::endl;
            std::cout << "Bottom object: " << bottomPosition.x << ", " << bottomPosition.y << ", " << bottomPosition.z << std::endl;
            std::cout << "Velocity: " << velocity << std::endl;
            std::cout << std::endl;
        }
        
        // Simulate frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    std::cout << "Physics simulation complete." << std::endl;
    return 0;
}
