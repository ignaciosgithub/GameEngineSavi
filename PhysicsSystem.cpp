#include "PhysicsSystem.h"
#include "RigidBody.h"
#include "CollisionSystem.h"
#include <algorithm>

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
                }
            }
        }
    }
}

void PhysicsSystem::AddBody(RigidBody* body) {
    // Check if body is already in the system
    if (std::find(bodies.begin(), bodies.end(), body) == bodies.end()) {
        bodies.push_back(body);
    }
}

void PhysicsSystem::RemoveBody(RigidBody* body) {
    // Remove body from the system
    bodies.erase(std::remove(bodies.begin(), bodies.end(), body), bodies.end());
}

void PhysicsSystem::SetGravity(float value) {
    gravity = value;
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
