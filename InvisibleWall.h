#ifndef INVISIBLE_WALL_H
#define INVISIBLE_WALL_H

#include "MonoBehaviourLike.h"
#include "GameObject.h"
#include "RigidBody.h"

class InvisibleWall : public MonoBehaviourLike {
private:
    RigidBody* rigidBody;
    bool isEnabled;
    
public:
    InvisibleWall() : rigidBody(nullptr), isEnabled(true) {}
    ~InvisibleWall() {}
    
    void Start() override {
        // Get the GameObject this component is attached to
        GameObject* gameObject = dynamic_cast<GameObject*>(this);
        if (!gameObject) {
            // This should never happen in normal usage
            return;
        }
        
        // Add a RigidBody component if it doesn't exist
        auto rigidBodies = gameObject->GetComponents<RigidBody>();
        if (rigidBodies.empty()) {
            rigidBody = gameObject->AddComponent(new RigidBody());
        } else {
            rigidBody = rigidBodies[0].get();
        }
        
        // Configure the RigidBody for an invisible wall
        if (rigidBody) {
            rigidBody->SetDynamic(false);  // Static object
            rigidBody->EnableGravity(false);  // No gravity
        }
    }
    
    void OnEnable() override {
        isEnabled = true;
        EnableCollisions(true);
    }
    
    void OnDisable() override {
        isEnabled = false;
        EnableCollisions(false);
    }
    
    bool IsEnabled() const {
        return isEnabled;
    }
    
    void EnableCollisions(bool enable) {
        // This would toggle collision detection for the wall
        // Currently, there's no direct method to disable collisions in RigidBody
        // So we'll implement a workaround
        if (rigidBody) {
            // In a real implementation, we would call something like:
            // rigidBody->EnableCollisions(enable);
            
            // For now, we'll just set the mass to a very high value when disabled
            // to effectively make it "pass through" other objects
            if (enable) {
                rigidBody->SetMass(1000.0f);  // Very high mass for solid wall
            } else {
                rigidBody->SetMass(0.0001f);  // Very low mass to effectively disable collisions
            }
        }
    }
};

#endif // INVISIBLE_WALL_H
