#include "Raycast.h"
#include "GameObject.h"
#include "Model.h"
#include "Scene.h"
#include <algorithm>
#include <limits>
#include <cmath>

std::vector<RaycastHit> Raycast::CastAll(Scene* scene) const {
    std::vector<RaycastHit> hits;
    
    // Normalize the direction
    Vector3 normalizedDirection = direction.normalized();
    
    if (!scene) {
        return hits;
    }
    
    // Since we can't directly access all game objects in the scene,
    // we'll use a simplified approach for the demo
    // In a real implementation, the Scene class would provide a method to get all game objects
    
    // For now, we'll just check a few common object names
    const std::vector<std::string> commonObjectNames = {
        "Floor", "Obstacle1", "Obstacle2", "AIEntity", "Wall", "Player"
    };
    
    for (const auto& name : commonObjectNames) {
        GameObject* gameObject = scene->FindGameObject(name);
        if (gameObject) {
            RaycastHit hit;
            if (CheckIntersection(gameObject, hit)) {
                hits.push_back(hit);
            }
        }
    }
    
    // Sort hits by distance
    std::sort(hits.begin(), hits.end(), [](const RaycastHit& a, const RaycastHit& b) {
        return a.distance < b.distance;
    });
    
    return hits;
}

bool Raycast::Cast(RaycastHit& hit) const {
    std::vector<RaycastHit> hits = CastAll();
    
    if (hits.empty()) {
        return false;
    }
    
    hit = hits[0];
    return true;
}

bool Raycast::CheckIntersection(GameObject* gameObject, RaycastHit& hit) const {
    if (!gameObject) {
        return false;
    }
    
    // Check all meshes of the game object
    for (auto& mesh : gameObject->meshes) {
        if (!mesh) {
            continue;
        }
        
        // This is a simplified approach; in a real implementation, you would use a more sophisticated
        // algorithm to check for intersection with the mesh
        
        // For now, we'll just check if the ray intersects with the bounding sphere of the mesh
        Vector3 center = mesh->position;
        float radius = 1.0f; // Simplified radius
        
        Vector3 oc = center - start;
        float a = direction.dot(direction);
        float b = 2.0f * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant < 0) {
            continue;
        }
        
        float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        
        if (t < 0) {
            continue;
        }
        
        // Check if the intersection is in the direction of the ray
        Vector3 hitPoint = start + direction * t;
        Vector3 hitDirection = hitPoint - start;
        
        if (hitDirection.dot(direction) <= 0) {
            continue;
        }
        
        hit.hitObject = gameObject;
        hit.hitPoint = hitPoint;
        hit.hitNormal = (hitPoint - center).normalized();
        hit.distance = t;
        
        return true;
    }
    
    return false;
}
#include "Raycast.h"
#include "GameObject.h"
#include "Model.h"
#include "Scene.h"
#include <algorithm>
#include <limits>
#include <cmath>

std::vector<RaycastHit> Raycast::CastAll() const {
    std::vector<RaycastHit> hits;
    
    // Normalize the direction
    Vector3 normalizedDirection = direction.normalized();
    
    // Get the current scene
    // This is a simplified approach; in a real implementation, you would need to get the scene from somewhere
    Scene* scene = nullptr; // TODO: Get the current scene
    
    if (!scene) {
        return hits;
    }
    
    // Check all game objects in the scene
    // This is a simplified approach; in a real implementation, you would use spatial partitioning
    // to optimize the ray casting process
    for (size_t i = 0; i < scene->gameObjects.size(); ++i) {
        GameObject* gameObject = scene->gameObjects[i].get();
        
        RaycastHit hit;
        if (CheckIntersection(gameObject, hit)) {
            hits.push_back(hit);
        }
    }
    
    // Sort hits by distance
    std::sort(hits.begin(), hits.end(), [](const RaycastHit& a, const RaycastHit& b) {
        return a.distance < b.distance;
    });
    
    return hits;
}

bool Raycast::Cast(RaycastHit& hit, Scene* scene) const {
    std::vector<RaycastHit> hits = CastAll(scene);
    
    if (hits.empty()) {
        return false;
    }
    
    hit = hits[0];
    return true;
}

bool Raycast::CheckIntersection(GameObject* gameObject, RaycastHit& hit) const {
    if (!gameObject) {
        return false;
    }
    
    // Check all meshes of the game object
    for (auto& mesh : gameObject->meshes) {
        if (!mesh) {
            continue;
        }
        
        // This is a simplified approach; in a real implementation, you would use a more sophisticated
        // algorithm to check for intersection with the mesh
        
        // For now, we'll just check if the ray intersects with the bounding sphere of the mesh
        Vector3 center = mesh->position;
        float radius = 1.0f; // Simplified radius
        
        Vector3 oc = center - start;
        float a = direction.dot(direction);
        float b = 2.0f * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant < 0) {
            continue;
        }
        
        float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        
        if (t < 0) {
            continue;
        }
        
        // Check if the intersection is in the direction of the ray
        Vector3 hitPoint = start + direction * t;
        Vector3 hitDirection = hitPoint - start;
        
        if (hitDirection.dot(direction) <= 0) {
            continue;
        }
        
        hit.hitObject = gameObject;
        hit.hitPoint = hitPoint;
        hit.hitNormal = (hitPoint - center).normalized();
        hit.distance = t;
        
        return true;
    }
    
    return false;
}
