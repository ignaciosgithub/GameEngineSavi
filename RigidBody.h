#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "MonoBehaviourLike.h"
#include "Vector3.h"
#include "CollisionInfo.h"

class GameObject;

class RigidBody : public MonoBehaviourLike {
public:
    // Public properties for easy access
    float mass;
    float frictionCoeff;
    bool useGravity;
    bool isKinematic;
    float drag;
    float angularDrag;
    Vector3 velocity;
    Vector3 angularVelocity;
    Vector3 force;
    Vector3 torque;
    Vector3 inertiaTensor; // Moment of inertia tensor (diagonal components)
    
    // Constructor
    RigidBody();
    
    // Destructor
    virtual ~RigidBody();
    
    // Set the game object this rigid body is attached to
    void SetGameObject(GameObject* obj);
    
    // Get the game object this rigid body is attached to
    GameObject* GetGameObject() const;
    
    // Set mass
    void SetMass(float value);
    
    // Get mass
    float GetMass() const;
    
    // Set whether gravity affects this rigid body
    void SetUseGravity(bool value);
    
    // Get whether gravity affects this rigid body
    bool GetUseGravity() const;
    
    // Set whether this rigid body is kinematic
    void SetIsKinematic(bool value);
    
    // Get whether this rigid body is kinematic
    bool GetIsKinematic() const;
    
    // Set drag
    void SetDrag(float value);
    
    // Get drag
    float GetDrag() const;
    
    // Set angular drag
    void SetAngularDrag(float value);
    
    // Get angular drag
    float GetAngularDrag() const;
    
    // Set velocity
    void SetVelocity(const Vector3& value);
    
    // Get velocity
    Vector3 GetVelocity() const;
    
    // Set angular velocity
    void SetAngularVelocity(const Vector3& value);
    
    // Get angular velocity
    Vector3 GetAngularVelocity() const;
    
    // Add force to the rigid body
    void AddForce(const Vector3& force);
    
    // Add torque to the rigid body
    void AddTorque(const Vector3& torque);
    
    // Add force at a specific position
    void AddForceAtPosition(const Vector3& force, const Vector3& position);
    
    // Add relative force (in local space)
    void AddRelativeForce(const Vector3& force);
    
    // Add relative torque (in local space)
    void AddRelativeTorque(const Vector3& torque);
    
    // Called when this rigid body collides with another
    void OnCollision(RigidBody* other, const CollisionInfo& info);
    
    // Update physics
    virtual void Update(float deltaTime) override;
    
    // For backward compatibility
    void SetFrictionCoefficient(float coeff) { frictionCoeff = coeff; }
    float GetFrictionCoefficient() const { return frictionCoeff; }
    void EnableGravity(bool enable) { useGravity = enable; }
    bool IsGravityEnabled() const { return useGravity; }
    void SetDynamic(bool dynamic) { isKinematic = !dynamic; }
    bool IsDynamic() const { return !isKinematic; }
    void ApplyForce(const Vector3& f) { AddForce(f); }
    void ApplyForceAtPoint(const Vector3& f, const Vector3& point) { AddForceAtPosition(f, point); }
    void ApplyTorque(const Vector3& t) { AddTorque(t); }
    void ClearForces() { force = Vector3(0, 0, 0); torque = Vector3(0, 0, 0); }
    void PhysicsUpdate(float deltaTime) { Update(deltaTime); }
    
    // MonoBehaviourLike overrides
    void OnCollisionEnter() override;
    void OnCollisionStay() override;
    void OnCollisionExit() override;
    
private:
    // The game object this rigid body is attached to
    GameObject* gameObject;
    
    // Calculate moment of inertia based on shape
    void CalculateInertiaTensor();
};

#endif // RIGID_BODY_H
