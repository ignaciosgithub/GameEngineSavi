#include "RigidBody.h"
#include "GameObject.h"
#include <iostream>
#include <algorithm>

RigidBody::RigidBody() {
    mass = 1.0f;
    useGravity = true;
    isKinematic = false;
    drag = 0.0f;
    angularDrag = 0.05f;
    velocity = Vector3(0, 0, 0);
    angularVelocity = Vector3(0, 0, 0);
    force = Vector3(0, 0, 0);
    torque = Vector3(0, 0, 0);
    gameObject = nullptr;
}

RigidBody::~RigidBody() {
    // Nothing to clean up
}

void RigidBody::SetGameObject(GameObject* obj) {
    gameObject = obj;
}

GameObject* RigidBody::GetGameObject() const {
    return gameObject;
}

void RigidBody::SetMass(float value) {
    mass = std::max(0.0001f, value);
}

float RigidBody::GetMass() const {
    return mass;
}

void RigidBody::SetUseGravity(bool value) {
    useGravity = value;
}

bool RigidBody::GetUseGravity() const {
    return useGravity;
}

void RigidBody::SetIsKinematic(bool value) {
    isKinematic = value;
}

bool RigidBody::GetIsKinematic() const {
    return isKinematic;
}

void RigidBody::SetDrag(float value) {
    drag = std::max(0.0f, value);
}

float RigidBody::GetDrag() const {
    return drag;
}

void RigidBody::SetAngularDrag(float value) {
    angularDrag = std::max(0.0f, value);
}

float RigidBody::GetAngularDrag() const {
    return angularDrag;
}

void RigidBody::SetVelocity(const Vector3& value) {
    velocity = value;
}

Vector3 RigidBody::GetVelocity() const {
    return velocity;
}

void RigidBody::SetAngularVelocity(const Vector3& value) {
    angularVelocity = value;
}

Vector3 RigidBody::GetAngularVelocity() const {
    return angularVelocity;
}

void RigidBody::AddForce(const Vector3& newForce) {
    force += newForce;
}

void RigidBody::AddTorque(const Vector3& newTorque) {
    torque += newTorque;
}

void RigidBody::AddForceAtPosition(const Vector3& newForce, const Vector3& position) {
    // Add force
    force += newForce;
    
    // Calculate torque
    if (gameObject) {
        Vector3 relativePos = position - gameObject->GetPosition();
        Vector3 resultingTorque = relativePos.cross(newForce);
        torque += resultingTorque;
    }
}

void RigidBody::AddRelativeForce(const Vector3& newForce) {
    // TODO: Transform force from local to world space
    force += newForce;
}

void RigidBody::AddRelativeTorque(const Vector3& newTorque) {
    // TODO: Transform torque from local to world space
    torque += newTorque;
}

void RigidBody::OnCollision(RigidBody* other, const CollisionInfo& info) {
    // Handle collision event
    // This is a placeholder for custom collision response
}

void RigidBody::Update(float deltaTime) {
    if (isKinematic || !gameObject) {
        return;
    }
    
    // Apply forces to velocity
    if (mass > 0) {
        velocity += force * (1.0f / mass) * deltaTime;
        
        // Apply drag
        if (drag > 0) {
            float dragFactor = 1.0f - drag * deltaTime;
            dragFactor = std::max(0.0f, dragFactor);
            velocity *= dragFactor;
        }
        
        // Apply angular forces
        angularVelocity += torque * (1.0f / mass) * deltaTime;
        
        // Apply angular drag
        if (angularDrag > 0) {
            float angularDragFactor = 1.0f - angularDrag * deltaTime;
            angularDragFactor = std::max(0.0f, angularDragFactor);
            angularVelocity *= angularDragFactor;
        }
    }
    
    // Update position based on velocity
    Vector3 position = gameObject->GetPosition();
    position += velocity * deltaTime;
    gameObject->SetPosition(position);
    
    // Update rotation based on angular velocity
    Vector3 rotation = gameObject->GetRotation();
    rotation += angularVelocity * deltaTime;
    gameObject->SetRotation(rotation);
    
    // Reset forces and torques
    force = Vector3(0, 0, 0);
    torque = Vector3(0, 0, 0);
}
