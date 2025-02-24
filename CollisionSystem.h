#ifndef COLLISION_SYSTEM_H
#define COLLISION_SYSTEM_H

#include "CollisionInfo.h"
#include "Model.h"
#include "Pyramid.h"

class RigidBody;

class CollisionSystem {
public:
    CollisionSystem();
    ~CollisionSystem();
    
    // Check for collision between two models
    bool CheckCollision(const RigidBody* bodyA, const RigidBody* bodyB, CollisionInfo& info);
    
    // Resolve collision between two rigid bodies
    void ResolveCollision(RigidBody* bodyA, RigidBody* bodyB, const CollisionInfo& info);
    
private:
    // Helper methods for collision detection
    bool CheckModelCollision(const Model* modelA, const Model* modelB, CollisionInfo& info);
    bool CheckPyramidCollision(const Pyramid* pyramidA, const Pyramid* pyramidB, CollisionInfo& info);
    bool CheckModelPyramidCollision(const Model* model, const Pyramid* pyramid, CollisionInfo& info);
    
    // Triangle-triangle intersection test
    bool TriangleTriangleIntersection(
        const Vector3& a1, const Vector3& b1, const Vector3& c1,
        const Vector3& a2, const Vector3& b2, const Vector3& c2,
        Vector3& intersectionPoint, Vector3& normal, float& depth);
    
    // Bounding box collision (for broad phase)
    struct BoundingBox {
        Vector3 min;
        Vector3 max;
    };
    
    BoundingBox CalculateBoundingBox(const Model* model);
    BoundingBox CalculateBoundingBox(const Pyramid* pyramid);
    bool CheckBoundingBoxCollision(const BoundingBox& boxA, const BoundingBox& boxB);
};

#endif // COLLISION_SYSTEM_H
