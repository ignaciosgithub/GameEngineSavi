#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "RigidBody.h"
#include "Vector3.h"
#include "SimplifiedModel.h"

// Simple GameObject class for Linux demo
class SimpleGameObject {
public:
    std::string name;
    Vector3 position;
    Vector3 rotation;
    SimplifiedModel model;
    RigidBody* rigidBody;
    
    SimpleGameObject(const std::string& _name, const Vector3& _position)
        : name(_name), position(_position), rotation(0, 0, 0), rigidBody(nullptr) {
        model = SimplifiedModel(position, rotation);
    }
    
    void Update(float deltaTime) {
        // Update position and rotation from rigid body if available
        if (rigidBody) {
            // For the demo, we'll just update the position directly
            // since we don't have a full GameObject implementation
            position.y += rigidBody->GetVelocity().y * deltaTime;
            rotation += Vector3(0.1f, 0.1f, 0.1f) * deltaTime; // Simple rotation for demo
            model.position = position;
            model.rotation = rotation;
        }
    }
};

// Simple GameObject implementation for RigidBody
class SimpleGameObjectImpl {
public:
    std::string name;
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    
    SimpleGameObjectImpl(const std::string& _name, const Vector3& _position)
        : name(_name), position(_position), rotation(0, 0, 0), size(1, 1, 1) {
    }
};

int main() {
    std::cout << "Physics System Demo" << std::endl;
    std::cout << "===================" << std::endl;
    
    // Create physics and collision systems
    PhysicsSystem physicsSystem;
    CollisionSystem collisionSystem;
    
    // Connect the systems
    physicsSystem.SetCollisionSystem(&collisionSystem);
    
    // Set gravity
    physicsSystem.SetGravity(-9.81f);
    
    // Create top pyramid with gravity
    SimpleGameObject* topPyramid = new SimpleGameObject("TopPyramid", Vector3(0, 2, 0));
    topPyramid->model.createPyramid();
    
    SimpleGameObjectImpl* topGameObject = new SimpleGameObjectImpl("TopPyramid", Vector3(0, 2, 0));
    
    RigidBody* topRb = new RigidBody();
    topRb->EnableGravity(true);
    topRb->SetFrictionCoefficient(0.5f);
    topRb->SetMass(1.0f);
    topRb->SetGameObject(reinterpret_cast<GameObject*>(topGameObject));
    
    // Add the rigid body to the physics system
    physicsSystem.AddBody(topRb);
    topPyramid->rigidBody = topRb;
    
    // Create bottom pyramid without gravity
    SimpleGameObject* bottomPyramid = new SimpleGameObject("BottomPyramid", Vector3(0, 0, 0));
    bottomPyramid->model.createPyramid();
    
    SimpleGameObjectImpl* bottomGameObject = new SimpleGameObjectImpl("BottomPyramid", Vector3(0, 0, 0));
    
    RigidBody* bottomRb = new RigidBody();
    bottomRb->EnableGravity(false);
    bottomRb->SetFrictionCoefficient(0.3f);
    bottomRb->SetMass(2.0f);
    bottomRb->SetGameObject(reinterpret_cast<GameObject*>(bottomGameObject));
    
    // Add the rigid body to the physics system
    physicsSystem.AddBody(bottomRb);
    bottomPyramid->rigidBody = bottomRb;
    
    // Simulate physics for 10 seconds
    float deltaTime = 0.016f; // 60 FPS
    for (int i = 0; i < 600; i++) {
        physicsSystem.Update(deltaTime);
        
        // Update game objects
        topPyramid->Update(deltaTime);
        bottomPyramid->Update(deltaTime);
        
        // Print positions every 60 frames (1 second)
        if (i % 60 == 0) {
            std::cout << "Time: " << i * deltaTime << "s" << std::endl;
            std::cout << "Top Pyramid Position: " << topPyramid->position.x << ", " 
                      << topPyramid->position.y << ", " << topPyramid->position.z << std::endl;
            std::cout << "Bottom Pyramid Position: " << bottomPyramid->position.x << ", " 
                      << bottomPyramid->position.y << ", " << bottomPyramid->position.z << std::endl;
            std::cout << std::endl;
        }
        
        // Simulate frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    // Clean up
    delete topPyramid;
    delete bottomPyramid;
    delete topGameObject;
    delete bottomGameObject;
    delete topRb;
    delete bottomRb;
    
    std::cout << "Physics simulation complete." << std::endl;
    return 0;
}
