#include "Animation.h"
#include <algorithm>
#include <iostream>
#include <cmath> // For fmod

Animation::Animation(const std::string& name, float duration, bool loop)
    : name(name), duration(duration), isLooping(loop) {
    // Initialize the interpolated vertices vector
    interpolatedVertices.clear();
}

void Animation::AddKeyFrame(float time, const std::vector<GLfloat>& vertices) {
    // Create a new keyframe
    KeyFrame keyframe(time, vertices);
    
    // Add it to the keyframes vector
    keyframes.push_back(keyframe);
    
    // Sort keyframes by timestamp
    std::sort(keyframes.begin(), keyframes.end(), 
              [](const KeyFrame& a, const KeyFrame& b) { return a.timestamp < b.timestamp; });
}

void Animation::AddKeyFrameFromOBJ(float time, const std::string& objFilePath) {
    // Create a new keyframe with the OBJ file path
    KeyFrame keyframe(time, objFilePath);
    
    // Load the vertex positions from the OBJ file
    if (!keyframe.LoadFromOBJ()) {
        std::cerr << "Failed to load keyframe from OBJ file: " << objFilePath << std::endl;
        return;
    }
    
    // Add it to the keyframes vector
    keyframes.push_back(keyframe);
    
    // Sort keyframes by timestamp
    std::sort(keyframes.begin(), keyframes.end(), 
              [](const KeyFrame& a, const KeyFrame& b) { return a.timestamp < b.timestamp; });
}

void Animation::FindKeyFramesForTime(float time, KeyFrame** outPrevFrame, KeyFrame** outNextFrame) const {
    // Handle empty keyframes vector
    if (keyframes.empty()) {
        *outPrevFrame = nullptr;
        *outNextFrame = nullptr;
        return;
    }
    
    // Handle single keyframe
    if (keyframes.size() == 1) {
        *outPrevFrame = const_cast<KeyFrame*>(&keyframes[0]);
        *outNextFrame = const_cast<KeyFrame*>(&keyframes[0]);
        return;
    }
    
    // Handle looping
    float normalizedTime = time;
    if (isLooping) {
        normalizedTime = fmod(time, duration);
        if (normalizedTime < 0) {
            normalizedTime += duration;
        }
    } else {
        // Clamp time to duration
        normalizedTime = std::max(0.0f, std::min(normalizedTime, duration));
    }
    
    // Find the keyframes to interpolate between
    for (size_t i = 0; i < keyframes.size() - 1; ++i) {
        if (normalizedTime >= keyframes[i].timestamp && normalizedTime <= keyframes[i + 1].timestamp) {
            *outPrevFrame = const_cast<KeyFrame*>(&keyframes[i]);
            *outNextFrame = const_cast<KeyFrame*>(&keyframes[i + 1]);
            return;
        }
    }
    
    // If we get here, time is outside the range of keyframes
    if (normalizedTime < keyframes.front().timestamp) {
        *outPrevFrame = const_cast<KeyFrame*>(&keyframes.front());
        *outNextFrame = const_cast<KeyFrame*>(&keyframes.front());
    } else {
        *outPrevFrame = const_cast<KeyFrame*>(&keyframes.back());
        *outNextFrame = const_cast<KeyFrame*>(&keyframes.back());
    }
}

void Animation::InterpolateVertices(const std::vector<GLfloat>& start, const std::vector<GLfloat>& end, float t) {
    // Ensure both vectors have the same size
    if (start.size() != end.size()) {
        std::cerr << "Cannot interpolate between vertex arrays of different sizes" << std::endl;
        return;
    }
    
    // Resize the interpolated vertices vector if needed
    interpolatedVertices.resize(start.size());
    
    // Perform linear interpolation for each vertex component
    for (size_t i = 0; i < start.size(); ++i) {
        interpolatedVertices[i] = start[i] + t * (end[i] - start[i]);
    }
}

const std::vector<GLfloat>& Animation::GetInterpolatedVertices(float time) const {
    // Find the keyframes to interpolate between
    KeyFrame* prevFrame = nullptr;
    KeyFrame* nextFrame = nullptr;
    FindKeyFramesForTime(time, &prevFrame, &nextFrame);
    
    // If no keyframes found, return an empty vector
    if (!prevFrame || !nextFrame) {
        interpolatedVertices.clear();
        return interpolatedVertices;
    }
    
    // If both keyframes are the same, return the vertices from that keyframe
    if (prevFrame == nextFrame) {
        interpolatedVertices = prevFrame->vertexPositions;
        return interpolatedVertices;
    }
    
    // Calculate the interpolation factor (t)
    float t = (time - prevFrame->timestamp) / (nextFrame->timestamp - prevFrame->timestamp);
    
    // Clamp t to [0, 1]
    t = std::max(0.0f, std::min(t, 1.0f));
    
    // Perform linear interpolation
    const_cast<Animation*>(this)->InterpolateVertices(prevFrame->vertexPositions, nextFrame->vertexPositions, t);
    
    return interpolatedVertices;
}
