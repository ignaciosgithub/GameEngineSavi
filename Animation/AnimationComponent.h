#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include <string>
#include <memory>
#include <unordered_map>
#include "../MonoBehaviourLike.h"
#include "Animation.h"

class Model;

class AnimationComponent : public MonoBehaviourLike {
private:
    std::shared_ptr<Animation> currentAnimation;
    float currentTime;
    bool isPlaying;
    Model* targetModel;
    
    // Map of animation names to Animation objects
    std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
    
public:
    // Constructor
    AnimationComponent();
    
    // Add an animation to the component
    void AddAnimation(const std::string& name, std::shared_ptr<Animation> animation);
    
    // Add an animation from a set of OBJ files
    void AddAnimationFromOBJs(const std::string& name, const std::vector<std::string>& objFilePaths, 
                             const std::vector<float>& timestamps, float duration, bool loop = true);
    
    // Play an animation by name
    void Play(const std::string& animName);
    
    // Stop the current animation
    void Stop();
    
    // Pause the current animation
    void Pause();
    
    // Resume the current animation
    void Resume();
    
    // Set the target model to animate
    void SetTargetModel(Model* model);
    
    // Get the target model
    Model* GetTargetModel() const;
    
    // MonoBehaviourLike interface implementation
    void Start() override;
    void Update(float deltaTime) override;
    
    // Get the current animation
    std::shared_ptr<Animation> GetCurrentAnimation() const;
    
    // Get all animations
    const std::unordered_map<std::string, std::shared_ptr<Animation>>& GetAnimations() const;
    
    // Check if an animation is playing
    bool IsPlaying() const;
    
    // Get the current animation time
    float GetCurrentTime() const;
    
    // Set the current animation time
    void SetCurrentTime(float time);
};

#endif // ANIMATION_COMPONENT_H
