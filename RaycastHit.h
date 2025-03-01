#ifndef RAYCAST_HIT_H
#define RAYCAST_HIT_H

#include "Vector3.h"

class GameObject;

// Structure to hold information about a raycast hit
struct RaycastHit {
    GameObject* hitObject;
    Vector3 hitPoint;
    Vector3 hitNormal;
    float distance;
    
    RaycastHit() : hitObject(nullptr), hitPoint(0, 0, 0), hitNormal(0, 1, 0), distance(0) {}
};

#endif // RAYCAST_HIT_H
