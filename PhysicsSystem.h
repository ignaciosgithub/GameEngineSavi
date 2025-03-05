#pragma once
#include "Vector3.h"

class PhysicsSystem {
public:
    PhysicsSystem();
    ~PhysicsSystem();
    
    void Update(float deltaTime);
    void SetGravity(float gravity);
    float GetGravity() const;
};
