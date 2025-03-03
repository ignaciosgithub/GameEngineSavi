#include "Raycast.h"
#include "Scene.h"
#include "GameObject.h"
#include "RaycastHit.h"
#include <iostream>
#include <vector>
#include <algorithm>

bool Raycast::Cast(RaycastHit& hit, Scene* scene) const {
    if (!scene) {
        return false;
    }
    
    // Get all game objects in the scene
    std::vector<GameObject*> gameObjects;
    // Since we don't have access to the scene's game objects directly,
    // we'll use a simplified approach for this implementation
    
    // For each game object, check if it has a collider
    for (auto& gameObject : gameObjects) {
        if (!gameObject) {
            continue;
        }
        
        // Check if the ray intersects with the game object
        if (CheckIntersection(gameObject, hit)) {
            return true;
        }
    }
    
    return false;
}

std::vector<RaycastHit> Raycast::CastAll(Scene* scene) const {
    std::vector<RaycastHit> hits;
    
    if (!scene) {
        return hits;
    }
    
    // Get all game objects in the scene
    std::vector<GameObject*> gameObjects;
    // Since we don't have access to the scene's game objects directly,
    // we'll use a simplified approach for this implementation
    
    // For each game object, check if it has a collider
    for (auto& gameObject : gameObjects) {
        if (!gameObject) {
            continue;
        }
        
        // Check if the ray intersects with the game object
        RaycastHit hit;
        if (CheckIntersection(gameObject, hit)) {
            hits.push_back(hit);
        }
    }
    
    // Sort the hits by distance
    std::sort(hits.begin(), hits.end(), [](const RaycastHit& a, const RaycastHit& b) {
        return a.distance < b.distance;
    });
    
    return hits;
}

bool Raycast::CheckIntersection(GameObject* gameObject, RaycastHit& hit) const {
    if (!gameObject) {
        return false;
    }
    
    // Check if the ray intersects with the game object
    Vector3 toGameObject = gameObject->GetPosition() - start;
    float distanceAlongRay = toGameObject.dot(direction);
    
    // If the game object is behind the ray, it doesn't intersect
    if (distanceAlongRay < 0) {
        return false;
    }
    
    // Calculate the closest point on the ray to the game object
    Vector3 closestPoint = start + direction * distanceAlongRay;
    
    // Calculate the distance from the closest point to the game object
    float distance = (closestPoint - gameObject->GetPosition()).magnitude();
    
    // If the distance is less than the game object's "radius", it intersects
    float radius = 1.0f; // Assume all game objects have a radius of 1.0f
    
    if (distance <= radius) {
        // Fill in the hit information
        hit.gameObject = gameObject;
        hit.point = closestPoint;
        hit.distance = distanceAlongRay;
        hit.normal = (closestPoint - gameObject->GetPosition()).normalized();
        
        return true;
    }
    
    return false;
}
