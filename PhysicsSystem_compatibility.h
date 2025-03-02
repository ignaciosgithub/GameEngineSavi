#ifndef PHYSICS_SYSTEM_COMPATIBILITY_H
#define PHYSICS_SYSTEM_COMPATIBILITY_H

#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "ProjectSettings/ProjectSettings.h"

// Compatibility layer for PhysicsSystem to ensure cross-platform builds

// Helper methods for PhysicsSystem
inline void InitializePhysicsSystem(PhysicsSystem* system) {
    if (!system) return;
    
    // Load settings from ProjectSettings
    auto& settings = ProjectSettings::GetInstance();
    
    // Set gravity
    system->SetGravity(settings.GetPhysicsGravity());
    
    // Set fixed time step
    system->SetFixedTimeStep(settings.GetPhysicsTimeStep());
    
    // Set collision detection
    system->SetEnableCollisions(settings.GetEnableCollisions());
    
    // Set global restitution
    system->SetGlobalRestitution(settings.GetGlobalRestitution());
}

// Helper methods for CollisionSystem
inline bool CheckCollision(CollisionSystem* system, RigidBody* bodyA, RigidBody* bodyB, CollisionInfo& info) {
    if (!system || !bodyA || !bodyB) return false;
    
    GameObject* objA = bodyA->GetGameObject();
    GameObject* objB = bodyB->GetGameObject();
    
    if (!objA || !objB) return false;
    
    // For simplicity, we'll just check sphere-sphere collision
    Vector3 posA = objA->GetPosition();
    Vector3 posB = objB->GetPosition();
    
    // Use a fixed radius for simplicity
    float radiusA = 1.0f;
    float radiusB = 1.0f;
    
    // Calculate distance between centers
    Vector3 direction = posB - posA;
    float distance = direction.magnitude();
    
    // Check if spheres are overlapping
    float minDistance = radiusA + radiusB;
    
    if (distance >= minDistance) {
        return false;
    }
    
    // Calculate collision info
    info.normal = direction.normalized();
    info.depth = minDistance - distance;
    info.contactPoint = posA + info.normal * radiusA;
    
    return true;
}

inline void ResolveCollision(CollisionSystem* system, RigidBody* bodyA, RigidBody* bodyB, 
                            const CollisionInfo& info, float restitution = 0.5f) {
    if (!system || !bodyA || !bodyB) return;
    
    // Skip if both bodies are kinematic
    if (bodyA->GetIsKinematic() && bodyB->GetIsKinematic()) {
        return;
    }
    
    // Calculate relative velocity
    Vector3 relativeVelocity = bodyB->GetVelocity() - bodyA->GetVelocity();
    
    // Calculate relative velocity along the normal
    float velocityAlongNormal = relativeVelocity.dot(info.normal);
    
    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0) {
        return;
    }
    
    // Calculate impulse scalar
    float j = -(1.0f + restitution) * velocityAlongNormal;
    j /= (1.0f / bodyA->GetMass()) + (1.0f / bodyB->GetMass());
    
    // Apply impulse
    Vector3 impulse = info.normal * j;
    
    if (!bodyA->GetIsKinematic()) {
        bodyA->SetVelocity(bodyA->GetVelocity() - impulse / bodyA->GetMass());
    }
    
    if (!bodyB->GetIsKinematic()) {
        bodyB->SetVelocity(bodyB->GetVelocity() + impulse / bodyB->GetMass());
    }
    
    // Positional correction to prevent sinking
    const float percent = 0.2f; // Penetration percentage to correct
    const float slop = 0.01f; // Penetration allowance
    
    Vector3 correction = info.normal * (std::max(info.depth - slop, 0.0f) / 
                                       ((1.0f / bodyA->GetMass()) + (1.0f / bodyB->GetMass()))) * percent;
    
    if (!bodyA->GetIsKinematic()) {
        GameObject* objA = bodyA->GetGameObject();
        if (objA) {
            objA->SetPosition(objA->GetPosition() - correction / bodyA->GetMass());
        }
    }
    
    if (!bodyB->GetIsKinematic()) {
        GameObject* objB = bodyB->GetGameObject();
        if (objB) {
            objB->SetPosition(objB->GetPosition() + correction / bodyB->GetMass());
        }
    }
}

#endif // PHYSICS_SYSTEM_COMPATIBILITY_H
