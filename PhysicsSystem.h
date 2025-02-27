#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "Vector3.h"
#include <vector>

// Forward declarations
class RigidBody;
class CollisionSystem;

class PhysicsSystem {
private:
  float gravity;
  float fixedTimeStep;
  bool enableCollisions;
  float globalRestitution;
  std::vector<RigidBody *> bodies;
  CollisionSystem *collisionSystem;

public:
  PhysicsSystem();
  ~PhysicsSystem();

  // Initialize physics system with settings from ProjectSettings
  void Initialize();

  void Update(float deltaTime);
  void AddBody(RigidBody *body);
  void RemoveBody(RigidBody *body);

  // Gravity control
  void SetGravity(float value);
  float GetGravity() const;

  // Fixed time step control
  void SetFixedTimeStep(float value);
  float GetFixedTimeStep() const;

  // Collision control
  void SetEnableCollisions(bool enable);
  bool GetEnableCollisions() const;

  // Collision elasticity control
  void SetGlobalRestitution(float value);
  float GetGlobalRestitution() const;

  // Collision system access
  void SetCollisionSystem(CollisionSystem *system);
  CollisionSystem *GetCollisionSystem() const;
};

#endif // PHYSICS_SYSTEM_H
