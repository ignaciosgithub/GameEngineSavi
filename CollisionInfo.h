#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "Vector3.h"

// Structure to hold information about a collision
struct CollisionInfo {
    Vector3 point;      // Point of contact
    Vector3 normal;     // Collision normal (points from B to A)
    float depth;        // Penetration depth
    
    CollisionInfo() : point(0, 0, 0), normal(0, 1, 0), depth(0) {}
};

#endif // COLLISION_INFO_H
