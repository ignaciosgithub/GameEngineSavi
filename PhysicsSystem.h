#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>
#include "Vector3.h"

// Forward declarations
class RigidBody;
class CollisionSystem;

class PhysicsSystem {
private:
    float gravity;
    std::vector<RigidBody*> bodies;
    CollisionSystem* collisionSystem;
    
public:
    PhysicsSystem();
    ~PhysicsSystem();
    
    void Update(float deltaTime);
    void AddBody(RigidBody* body);
    void RemoveBody(RigidBody* body);
    
    // Gravity control
    void SetGravity(float value);
    float GetGravity() const;
    
    // Collision system access
    void SetCollisionSystem(CollisionSystem* system);
    CollisionSystem* GetCollisionSystem() const;
};

#endif // PHYSICS_SYSTEM_H
