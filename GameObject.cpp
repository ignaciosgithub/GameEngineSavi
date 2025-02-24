#include "GameObject.h"
#include "Model.h"
#include "MonoBehaviourLike.h"

void GameObject::Render(const std::vector<PointLight>& lightss) {
    // Render all meshes
    for (auto& mesh : meshes) {
        if (mesh) {
            mesh->Render(lightss);
        }
    }
    
    // Render all child GameObjects
    for (auto& child : childGameObjects) {
        if (child) {
            child->Render(lightss);
        }
    }
}

void GameObject::AddMesh(Model* mesh) {
    if (mesh) {
        meshes.push_back(mesh);
    }
}

void GameObject::AddLight(PointLight light) {
    lights.push_back(light);
}
