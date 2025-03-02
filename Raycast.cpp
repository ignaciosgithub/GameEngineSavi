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
    
    bool hitSomething = false;
    float closestDistance = std::numeric_limits<float>::max();
    
    // Check intersection with each mesh
    for (auto& mesh : gameObject->meshes) {
        if (!mesh) {
            continue;
        }
        
        // For simplicity, we'll just check against a bounding sphere
        // In a real implementation, you'd check against the actual mesh geometry
        
        // Calculate bounding sphere center and radius
        Vector3 center = gameObject->position; // Use GameObject position instead of mesh->position
        float radius = 1.0f; // Simplified - would be calculated from mesh vertices
        
        // Check ray-sphere intersection
        Vector3 oc = center - start;
        float a = direction.dot(direction);
        float b = 2.0f * oc.dot(direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        
        if (discriminant >= 0) {
            float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
            
            // Only consider positive t values (in front of the ray)
            if (t > 0 && t < closestDistance) {
                closestDistance = t;
                hit.distance = t;
                hit.point = start + direction * t;
                hit.normal = (hit.point - center).normalized();
                hit.gameObject = gameObject;
                hitSomething = true;
            }
        }
    }
    
    // Check intersection with child game objects
    for (auto& child : gameObject->childGameObjects) {
        RaycastHit childHit;
        if (CheckIntersection(child, childHit) && childHit.distance < closestDistance) {
            hit = childHit;
            hitSomething = true;
            closestDistance = childHit.distance;
        }
    }
    
    return hitSomething;
}
