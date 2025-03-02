#include "RigidBody.h"
#include "GameObject.h"
#include <algorithm>

RigidBody::RigidBody() 
    : mass(1.0f), 
      frictionCoeff(0.5f), 
      useGravity(true), 
      isKinematic(false),
      drag(0.0f),
      angularDrag(0.05f),
      velocity(0, 0, 0), 
      angularVelocity(0, 0, 0), 
      force(0, 0, 0), 
      torque(0, 0, 0),
      inertiaTensor(1.0f, 1.0f, 1.0f),
      gameObject(nullptr) {
    CalculateInertiaTensor();
}

RigidBody::~RigidBody() {
    // Nothing to clean up
}

void RigidBody::SetMass(float value) {
    // Mass must be positive
    mass = std::max(0.001f, value);
    CalculateInertiaTensor();
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
    // Transform force from local to world space
    if (gameObject) {
        // Simple approximation - in a real implementation, we would use a proper
        // transformation matrix based on the game object's rotation
        Vector3 worldForce = newForce; // Placeholder for transformation
        force += worldForce;
    } else {
        force += newForce;
    }
}

void RigidBody::AddRelativeTorque(const Vector3& newTorque) {
    // Transform torque from local to world space
    if (gameObject) {
        // Simple approximation - in a real implementation, we would use a proper
        // transformation matrix based on the game object's rotation
        Vector3 worldTorque = newTorque; // Placeholder for transformation
        torque += worldTorque;
    } else {
        torque += newTorque;
    }
}

void RigidBody::OnCollision(RigidBody* other, const CollisionInfo& info) {
    // This is called by the physics system when a collision is detected
    // We can use this to trigger the MonoBehaviourLike collision events
    OnCollisionEnter();
}

void RigidBody::Update(float deltaTime) {
    if (isKinematic || !gameObject) {
        ClearForces();
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
        
        // Apply angular forces using inertia tensor
        Vector3 angularAcceleration;
        angularAcceleration.x = torque.x / inertiaTensor.x;
        angularAcceleration.y = torque.y / inertiaTensor.y;
        angularAcceleration.z = torque.z / inertiaTensor.z;
        
        angularVelocity += angularAcceleration * deltaTime;
        
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
    ClearForces();
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
    CalculateInertiaTensor();
}

GameObject* RigidBody::GetGameObject() const {
    return gameObject;
}

void RigidBody::CalculateInertiaTensor() {
    // Validate mass
    if (mass <= 0) {
        mass = 1.0f;  // Prevent division by zero
    }

    // Default to unit sphere if no game object
    if (!gameObject) {
        float sphereInertia = (2.0f/5.0f) * mass;
        inertiaTensor = Vector3(sphereInertia, sphereInertia, sphereInertia);
        return;
    }

    // Get object scale, prevent zero dimensions
    Vector3 size = gameObject->GetScale();
    size.x = std::max(0.01f, size.x);
    size.y = std::max(0.01f, size.y);
    size.z = std::max(0.01f, size.z);

    // Box inertia tensor calculation
    float factor = 1.0f / 12.0f;
    float x2 = size.x * size.x;
    float y2 = size.y * size.y;
    float z2 = size.z * size.z;

    inertiaTensor.x = factor * mass * (y2 + z2);
    inertiaTensor.y = factor * mass * (x2 + z2);
    inertiaTensor.z = factor * mass * (x2 + y2);
}
