#include "CollisionSystem.h"
#include "GameObject.h"
#include "RigidBody.h"
#include "CollisionInfo.h"
#include <limits>
#include <algorithm>

// Implementation of CollisionSystem methods

CollisionSystem::CollisionSystem() {
    // Initialize collision system
}

CollisionSystem::~CollisionSystem() {
    // Clean up collision system
}

CollisionSystem::BoundingBox CollisionSystem::CalculateBoundingBox(const Model* model) {
    BoundingBox box;
    
    // Initialize min and max to extreme values
    box.min = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    box.max = Vector3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    
    // Since we can't access private members directly, we'll use a simplified approach
    // In a real implementation, Model would provide a method to get its bounding box
    
    // Simplified: Use a default box size based on the GameObject's position
    Vector3 position(0, 0, 0); // Default position
    float size = 1.0f; // Default size
    
    box.min = position - Vector3(size, size, size);
    box.max = position + Vector3(size, size, size);
    
    return box;
}

bool CollisionSystem::CheckCollision(const RigidBody* bodyA, const RigidBody* bodyB, CollisionInfo& info) {
    // Simple collision check implementation
    if (!bodyA || !bodyB) {
        return false;
    }
    
    // Get game objects
    GameObject* objA = bodyA->GetGameObject();
    GameObject* objB = bodyB->GetGameObject();
    
    if (!objA || !objB) {
        return false;
    }
    
    // Get positions
    Vector3 posA = objA->GetPosition();
    Vector3 posB = objB->GetPosition();
    
    // Get sizes
    Vector3 sizeA = objA->GetScale();
    Vector3 sizeB = objB->GetScale();
    
    // Check for AABB collision
    bool collisionX = posA.x + sizeA.x >= posB.x - sizeB.x && posB.x + sizeB.x >= posA.x - sizeA.x;
    bool collisionY = posA.y + sizeA.y >= posB.y - sizeB.y && posB.y + sizeB.y >= posA.y - sizeA.y;
    bool collisionZ = posA.z + sizeA.z >= posB.z - sizeB.z && posB.z + sizeB.z >= posA.z - sizeA.z;
    
    if (collisionX && collisionY && collisionZ) {
        // Calculate collision normal
        Vector3 normal = posB - posA;
        normal.normalize();
        
        // Set collision info
        info.normal = normal;
        info.point = (posA + posB) * 0.5f;
        info.depth = 0.1f; // Simple penetration depth
        
        return true;
    }
    
    return false;
}

void CollisionSystem::ResolveCollision(RigidBody* bodyA, RigidBody* bodyB, const CollisionInfo& info) {
    if (!bodyA || !bodyB) {
        return;
    }
    
    // Calculate relative velocity
    Vector3 relativeVelocity = bodyB->velocity - bodyA->velocity;
    
    // Calculate relative velocity along the normal
    float velocityAlongNormal = relativeVelocity.dot(info.normal);
    
    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0) {
        return;
    }
    
    // Calculate restitution (use a default value since RigidBody doesn't have restitution)
    float restitution = 0.2f; // Default restitution value
    
    // Calculate impulse scalar
    float j = -(1.0f + restitution) * velocityAlongNormal;
    j /= (1.0f / bodyA->mass) + (1.0f / bodyB->mass);
    
    // Apply impulse
    Vector3 impulse = info.normal * j;
    bodyA->velocity -= impulse * (1.0f / bodyA->mass);
    bodyB->velocity += impulse * (1.0f / bodyB->mass);
}
