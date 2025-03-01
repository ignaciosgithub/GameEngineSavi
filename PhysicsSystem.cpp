#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "ProjectSettings/ProjectSettings.h"
#include <iostream>

// Implementation of PhysicsSystem methods

PhysicsSystem::PhysicsSystem() 
    : gravity(9.81f), fixedTimeStep(1.0f/60.0f), enableCollisions(true), globalRestitution(0.5f), collisionSystem(nullptr) {
    // Initialize with default values
}

PhysicsSystem::~PhysicsSystem() {
    // Clean up resources
    if (collisionSystem) {
        delete collisionSystem;
        collisionSystem = nullptr;
    }
    
    // Clear bodies
    bodies.clear();
}

void PhysicsSystem::Initialize() {
    // Load settings from ProjectSettings
    auto& settings = ProjectSettings::GetInstance();
    
    // Set gravity
    gravity = settings.GetPhysicsGravity();
    
    // Set fixed time step
    fixedTimeStep = settings.GetPhysicsTimeStep();
    
    // Set collision detection
    enableCollisions = settings.GetEnableCollisions();
    
    // Set global restitution
    globalRestitution = settings.GetGlobalRestitution();
    
    // Create collision system if not already created
    if (!collisionSystem) {
        collisionSystem = new CollisionSystem();
    }
    
    std::cout << "PhysicsSystem initialized with:" << std::endl;
    std::cout << "  Gravity: " << gravity << std::endl;
    std::cout << "  Fixed time step: " << fixedTimeStep << std::endl;
    std::cout << "  Collisions enabled: " << (enableCollisions ? "Yes" : "No") << std::endl;
    std::cout << "  Global restitution: " << globalRestitution << std::endl;
}

void PhysicsSystem::Update(float deltaTime) {
    if (bodies.empty()) {
        return;
    }
    
    // Apply gravity to all bodies
    for (auto& body : bodies) {
        if (body->GetUseGravity()) {
            body->AddForce(Vector3(0, -gravity * body->GetMass(), 0));
        }
    }
    
    // Update positions based on forces
    for (auto& body : bodies) {
        body->Update(deltaTime);
    }
    
    // Check for collisions if enabled
    if (enableCollisions && collisionSystem) {
        for (size_t i = 0; i < bodies.size(); ++i) {
            for (size_t j = i + 1; j < bodies.size(); ++j) {
                RigidBody* bodyA = bodies[i];
                RigidBody* bodyB = bodies[j];
                
                if (!bodyA->GetIsKinematic() || !bodyB->GetIsKinematic()) {
                    CollisionInfo collision;
                    if (collisionSystem->CheckCollision(bodyA, bodyB, collision)) {
                        // Handle collision response
                        collisionSystem->ResolveCollision(bodyA, bodyB, collision, globalRestitution);
                        
                        // Log collision
                        std::cout << "Collision detected between bodies" << std::endl;
                    }
                }
            }
        }
    }
}

void PhysicsSystem::AddBody(RigidBody* body) {
    if (!body) {
        return;
    }
    
    // Check if body already exists
    auto it = std::find(bodies.begin(), bodies.end(), body);
    if (it == bodies.end()) {
        bodies.push_back(body);
        
        // Log addition
        std::cout << "Added rigid body" << std::endl;
    }
}

void PhysicsSystem::RemoveBody(RigidBody* body) {
    if (!body) {
        return;
    }
    
    // Find and remove body
    auto it = std::find(bodies.begin(), bodies.end(), body);
    if (it != bodies.end()) {
        bodies.erase(it);
        
        // Log removal
        std::cout << "Removed rigid body for object" << std::endl;
    }
}

void PhysicsSystem::SetGravity(float value) {
    gravity = value;
}

float PhysicsSystem::GetGravity() const {
    return gravity;
}

void PhysicsSystem::SetFixedTimeStep(float value) {
    fixedTimeStep = value > 0.0f ? value : 1.0f/60.0f;
}

float PhysicsSystem::GetFixedTimeStep() const {
    return fixedTimeStep;
}

void PhysicsSystem::SetEnableCollisions(bool enable) {
    enableCollisions = enable;
}

bool PhysicsSystem::GetEnableCollisions() const {
    return enableCollisions;
}

void PhysicsSystem::SetGlobalRestitution(float value) {
    globalRestitution = value;
}

float PhysicsSystem::GetGlobalRestitution() const {
    return globalRestitution;
}

void PhysicsSystem::SetCollisionSystem(CollisionSystem* system) {
    if (collisionSystem) {
        delete collisionSystem;
    }
    
    collisionSystem = system;
}

CollisionSystem* PhysicsSystem::GetCollisionSystem() const {
    return collisionSystem;
}
