#ifndef TRIGGER_VOLUME_H
#define TRIGGER_VOLUME_H

#include "MonoBehaviourLike.h"
#include "GameObject.h"
#include "RigidBody.h"
#include <vector>
#include <algorithm>

class TriggerVolume : public MonoBehaviourLike {
private:
    bool isEnabled;
    std::vector<GameObject*> objectsInTrigger;
    
public:
    TriggerVolume() : isEnabled(true) {}
    ~TriggerVolume() {}
    
    void OnEnable() override { isEnabled = true; }
    void OnDisable() override { isEnabled = false; }
    
    bool IsEnabled() const { return isEnabled; }
    
    // Get all objects currently in the trigger volume
    const std::vector<GameObject*>& GetObjectsInTrigger() const { return objectsInTrigger; }
    
    // Check if a specific object is in the trigger volume
    bool IsObjectInTrigger(GameObject* obj) const {
        return std::find(objectsInTrigger.begin(), objectsInTrigger.end(), obj) != objectsInTrigger.end();
    }
    
    // Called when a collision is detected
    void OnCollision(RigidBody* other, const CollisionInfo& info) {
        if (!isEnabled || !other) return;
        
        GameObject* otherObj = other->GetGameObject();
        if (!otherObj) return;
        
        // Check if this is a new object entering the trigger
        if (!IsObjectInTrigger(otherObj)) {
            objectsInTrigger.push_back(otherObj);
            OnTriggerEnter();
        } else {
            // Object is still in the trigger
            OnTriggerStay();
        }
    }
    
    // Called when a collision ends
    void OnCollisionEnd(RigidBody* other) {
        if (!isEnabled || !other) return;
        
        GameObject* otherObj = other->GetGameObject();
        if (!otherObj) return;
        
        auto it = std::find(objectsInTrigger.begin(), objectsInTrigger.end(), otherObj);
        if (it != objectsInTrigger.end()) {
            objectsInTrigger.erase(it);
            OnTriggerExit();
        }
    }
    
    // Override MonoBehaviourLike trigger events for custom behavior
    void OnTriggerEnter() override {}
    void OnTriggerExit() override {}
    void OnTriggerStay() override {}
};

#endif // TRIGGER_VOLUME_H
