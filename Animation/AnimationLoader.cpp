#include "AnimationLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

// For JSON parsing, we'll use a simple approach since we don't have a JSON library
// In a real implementation, you would use a proper JSON library like nlohmann/json

std::shared_ptr<Animation> AnimationLoader::LoadFromFile(const std::string& path) {
    // Open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open animation file: " << path << std::endl;
        return nullptr;
    }
    
    // Read the entire file into a string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    // Close the file
    file.close();
    
    // Parse the JSON content
    std::string animationName;
    float duration = 1.0f;
    bool isLooping = true;
    std::vector<KeyFrame> keyframes;
    
    // Extract animation name
    size_t namePos = content.find("\"animationName\":");
    if (namePos != std::string::npos) {
        size_t nameStart = content.find("\"", namePos + 16) + 1;
        size_t nameEnd = content.find("\"", nameStart);
        animationName = content.substr(nameStart, nameEnd - nameStart);
    }
    
    // Extract duration
    size_t durationPos = content.find("\"duration\":");
    if (durationPos != std::string::npos) {
        size_t durationStart = durationPos + 11;
        size_t durationEnd = content.find(",", durationStart);
        if (durationEnd == std::string::npos) {
            durationEnd = content.find("}", durationStart);
        }
        std::string durationStr = content.substr(durationStart, durationEnd - durationStart);
        duration = std::stof(durationStr);
    }
    
    // Extract isLooping
    size_t loopingPos = content.find("\"isLooping\":");
    if (loopingPos != std::string::npos) {
        size_t loopingStart = loopingPos + 12;
        size_t loopingEnd = content.find(",", loopingStart);
        if (loopingEnd == std::string::npos) {
            loopingEnd = content.find("}", loopingStart);
        }
        std::string loopingStr = content.substr(loopingStart, loopingEnd - loopingStart);
        isLooping = (loopingStr == "true");
    }
    
    // Extract keyframes
    size_t keyframesPos = content.find("\"keyframes\":");
    if (keyframesPos != std::string::npos) {
        size_t keyframesStart = content.find("[", keyframesPos);
        size_t keyframesEnd = content.find("]", keyframesStart);
        std::string keyframesStr = content.substr(keyframesStart + 1, keyframesEnd - keyframesStart - 1);
        
        // Parse each keyframe
        size_t pos = 0;
        while (pos < keyframesStr.length()) {
            // Find the start of the keyframe object
            size_t keyframeStart = keyframesStr.find("{", pos);
            if (keyframeStart == std::string::npos) {
                break;
            }
            
            // Find the end of the keyframe object
            size_t keyframeEnd = keyframesStr.find("}", keyframeStart);
            if (keyframeEnd == std::string::npos) {
                break;
            }
            
            // Extract the keyframe object
            std::string keyframeStr = keyframesStr.substr(keyframeStart, keyframeEnd - keyframeStart + 1);
            
            // Extract timestamp
            float timestamp = 0.0f;
            size_t timestampPos = keyframeStr.find("\"timestamp\":");
            if (timestampPos != std::string::npos) {
                size_t timestampStart = timestampPos + 12;
                size_t timestampEnd = keyframeStr.find(",", timestampStart);
                if (timestampEnd == std::string::npos) {
                    timestampEnd = keyframeStr.find("}", timestampStart);
                }
                std::string timestampStr = keyframeStr.substr(timestampStart, timestampEnd - timestampStart);
                timestamp = std::stof(timestampStr);
            }
            
            // Extract OBJ file path
            std::string objFilePath;
            size_t objFilePathPos = keyframeStr.find("\"objFilePath\":");
            if (objFilePathPos != std::string::npos) {
                size_t objFilePathStart = keyframeStr.find("\"", objFilePathPos + 14) + 1;
                size_t objFilePathEnd = keyframeStr.find("\"", objFilePathStart);
                objFilePath = keyframeStr.substr(objFilePathStart, objFilePathEnd - objFilePathStart);
                
                // Create a keyframe with the OBJ file path
                KeyFrame keyframe(timestamp, objFilePath);
                keyframes.push_back(keyframe);
            }
            
            // Move to the next keyframe
            pos = keyframeEnd + 1;
        }
    }
    
    // Create the animation
    std::shared_ptr<Animation> animation = std::make_shared<Animation>(animationName, duration, isLooping);
    
    // Add the keyframes to the animation
    for (const auto& keyframe : keyframes) {
        animation->AddKeyFrameFromOBJ(keyframe.timestamp, keyframe.objFilePath);
    }
    
    return animation;
}

void AnimationLoader::SaveToFile(const Animation& anim, const std::string& path) {
    // Open the file
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open animation file for writing: " << path << std::endl;
        return;
    }
    
    // Write the animation data in JSON format
    file << "{\n";
    file << "    \"animationName\": \"" << anim.GetName() << "\",\n";
    file << "    \"duration\": " << anim.GetDuration() << ",\n";
    file << "    \"isLooping\": " << (anim.IsLooping() ? "true" : "false") << ",\n";
    file << "    \"keyframes\": [\n";
    
    // Write the keyframes
    const auto& keyframes = anim.GetKeyFrames();
    for (size_t i = 0; i < keyframes.size(); ++i) {
        file << "        {\n";
        file << "            \"timestamp\": " << keyframes[i].timestamp << ",\n";
        
        // Write the OBJ file path if it exists
        if (!keyframes[i].objFilePath.empty()) {
            file << "            \"objFilePath\": \"" << keyframes[i].objFilePath << "\"\n";
        }
        
        file << "        }";
        
        // Add a comma if this is not the last keyframe
        if (i < keyframes.size() - 1) {
            file << ",";
        }
        
        file << "\n";
    }
    
    file << "    ]\n";
    file << "}\n";
    
    // Close the file
    file.close();
}

std::shared_ptr<Animation> AnimationLoader::LoadFromOBJSequence(
    const std::string& name,
    const std::vector<std::string>& objFilePaths,
    const std::vector<float>& timestamps,
    float duration,
    bool loop) {
    // Validate input
    if (objFilePaths.size() != timestamps.size() || objFilePaths.empty()) {
        std::cerr << "Invalid animation data: number of OBJ files must match number of timestamps" << std::endl;
        return nullptr;
    }
    
    // Create a new animation
    std::shared_ptr<Animation> animation = std::make_shared<Animation>(name, duration, loop);
    
    // Add keyframes from OBJ files
    for (size_t i = 0; i < objFilePaths.size(); ++i) {
        animation->AddKeyFrameFromOBJ(timestamps[i], objFilePaths[i]);
    }
    
    return animation;
}
