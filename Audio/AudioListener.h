#ifndef AUDIO_LISTENER_H
#define AUDIO_LISTENER_H

#include "../MonoBehaviourLike.h"
#include "../GameObject.h"
#include "../Vector3.h"
#include "AudioSystem.h"

class AudioListener : public MonoBehaviourLike {
private:
    Vector3 position;
    Vector3 velocity;
    bool isEnabled;
    
public:
    AudioListener() : position(0, 0, 0), velocity(0, 0, 0), isEnabled(true) {}
    ~AudioListener() {}
    
    void Start() override {
        // Initialize position to the GameObject's position
        GameObject* gameObject = dynamic_cast<GameObject*>(this);
        if (gameObject) {
            position = gameObject->position;
        }
    }
    
    void Update(float deltaTime) override {
        if (!isEnabled) return;
        
        // Get the GameObject this component is attached to
        GameObject* gameObject = dynamic_cast<GameObject*>(this);
        if (!gameObject) return;
        
        // Update position and calculate velocity
        Vector3 newPosition = gameObject->position;
        
        // Only calculate velocity if deltaTime is non-zero to avoid division by zero
        if (deltaTime > 0.0001f) {
            velocity = (newPosition - position) / deltaTime;
        }
        
        position = newPosition;
        
        // Update audio system listener position
        AudioSystem::GetInstance().SetListenerPosition(position);
        AudioSystem::GetInstance().SetListenerVelocity(velocity);
    }
    
    void OnEnable() override { 
        isEnabled = true; 
    }
    
    void OnDisable() override { 
        isEnabled = false; 
    }
    
    // Getters for position and velocity
    Vector3 GetPosition() const { return position; }
    Vector3 GetVelocity() const { return velocity; }
    bool IsEnabled() const { return isEnabled; }
};

#endif // AUDIO_LISTENER_H
