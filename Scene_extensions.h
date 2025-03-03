#ifndef SCENE_EXTENSIONS_H
#define SCENE_EXTENSIONS_H

#include "Scene.h"

// Extension methods for Scene class to support editor functionality
inline const std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects() const {
    return gameObjects;
}

inline size_t Scene::GetGameObjectCount() const {
    return gameObjects.size();
}

inline GameObject* Scene::GetGameObject(size_t index) const {
    if (index < gameObjects.size()) {
        return gameObjects[index].get();
    }
    return nullptr;
}

#endif // SCENE_EXTENSIONS_H
