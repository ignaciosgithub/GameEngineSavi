#include "PhysicsSystem.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include "EngineCondition.h"
#include <algorithm>
#include <iostream>

PhysicsSystem::PhysicsSystem() : gravity(-9.81f), collisionSystem(nullptr) {
}

PhysicsSystem::~PhysicsSystem() {
    // Clear the bodies vector
    bodies.clear();
}

void PhysicsSystem::Update(float deltaTime) {
    // Apply gravity and update physics for all bodies
    for (auto body : bodies) {
        if (body->IsGravityEnabled()) {
            // Apply gravity force (F = m*g)
            Vector3 gravityForce(0, gravity * body->GetMass(), 0);
            body->ApplyForce(gravityForce);
        }
        
        // Update physics state (handled by RigidBody)
        body->PhysicsUpdate(deltaTime);
    }
    
    // Check for collisions if we have a collision system
    if (collisionSystem) {
        // Additional debug information in debug builds
        #ifdef DEBUG_BUILD
        if (EngineCondition::IsDebugBuild()) {
            std::cout << "Physics update: " << bodies.size() << " bodies, " 
                      << "Delta time: " << deltaTime * 1000.0f << "ms" << std::endl;
        }
        #endif
        
        // Check all possible pairs of bodies for collisions
        for (size_t i = 0; i < bodies.size(); i++) {
            for (size_t j = i + 1; j < bodies.size(); j++) {
                RigidBody* bodyA = bodies[i];
                RigidBody* bodyB = bodies[j];
                
                // Skip if both bodies are static
                if (!bodyA->IsDynamic() && !bodyB->IsDynamic()) {
                    continue;
                }
                
                // Check for collision
                CollisionInfo collisionInfo;
                if (collisionSystem->CheckCollision(bodyA, bodyB, collisionInfo)) {
                    // Resolve the collision
                    collisionSystem->ResolveCollision(bodyA, bodyB, collisionInfo);
                    
                    // Trigger collision events
                    bodyA->OnCollision(bodyB, collisionInfo);
                    bodyB->OnCollision(bodyA, collisionInfo);
                    
                    // Additional debug information in debug builds
                    #ifdef DEBUG_BUILD
                    if (EngineCondition::IsDebugBuild()) {
                        std::cout << "Collision detected between " << bodyA->GetGameObject()->GetName() 
                                  << " and " << bodyB->GetGameObject()->GetName() << std::endl;
                    }
                    #endif
                }
            }
        }
    }
    
    // Pause physics updates in editor editing and compiling modes
    if (EngineCondition::IsInEditorEditing() || EngineCondition::IsInEditorCompiling()) {
        #ifdef DEBUG_BUILD
        std::cout << "Physics updates paused in editor" << std::endl;
        #endif
        return;
    }
}

void PhysicsSystem::AddBody(RigidBody* body) {
    // Check if body is already in the system
    if (std::find(bodies.begin(), bodies.end(), body) == bodies.end()) {
        bodies.push_back(body);
        
        // Additional debug information in debug builds
        #ifdef DEBUG_BUILD
        if (EngineCondition::IsDebugBuild()) {
            std::cout << "Added body to physics system: " 
                      << body->GetGameObject()->GetName() << std::endl;
        }
        #endif
    }
}

void PhysicsSystem::RemoveBody(RigidBody* body) {
    // Remove body from the system
    bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
    
    // Additional debug information in debug builds
    #ifdef DEBUG_BUILD
    if (EngineCondition::IsDebugBuild()) {
        std::cout << "Removed body from physics system: " 
                  << body->GetGameObject()->GetName() << std::endl;
    }
    #endif
}

void PhysicsSystem::SetGravity(float value) {
    gravity = value;
    
    // Additional debug information in debug builds
    #ifdef DEBUG_BUILD
    if (EngineCondition::IsDebugBuild()) {
        std::cout << "Gravity set to: " << gravity << std::endl;
    }
    #endif
}

float PhysicsSystem::GetGravity() const {
    return gravity;
}

void PhysicsSystem::SetCollisionSystem(CollisionSystem* system) {
    collisionSystem = system;
}

CollisionSystem* PhysicsSystem::GetCollisionSystem() const {
    return collisionSystem;
}
