#ifndef ANIMATION_LOADER_H
#define ANIMATION_LOADER_H

#include <string>
#include <memory>
#include "Animation.h"

class AnimationLoader {
public:
    // Load an animation from a .savanim file
    static std::shared_ptr<Animation> LoadFromFile(const std::string& path);
    
    // Save an animation to a .savanim file
    static void SaveToFile(const Animation& anim, const std::string& path);
    
    // Load an animation from a set of OBJ files
    static std::shared_ptr<Animation> LoadFromOBJSequence(
        const std::string& name,
        const std::vector<std::string>& objFilePaths,
        const std::vector<float>& timestamps,
        float duration,
        bool loop = true);
};

#endif // ANIMATION_LOADER_H
