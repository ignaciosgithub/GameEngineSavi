#ifndef RAYCAST_HIT_H
#define RAYCAST_HIT_H

#include "Vector3.h"

class GameObject;

// Structure to hold information about a raycast hit
struct RaycastHit {
    GameObject* gameObject;
    Vector3 point;
    Vector3 normal;
    float distance;
    
    RaycastHit() : gameObject(nullptr), point(0, 0, 0), normal(0, 1, 0), distance(0) {}
};

#endif // RAYCAST_HIT_H
