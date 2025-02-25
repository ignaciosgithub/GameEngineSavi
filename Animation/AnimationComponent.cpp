#include "AnimationComponent.h"
#include "../Model.h"
#include <iostream>

AnimationComponent::AnimationComponent()
    : currentTime(0.0f), isPlaying(false), targetModel(nullptr) {
}

void AnimationComponent::AddAnimation(const std::string& name, std::shared_ptr<Animation> animation) {
    animations[name] = animation;
}

void AnimationComponent::AddAnimationFromOBJs(const std::string& name, 
                                             const std::vector<std::string>& objFilePaths, 
                                             const std::vector<float>& timestamps, 
                                             float duration, 
                                             bool loop) {
    // Validate input
    if (objFilePaths.size() != timestamps.size() || objFilePaths.empty()) {
        std::cerr << "Invalid animation data: number of OBJ files must match number of timestamps" << std::endl;
        return;
    }
    
    // Create a new animation
    std::shared_ptr<Animation> animation = std::make_shared<Animation>(name, duration, loop);
    
    // Add keyframes from OBJ files
    for (size_t i = 0; i < objFilePaths.size(); ++i) {
        animation->AddKeyFrameFromOBJ(timestamps[i], objFilePaths[i]);
    }
    
    // Add the animation to the component
    animations[name] = animation;
}

void AnimationComponent::Play(const std::string& animName) {
    // Find the animation
    auto it = animations.find(animName);
    if (it == animations.end()) {
        std::cerr << "Animation not found: " << animName << std::endl;
        return;
    }
    
    // Set the current animation
    currentAnimation = it->second;
    currentTime = 0.0f;
    isPlaying = true;
}

void AnimationComponent::Stop() {
    isPlaying = false;
    currentTime = 0.0f;
    currentAnimation = nullptr;
}

void AnimationComponent::Pause() {
    isPlaying = false;
}

void AnimationComponent::Resume() {
    if (currentAnimation) {
        isPlaying = true;
    }
}

void AnimationComponent::SetTargetModel(Model* model) {
    targetModel = model;
}

Model* AnimationComponent::GetTargetModel() const {
    return targetModel;
}

void AnimationComponent::Start() {
    // Initialize the component
    currentTime = 0.0f;
    isPlaying = false;
}

void AnimationComponent::Update(float deltaTime) {
    // If not playing or no animation is set, do nothing
    if (!isPlaying || !currentAnimation || !targetModel) {
        return;
    }
    
    // Update the current time
    currentTime += deltaTime;
    
    // Get the interpolated vertices for the current time
    const std::vector<GLfloat>& vertices = currentAnimation->GetInterpolatedVertices(currentTime);
    
    // Update the model's vertices
    if (!vertices.empty()) {
        targetModel->UpdateVertices(vertices);
    }
    
    // Check if the animation has finished
    if (!currentAnimation->IsLooping() && currentTime >= currentAnimation->GetDuration()) {
        isPlaying = false;
    }
}

std::shared_ptr<Animation> AnimationComponent::GetCurrentAnimation() const {
    return currentAnimation;
}

const std::unordered_map<std::string, std::shared_ptr<Animation>>& AnimationComponent::GetAnimations() const {
    return animations;
}

bool AnimationComponent::IsPlaying() const {
    return isPlaying;
}

float AnimationComponent::GetCurrentTime() const {
    return currentTime;
}

void AnimationComponent::SetCurrentTime(float time) {
    currentTime = time;
}
