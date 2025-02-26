#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "KeyFrame.h"

class Animation {
private:
    std::string name;
    std::vector<KeyFrame> keyframes;
    float duration;
    bool isLooping;
    
    // Cache for interpolated vertices
    mutable std::vector<GLfloat> interpolatedVertices;
    
    // Find the two keyframes to interpolate between
    void FindKeyFramesForTime(float time, KeyFrame** outPrevFrame, KeyFrame** outNextFrame) const;
    
    // Linear interpolation between two vectors of vertices
    void InterpolateVertices(const std::vector<GLfloat>& start, const std::vector<GLfloat>& end, float t);
    
public:
    // Constructor
    Animation(const std::string& name, float duration = 1.0f, bool loop = true);
    
    // Add a keyframe to the animation
    void AddKeyFrame(float time, const std::vector<GLfloat>& vertices);
    
    // Add a keyframe from an OBJ file
    void AddKeyFrameFromOBJ(float time, const std::string& objFilePath);
    
    // Get interpolated vertices at a specific time
    const std::vector<GLfloat>& GetInterpolatedVertices(float time) const;
    
    // Getters and setters
    std::string GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }
    
    float GetDuration() const { return duration; }
    void SetDuration(float newDuration) { duration = newDuration > 0.0f ? newDuration : 1.0f; }
    
    bool IsLooping() const { return isLooping; }
    void SetLooping(bool loop) { isLooping = loop; }
    
    // Get all keyframes
    const std::vector<KeyFrame>& GetKeyFrames() const { return keyframes; }
};

#endif // ANIMATION_H
