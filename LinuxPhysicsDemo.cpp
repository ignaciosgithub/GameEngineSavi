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
    std::unique_ptr<RigidBody> rigidBody;
    
    SimpleGameObject(const std::string& _name, const Vector3& _position)
        : name(_name), position(_position), rotation(0, 0, 0) {
        model = SimplifiedModel(position, rotation);
    }
    
    void Update(float deltaTime) {
        // Update position and rotation from rigid body if available
        if (rigidBody) {
            position = rigidBody->GetGameObject()->position;
            rotation = rigidBody->GetGameObject()->rotation;
            model.position = position;
            model.rotation = rotation;
        }
    }
};

// Simple GameObject implementation for RigidBody
class SimpleGameObjectImpl : public GameObject {
public:
    SimpleGameObjectImpl(const std::string& name, const Vector3& position)
        : GameObject(name, position, Vector3(0, 0, 0), Vector3(1, 1, 1), std::vector<PointLight>()) {
    }
    
    void Render(const std::vector<PointLight>& lights) override {
        // No rendering in Linux demo
    }
};

int main() {
    std::cout << "Physics System Demo" << std::endl;
    std::cout << "===================" << std::endl;
    
    // Create physics and collision systems
    auto physicsSystem = std::make_unique<PhysicsSystem>();
    auto collisionSystem = std::make_unique<CollisionSystem>();
    
    // Connect the systems
    physicsSystem->SetCollisionSystem(collisionSystem.get());
    
    // Set gravity
    physicsSystem->SetGravity(-9.81f);
    
    // Create top pyramid with gravity
    auto topPyramid = std::make_unique<SimpleGameObject>("TopPyramid", Vector3(0, 2, 0));
    topPyramid->model.createPyramid();
    
    auto topGameObject = std::make_unique<SimpleGameObjectImpl>("TopPyramid", Vector3(0, 2, 0));
    auto topRb = std::make_unique<RigidBody>();
    topRb->EnableGravity(true);
    topRb->SetFrictionCoefficient(0.5f);
    topRb->SetMass(1.0f);
    topRb->SetGameObject(topGameObject.get());
    
    // Add the rigid body to the physics system
    physicsSystem->AddBody(topRb.get());
    topPyramid->rigidBody = std::move(topRb);
    
    // Create bottom pyramid without gravity
    auto bottomPyramid = std::make_unique<SimpleGameObject>("BottomPyramid", Vector3(0, 0, 0));
    bottomPyramid->model.createPyramid();
    
    auto bottomGameObject = std::make_unique<SimpleGameObjectImpl>("BottomPyramid", Vector3(0, 0, 0));
    auto bottomRb = std::make_unique<RigidBody>();
    bottomRb->EnableGravity(false);
    bottomRb->SetFrictionCoefficient(0.3f);
    bottomRb->SetMass(2.0f);
    bottomRb->SetGameObject(bottomGameObject.get());
    
    // Add the rigid body to the physics system
    physicsSystem->AddBody(bottomRb.get());
    bottomPyramid->rigidBody = std::move(bottomRb);
    
    // Simulate physics for 10 seconds
    float deltaTime = 0.016f; // 60 FPS
    for (int i = 0; i < 600; i++) {
        physicsSystem->Update(deltaTime);
        
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
    
    std::cout << "Physics simulation complete." << std::endl;
    return 0;
}
