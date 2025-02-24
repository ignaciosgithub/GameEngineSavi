#include "CollisionSystem.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "Triangle.h"
#include <algorithm>
#include <limits>

// Ensure std::min and std::max are used instead of Windows macros
#ifdef PLATFORM_WINDOWS
#undef min
#undef max
#endif

CollisionSystem::CollisionSystem() {
}

CollisionSystem::~CollisionSystem() {
}

bool CollisionSystem::CheckCollision(const RigidBody* bodyA, const RigidBody* bodyB, CollisionInfo& info) {
    // Get the game objects associated with the rigid bodies
    GameObject* gameObjectA = bodyA->GetGameObject();
    GameObject* gameObjectB = bodyB->GetGameObject();
    
    if (!gameObjectA || !gameObjectB) {
        return false;
    }
    
    // Check if both objects have models
    if (!gameObjectA->meshes.empty() && !gameObjectB->meshes.empty()) {
        // For simplicity, we'll just check the first mesh of each object
        Model* modelA = gameObjectA->meshes[0];
        Model* modelB = gameObjectB->meshes[0];
        
        if (modelA && modelB) {
            return CheckModelCollision(modelA, modelB, info);
        }
    }
    
    // TODO: Add support for Pyramid collision detection
    
    return false;
}

void CollisionSystem::ResolveCollision(RigidBody* bodyA, RigidBody* bodyB, const CollisionInfo& info) {
    // Skip if either body is not dynamic
    if (!bodyA->IsDynamic() && !bodyB->IsDynamic()) {
        return;
    }
    
    // Calculate relative velocity at the point of collision
    Vector3 relativeVelocity = bodyB->GetVelocity() - bodyA->GetVelocity();
    
    // Calculate relative velocity along the normal
    float velocityAlongNormal = relativeVelocity.dot(info.normal);
    
    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0) {
        return;
    }
    
    // Calculate restitution (bounciness)
    float restitution = 0.2f; // A value between 0 and 1
    
    // Calculate impulse scalar
    float j = -(1.0f + restitution) * velocityAlongNormal;
    
    // Calculate effective mass
    float effectiveMassA = bodyA->IsDynamic() ? 1.0f / bodyA->GetMass() : 0.0f;
    float effectiveMassB = bodyB->IsDynamic() ? 1.0f / bodyB->GetMass() : 0.0f;
    j /= (effectiveMassA + effectiveMassB);
    
    // Apply impulse
    Vector3 impulse = info.normal * j;
    
    if (bodyA->IsDynamic()) {
        bodyA->SetVelocity(bodyA->GetVelocity() - impulse * (1.0f / bodyA->GetMass()));
    }
    
    if (bodyB->IsDynamic()) {
        bodyB->SetVelocity(bodyB->GetVelocity() + impulse * (1.0f / bodyB->GetMass()));
    }
    
    // Apply friction
    // Calculate tangent vector
    Vector3 tangent = relativeVelocity - (info.normal * velocityAlongNormal);
    if (tangent.magnitude() > 0.0001f) {
        tangent = tangent.normalized();
        
        // Calculate friction impulse
        float frictionImpulse = -relativeVelocity.dot(tangent);
        frictionImpulse /= (effectiveMassA + effectiveMassB);
        
        // Use the friction coefficients of both bodies (multiplicative)
        float combinedFriction = bodyA->GetFrictionCoefficient() * bodyB->GetFrictionCoefficient();
        
        // Clamp friction impulse
        Vector3 frictionVector = tangent * frictionImpulse * combinedFriction;
        
        // Apply friction impulse
        if (bodyA->IsDynamic()) {
            bodyA->SetVelocity(bodyA->GetVelocity() - frictionVector * (1.0f / bodyA->GetMass()));
        }
        
        if (bodyB->IsDynamic()) {
            bodyB->SetVelocity(bodyB->GetVelocity() + frictionVector * (1.0f / bodyB->GetMass()));
        }
    }
    
    // Apply torque from collision
    if (bodyA->IsDynamic()) {
        // Calculate torque: τ = r × F
        Vector3 r = info.point - bodyA->GetGameObject()->position;
        Vector3 torque;
        torque.x = r.y * impulse.z - r.z * impulse.y;
        torque.y = r.z * impulse.x - r.x * impulse.z;
        torque.z = r.x * impulse.y - r.y * impulse.x;
        
        bodyA->ApplyTorque(torque);
    }
    
    if (bodyB->IsDynamic()) {
        // Calculate torque: τ = r × F
        Vector3 r = info.point - bodyB->GetGameObject()->position;
        Vector3 torque;
        torque.x = r.y * -impulse.z - r.z * -impulse.y;
        torque.y = r.z * -impulse.x - r.x * -impulse.z;
        torque.z = r.x * -impulse.y - r.y * -impulse.x;
        
        bodyB->ApplyTorque(torque);
    }
    
    // Positional correction to prevent sinking
    const float percent = 0.2f; // Penetration percentage to correct
    const float slop = 0.01f;   // Penetration allowance
    
    // Use parentheses around std::max to avoid Windows macro conflicts
    Vector3 correction = info.normal * ((std::max)(info.depth - slop, 0.0f) / (effectiveMassA + effectiveMassB) * percent);
    
    if (bodyA->IsDynamic()) {
        GameObject* gameObjectA = bodyA->GetGameObject();
        gameObjectA->position = gameObjectA->position - correction * effectiveMassA;
    }
    
    if (bodyB->IsDynamic()) {
        GameObject* gameObjectB = bodyB->GetGameObject();
        gameObjectB->position = gameObjectB->position + correction * effectiveMassB;
    }
}

bool CollisionSystem::CheckModelCollision(const Model* modelA, const Model* modelB, CollisionInfo& info) {
    // First, do a quick bounding box check
    BoundingBox boxA = CalculateBoundingBox(modelA);
    BoundingBox boxB = CalculateBoundingBox(modelB);
    
    if (!CheckBoundingBoxCollision(boxA, boxB)) {
        return false;
    }
    
    // For a simple implementation, we'll use a sphere-based collision detection
    // Calculate the center of each model
    Vector3 centerA = (boxA.min + boxA.max) * 0.5f;
    Vector3 centerB = (boxB.min + boxB.max) * 0.5f;
    
    // Calculate the radius of each model (half the diagonal)
    float radiusA = (boxA.max - boxA.min).magnitude() * 0.5f;
    float radiusB = (boxB.max - boxB.min).magnitude() * 0.5f;
    
    // Check if the spheres overlap
    float distance = (centerB - centerA).magnitude();
    float minDistance = radiusA + radiusB;
    
    if (distance < minDistance) {
        // Collision detected
        info.point = centerA + (centerB - centerA) * (radiusA / minDistance);
        info.normal = (centerB - centerA).normalized();
        info.depth = minDistance - distance;
        return true;
    }
    
    return false;
}

bool CollisionSystem::CheckPyramidCollision(const Pyramid* pyramidA, const Pyramid* pyramidB, CollisionInfo& info) {
    // Similar to model collision, but using pyramid vertices
    // For simplicity, we'll use bounding box collision for now
    BoundingBox boxA = CalculateBoundingBox(pyramidA);
    BoundingBox boxB = CalculateBoundingBox(pyramidB);
    
    if (!CheckBoundingBoxCollision(boxA, boxB)) {
        return false;
    }
    
    // For a simple implementation, we'll use a sphere-based collision detection
    Vector3 centerA = (boxA.min + boxA.max) * 0.5f;
    Vector3 centerB = (boxB.min + boxB.max) * 0.5f;
    
    float radiusA = (boxA.max - boxA.min).magnitude() * 0.5f;
    float radiusB = (boxB.max - boxB.min).magnitude() * 0.5f;
    
    float distance = (centerB - centerA).magnitude();
    float minDistance = radiusA + radiusB;
    
    if (distance < minDistance) {
        info.point = centerA + (centerB - centerA) * (radiusA / minDistance);
        info.normal = (centerB - centerA).normalized();
        info.depth = minDistance - distance;
        return true;
    }
    
    return false;
}

bool CollisionSystem::CheckModelPyramidCollision(const Model* model, const Pyramid* pyramid, CollisionInfo& info) {
    // Similar to model-model collision, but using model and pyramid vertices
    BoundingBox boxA = CalculateBoundingBox(model);
    BoundingBox boxB = CalculateBoundingBox(pyramid);
    
    if (!CheckBoundingBoxCollision(boxA, boxB)) {
        return false;
    }
    
    // For a simple implementation, we'll use a sphere-based collision detection
    Vector3 centerA = (boxA.min + boxA.max) * 0.5f;
    Vector3 centerB = (boxB.min + boxB.max) * 0.5f;
    
    float radiusA = (boxA.max - boxA.min).magnitude() * 0.5f;
    float radiusB = (boxB.max - boxB.min).magnitude() * 0.5f;
    
    float distance = (centerB - centerA).magnitude();
    float minDistance = radiusA + radiusB;
    
    if (distance < minDistance) {
        info.point = centerA + (centerB - centerA) * (radiusA / minDistance);
        info.normal = (centerB - centerA).normalized();
        info.depth = minDistance - distance;
        return true;
    }
    
    return false;
}

CollisionSystem::BoundingBox CollisionSystem::CalculateBoundingBox(const Model* model) {
    BoundingBox box;
    
    // Initialize with extreme values
    box.min = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    box.max = Vector3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    
    // Iterate through all vertices to find min and max
    for (size_t i = 0; i < model->vertices.size(); i += 3) {
        Vector3 vertex(model->vertices[i], model->vertices[i+1], model->vertices[i+2]);
        
        // Apply model position
        vertex = vertex + model->position;
        
        // Update min and max (use parentheses around std::min/max to avoid Windows macro conflicts)
        box.min.x = (std::min)(box.min.x, vertex.x);
        box.min.y = (std::min)(box.min.y, vertex.y);
        box.min.z = (std::min)(box.min.z, vertex.z);
        
        box.max.x = (std::max)(box.max.x, vertex.x);
        box.max.y = (std::max)(box.max.y, vertex.y);
        box.max.z = (std::max)(box.max.z, vertex.z);
    }
    
    return box;
}

CollisionSystem::BoundingBox CollisionSystem::CalculateBoundingBox(const Pyramid* pyramid) {
    BoundingBox box;
    
    // Initialize with extreme values
    box.min = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    box.max = Vector3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    
    // Iterate through all triangles to find min and max
    // Use explicit types instead of auto references for Windows compatibility
    for (size_t i = 0; i < pyramid->triangles.size(); ++i) {
        const Triangle& triangle = pyramid->triangles[i];
        for (size_t j = 0; j < 3; ++j) {
            const Vector3& vertex = triangle.vertices[j];
            
            // Update min and max (use parentheses around std::min/max to avoid Windows macro conflicts)
            box.min.x = (std::min)(box.min.x, vertex.x);
            box.min.y = (std::min)(box.min.y, vertex.y);
            box.min.z = (std::min)(box.min.z, vertex.z);
            
            box.max.x = (std::max)(box.max.x, vertex.x);
            box.max.y = (std::max)(box.max.y, vertex.y);
            box.max.z = (std::max)(box.max.z, vertex.z);
        }
    }
    
    return box;
}

bool CollisionSystem::CheckBoundingBoxCollision(const BoundingBox& boxA, const BoundingBox& boxB) {
    // Check if the boxes overlap in all three axes
    return (boxA.min.x <= boxB.max.x && boxA.max.x >= boxB.min.x) &&
           (boxA.min.y <= boxB.max.y && boxA.max.y >= boxB.min.y) &&
           (boxA.min.z <= boxB.max.z && boxA.max.z >= boxB.min.z);
}

bool CollisionSystem::TriangleTriangleIntersection(
    const Vector3& a1, const Vector3& b1, const Vector3& c1,
    const Vector3& a2, const Vector3& b2, const Vector3& c2,
    Vector3& intersectionPoint, Vector3& normal, float& depth) {
    
    // This is a simplified implementation
    // In a real engine, you would use a more robust algorithm
    
    // For now, we'll just check if any vertex of one triangle is inside the other
    // This is not accurate but serves as a placeholder
    
    // Calculate normal of first triangle
    Vector3 edge1 = b1 - a1;
    Vector3 edge2 = c1 - a1;
    normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
    normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
    normal.z = edge1.x * edge2.y - edge1.y * edge2.x;
    normal = normal.normalized();
    
    // Calculate distance from second triangle vertices to the plane of the first triangle
    float d = -normal.dot(a1);
    float dist_a = normal.dot(a2) + d;
    float dist_b = normal.dot(b2) + d;
    float dist_c = normal.dot(c2) + d;
    
    // If all vertices are on the same side, no intersection
    if ((dist_a > 0 && dist_b > 0 && dist_c > 0) || 
        (dist_a < 0 && dist_b < 0 && dist_c < 0)) {
        return false;
    }
    
    // For simplicity, we'll just use the average of the vertices as the intersection point
    intersectionPoint = (a1 + b1 + c1 + a2 + b2 + c2) * (1.0f / 6.0f);
    
    // Calculate depth as the minimum distance
    // Use parentheses around std::min to avoid Windows macro conflicts
    depth = (std::min)(std::abs(dist_a), (std::min)(std::abs(dist_b), std::abs(dist_c)));
    
    return true;
}
