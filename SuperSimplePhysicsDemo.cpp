#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Vector3.h"

// Super simple physics demo that doesn't require any other dependencies
int main() {
    std::cout << "Super Simple Physics Demo" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // Physics parameters
    const float gravity = -9.81f;
    const float friction = 0.5f;
    const float restitution = 0.7f; // Bounciness
    
    // Create two objects
    Vector3 topPosition(0, 2, 0);
    Vector3 bottomPosition(0, 0, 0);
    Vector3 topVelocity(0, 0, 0);
    Vector3 bottomVelocity(0, 0, 0);
    float topMass = 1.0f;
    float bottomMass = 2.0f;
    bool topHasGravity = true;
    bool bottomHasGravity = false;
    
    // Print initial positions
    std::cout << "Initial positions:" << std::endl;
    std::cout << "Top object: " << topPosition.x << ", " << topPosition.y << ", " << topPosition.z << std::endl;
    std::cout << "Bottom object: " << bottomPosition.x << ", " << bottomPosition.y << ", " << bottomPosition.z << std::endl;
    std::cout << std::endl;
    
    // Simulate physics for 10 seconds
    float deltaTime = 0.016f; // 60 FPS
    
    for (int i = 0; i < 600; i++) {
        // Apply gravity to top object
        if (topHasGravity) {
            Vector3 gravityForce(0, gravity * topMass, 0);
            topVelocity += gravityForce * (deltaTime / topMass);
        }
        
        // Apply gravity to bottom object
        if (bottomHasGravity) {
            Vector3 gravityForce(0, gravity * bottomMass, 0);
            bottomVelocity += gravityForce * (deltaTime / bottomMass);
        }
        
        // Update positions
        topPosition += topVelocity * deltaTime;
        bottomPosition += bottomVelocity * deltaTime;
        
        // Check for collision
        float collisionThreshold = 0.5f; // Size of objects
        if (topPosition.y <= bottomPosition.y + collisionThreshold) {
            // Collision detected
            topPosition.y = bottomPosition.y + collisionThreshold;
            
            // Calculate collision response
            float totalMass = topMass + bottomMass;
            float topRatio = bottomMass / totalMass;
            float bottomRatio = topMass / totalMass;
            
            // Apply impulse
            Vector3 relativeVelocity = topVelocity - bottomVelocity;
            float impulseStrength = -(1.0f + restitution) * relativeVelocity.y;
            impulseStrength /= (1.0f / topMass) + (1.0f / bottomMass);
            
            // Apply impulse to velocities
            Vector3 impulse(0, impulseStrength, 0);
            topVelocity += impulse * (1.0f / topMass);
            bottomVelocity -= impulse * (1.0f / bottomMass);
            
            // Apply friction
            Vector3 tangentialVelocity = relativeVelocity;
            tangentialVelocity.y = 0; // Remove normal component
            
            if (tangentialVelocity.magnitude() > 0.001f) {
                Vector3 frictionImpulse = tangentialVelocity.normalized() * (-friction * impulseStrength);
                topVelocity += frictionImpulse * (1.0f / topMass);
                bottomVelocity -= frictionImpulse * (1.0f / bottomMass);
            }
            
            // Apply torque (simplified as rotation around y-axis)
            Vector3 topRotation(0, 0.1f, 0);
            Vector3 bottomRotation(0, -0.05f, 0);
        }
        
        // Print positions every 60 frames (1 second)
        if (i % 60 == 0) {
            std::cout << "Time: " << i * deltaTime << "s" << std::endl;
            std::cout << "Top object: " << topPosition.x << ", " << topPosition.y << ", " << topPosition.z << std::endl;
            std::cout << "Top velocity: " << topVelocity.x << ", " << topVelocity.y << ", " << topVelocity.z << std::endl;
            std::cout << "Bottom object: " << bottomPosition.x << ", " << bottomPosition.y << ", " << bottomPosition.z << std::endl;
            std::cout << "Bottom velocity: " << bottomVelocity.x << ", " << bottomVelocity.y << ", " << bottomVelocity.z << std::endl;
            std::cout << std::endl;
        }
        
        // Simulate frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    std::cout << "Physics simulation complete." << std::endl;
    return 0;
}
