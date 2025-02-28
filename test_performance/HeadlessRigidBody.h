#pragma once
#include "../Vector3.h"

class HeadlessRigidBody {
public:
    Vector3 position;
    Vector3 velocity;
    float mass = 1.0f;
    bool isDynamic = true;
    bool useGravity = true;

    HeadlessRigidBody() : position(0,0,0), velocity(0,0,0) {}
    
    void SetDynamic(bool dynamic) { isDynamic = dynamic; }
    void SetMass(float m) { mass = m; }
    void EnableGravity(bool enable) { useGravity = enable; }
};
