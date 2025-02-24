#include "RigidBody.h"
#include "GameObject.h"
#include <algorithm>

RigidBody::RigidBody() 
    : mass(1.0f), 
      frictionCoeff(0.5f), 
      useGravity(true), 
      isDynamic(true),
      velocity(0, 0, 0), 
      angularVelocity(0, 0, 0), 
      force(0, 0, 0), 
      torque(0, 0, 0),
      gameObject(nullptr) {
}

RigidBody::~RigidBody() {
    // Nothing to clean up
}

void RigidBody::SetMass(float value) {
    // Mass must be positive
    mass = std::max(0.001f, value);
}

float RigidBody::GetMass() const {
    return mass;
}

void RigidBody::SetFrictionCoefficient(float coeff) {
    // Friction coefficient must be non-negative
    frictionCoeff = std::max(0.0f, coeff);
}

float RigidBody::GetFrictionCoefficient() const {
    return frictionCoeff;
}

void RigidBody::EnableGravity(bool enable) {
    useGravity = enable;
}

bool RigidBody::IsGravityEnabled() const {
    return useGravity;
}

void RigidBody::SetDynamic(bool dynamic) {
    isDynamic = dynamic;
}

bool RigidBody::IsDynamic() const {
    return isDynamic;
}

void RigidBody::SetVelocity(const Vector3& vel) {
    velocity = vel;
}

Vector3 RigidBody::GetVelocity() const {
    return velocity;
}

void RigidBody::SetAngularVelocity(const Vector3& angVel) {
    angularVelocity = angVel;
}

Vector3 RigidBody::GetAngularVelocity() const {
    return angularVelocity;
}

void RigidBody::ApplyForce(const Vector3& f) {
    force += f;
}

void RigidBody::ApplyForceAtPoint(const Vector3& f, const Vector3& point) {
    // Apply force
    force += f;
    
    // Calculate torque: τ = r × F
    // Where r is the vector from center of mass to the point of force application
    if (gameObject) {
        Vector3 r = point - gameObject->position;
        
        // Cross product: r × F
        Vector3 t = r.cross(f);
        
        torque += t;
    }
}

void RigidBody::ApplyTorque(const Vector3& t) {
    torque += t;
}

void RigidBody::ClearForces() {
    force = Vector3(0, 0, 0);
    torque = Vector3(0, 0, 0);
}

void RigidBody::PhysicsUpdate(float deltaTime) {
    if (!isDynamic) {
        // Static bodies don't move
        ClearForces();
        return;
    }
    
    // Update linear velocity: v = v + (F/m) * dt
    Vector3 acceleration = force * (1.0f / mass);
    velocity += acceleration * deltaTime;
    
    // Apply friction to slow down movement
    if (velocity.magnitude() > 0.001f) {
        // Simple friction model: Ff = -μ * N * v̂
        // Where N is the normal force (approximated as mass * gravity)
        // and v̂ is the normalized velocity vector
        float normalForce = mass * 9.81f; // Approximation
        Vector3 frictionForce = velocity.normalized() * (-frictionCoeff * normalForce);
        
        // Apply friction to velocity directly
        Vector3 frictionAccel = frictionForce * (1.0f / mass);
        velocity += frictionAccel * deltaTime;
        
        // Ensure friction doesn't reverse direction
        if (velocity.dot(velocity - frictionAccel * deltaTime) < 0) {
            velocity = Vector3(0, 0, 0);
        }
    }
    
    // Update angular velocity: ω = ω + τ * dt
    // (simplified, ignoring moment of inertia tensor)
    angularVelocity += torque * deltaTime;
    
    // Update position and rotation if we have a game object
    if (gameObject) {
        // Update position: p = p + v * dt
        gameObject->position += velocity * deltaTime;
        
        // Update rotation: θ = θ + ω * dt
        gameObject->rotation += angularVelocity * deltaTime;
    }
    
    // Clear forces for next frame
    ClearForces();
}

void RigidBody::OnCollision(RigidBody* other, const CollisionInfo& info) {
    // This is called by the physics system when a collision is detected
    // We can use this to trigger the MonoBehaviourLike collision events
    OnCollisionEnter();
}

void RigidBody::Update() {
    // This is called by the GameObject's Update loop
    // Physics updates are handled by PhysicsSystem
}

void RigidBody::OnCollisionEnter() {
    // Override of MonoBehaviourLike method
    // Called when this body first collides with another
}

void RigidBody::OnCollisionStay() {
    // Override of MonoBehaviourLike method
    // Called while this body is colliding with another
}

void RigidBody::OnCollisionExit() {
    // Override of MonoBehaviourLike method
    // Called when this body stops colliding with another
}

void RigidBody::SetGameObject(GameObject* obj) {
    gameObject = obj;
}

GameObject* RigidBody::GetGameObject() const {
    return gameObject;
}
