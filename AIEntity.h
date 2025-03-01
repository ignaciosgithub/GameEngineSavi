#ifndef AI_ENTITY_H
#define AI_ENTITY_H

#include "MonoBehaviourLike.h"
#include "Vector3.h"

class GameObject;

class AIEntity : public MonoBehaviourLike {
private:
    Vector3 aiPosition;
    Vector3 aiOrientation;
    float maxAngleDiff;
    float maxDist;
    GameObject* gameObject;
    
public:
    AIEntity();
    ~AIEntity();
    
    // Set the AI position
    void SetAIPosition(const Vector3& position);
    
    // Get the AI position
    Vector3 GetAIPosition() const;
    
    // Set the AI orientation
    void SetAIOrientation(const Vector3& orientation);
    
    // Get the AI orientation
    Vector3 GetAIOrientation() const;
    
    // Set the maximum angle difference for navigation
    void SetMaxAngleDiff(float angle);
    
    // Get the maximum angle difference for navigation
    float GetMaxAngleDiff() const;
    
    // Set the maximum distance for navigation
    void SetMaxDist(float distance);
    
    // Get the maximum distance for navigation
    float GetMaxDist() const;
    
    // Set the game object associated with this AI entity
    void SetGameObject(GameObject* obj);
    
    // Get the game object associated with this AI entity
    GameObject* GetGameObject() const;
    
    // MonoBehaviourLike overrides
    void Update(float deltaTime) override;
};

#endif // AI_ENTITY_H
