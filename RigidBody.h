#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "MonoBehaviourLike.h"
#include "Vector3.h"
#include "CollisionInfo.h"

class GameObject;

class RigidBody : public MonoBehaviourLike {
private:
    float mass;
    float frictionCoeff;
    bool useGravity;
    bool isDynamic;
    
    Vector3 velocity;
    Vector3 angularVelocity;
    Vector3 force;
    Vector3 torque;
    
    GameObject* gameObject;
    
public:
    RigidBody();
    ~RigidBody();
    
    // Physics properties
    void SetMass(float value);
    float GetMass() const;
    
    void SetFrictionCoefficient(float coeff);
    float GetFrictionCoefficient() const;
    
    void EnableGravity(bool enable);
    bool IsGravityEnabled() const;
    
    void SetDynamic(bool dynamic);
    bool IsDynamic() const;
    
    // Physics state
    void SetVelocity(const Vector3& vel);
    Vector3 GetVelocity() const;
    
    void SetAngularVelocity(const Vector3& angVel);
    Vector3 GetAngularVelocity() const;
    
    // Force application
    void ApplyForce(const Vector3& force);
    void ApplyForceAtPoint(const Vector3& force, const Vector3& point);
    void ApplyTorque(const Vector3& torque);
    
    // Clear accumulated forces
    void ClearForces();
    
    // Physics update (called by PhysicsSystem)
    void PhysicsUpdate(float deltaTime);
    
    // Collision callback
    void OnCollision(RigidBody* other, const CollisionInfo& info);
    
    // MonoBehaviourLike overrides
    void Update(float deltaTime) override;
    void OnCollisionEnter() override;
    void OnCollisionStay() override;
    void OnCollisionExit() override;
    
    // GameObject connection
    void SetGameObject(GameObject* obj);
    GameObject* GetGameObject() const;
};

#endif // RIGID_BODY_H
