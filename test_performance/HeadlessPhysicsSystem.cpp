#include "HeadlessPhysicsSystem.h"

void HeadlessPhysicsSystem::UpdateVelocities(float deltaTime) {
    for (auto body : bodies) {
        if (!body || !body->isDynamic) continue;
        
        if (body->useGravity) {
            Vector3 force(0, gravity, 0);
            Vector3 acceleration = force / body->mass;
            body->velocity += acceleration * deltaTime;
        }
    }
}

void HeadlessPhysicsSystem::HandleCollisions() {
    for (size_t i = 0; i < bodies.size(); i++) {
        for (size_t j = i + 1; j < bodies.size(); j++) {
            auto body1 = bodies[i];
            auto body2 = bodies[j];
            
            if (!body1 || !body2) continue;
            
            // Simple sphere collision check
            Vector3 delta = body1->position - body2->position;
            float dist = delta.magnitude();
            float minDist = 1.0f; // Sphere radius
            
            if (dist < minDist * 2) {
                HandleCollisionResponse(body1, body2, delta, dist, minDist);
            }
        }
    }
}

void HeadlessPhysicsSystem::HandleCollisionResponse(HeadlessRigidBody* body1, HeadlessRigidBody* body2,
                                                  const Vector3& delta, float dist, float minDist) {
    Vector3 normal = delta.normalized();
    float overlap = minDist * 2 - dist;
    
    if (body1->isDynamic && body2->isDynamic) {
        body1->position += normal * (overlap * 0.5f);
        body2->position -= normal * (overlap * 0.5f);
        
        // Elastic collision
        Vector3 relativeVel = body1->velocity - body2->velocity;
        float velAlongNormal = Vector3::Dot(relativeVel, normal);
        
        if (velAlongNormal > 0) {
            float restitution = 0.5f;
            float j = -(1.0f + restitution) * velAlongNormal;
            j /= 1.0f/body1->mass + 1.0f/body2->mass;
            
            Vector3 impulse = normal * j;
            body1->velocity += impulse / body1->mass;
            body2->velocity -= impulse / body2->mass;
        }
    }
    else if (body1->isDynamic) {
        body1->position += normal * overlap;
        body1->velocity = body1->velocity - normal * 2.0f * Vector3::Dot(body1->velocity, normal);
    }
    else if (body2->isDynamic) {
        body2->position -= normal * overlap;
        body2->velocity = body2->velocity - normal * 2.0f * Vector3::Dot(body2->velocity, normal);
    }
}

void HeadlessPhysicsSystem::UpdatePositions(float deltaTime) {
    for (auto body : bodies) {
        if (!body || !body->isDynamic) continue;
        body->position += body->velocity * deltaTime;
        
        // Ground collision
        if (body->position.y < 0) {
            body->position.y = 0;
            body->velocity.y = -body->velocity.y * 0.5f;
            
            // Apply friction
            body->velocity.x *= 0.9f;
            body->velocity.z *= 0.9f;
        }
    }
}

void HeadlessPhysicsSystem::Update(float deltaTime) {
    UpdateVelocities(deltaTime);
    HandleCollisions();
    UpdatePositions(deltaTime);
}

void HeadlessPhysicsSystem::AddBody(HeadlessRigidBody* body) {
    if (body && std::find(bodies.begin(), bodies.end(), body) == bodies.end()) {
        bodies.push_back(body);
    }
}

void HeadlessPhysicsSystem::RemoveBody(HeadlessRigidBody* body) {
    auto it = std::find(bodies.begin(), bodies.end(), body);
    if (it != bodies.end()) {
        bodies.erase(it);
    }
}
