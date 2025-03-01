#ifndef RAYCAST_H
#define RAYCAST_H

#include "Vector3.h"
#include "RaycastHit.h"
#include <vector>

class GameObject;

class Raycast
{
public:
    Vector3 start;
    Vector3 direction;
   
    Raycast() : start(0, 0, 0), direction(0, 0, -1) {}
   
    Raycast(Vector3 _start, Vector3 _direction) : start(_start), direction(_direction) {}
    
    // Cast a ray and return all objects hit
    std::vector<RaycastHit> CastAll(Scene* scene) const;
    
    // Cast a ray and return the first object hit
    bool Cast(RaycastHit& hit, Scene* scene) const;
    
private:
    // Helper method to check if a ray intersects with a game object
    bool CheckIntersection(GameObject* gameObject, RaycastHit& hit) const;
};

#endif // RAYCAST_H
