#ifndef GAMEOBJECT_EXTENSIONS_H
#define GAMEOBJECT_EXTENSIONS_H

#include "GameObject.h"

// Extension methods for GameObject class to support editor functionality
inline size_t GameObject::GetMeshCount() const {
    return meshes.size();
}

inline Model* GameObject::GetMesh(size_t index) const {
    if (index < meshes.size()) {
        return meshes[index];
    }
    return nullptr;
}

#endif // GAMEOBJECT_EXTENSIONS_H
