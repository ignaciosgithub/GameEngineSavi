#include "GameObject.h"
#include "Model.h"
#include "MonoBehaviourLike.h"
#include <algorithm>

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

// AddLight implementation
void GameObject::AddLight(PointLight light) {
    lights.push_back(light);
}

void GameObject::UpdateComponents(float deltaTime) {
    // Update components
    for (auto& component : components) {
        component->Update(deltaTime);
    }
}

Vector3 GameObject::GetPosition() const {
    return position;
}

Vector3 GameObject::GetRotation() const {
    return rotation;
}

Vector3 GameObject::GetScale() const {
    return size;
}

void GameObject::RemoveMesh(Model* mesh) {
    if (!mesh) {
        return;
    }
    
    auto it = std::find(meshes.begin(), meshes.end(), mesh);
    if (it != meshes.end()) {
        meshes.erase(it);
    }
}

void GameObject::AddComponent(std::shared_ptr<MonoBehaviourLike> component) {
    if (component) {
        components.push_back(component);
    }
}

void GameObject::RemoveComponent(std::shared_ptr<MonoBehaviourLike> component) {
    if (!component) {
        return;
    }
    
    auto it = std::find(components.begin(), components.end(), component);
    if (it != components.end()) {
        components.erase(it);
    }
}

void GameObject::SetPosition(const Vector3& pos) {
    position = pos;
}

void GameObject::SetRotation(const Vector3& rot) {
    rotation = rot;
}

void GameObject::SetScale(const Vector3& scale) {
    size = scale;
}

void GameObject::SetName(const std::string& newName) {
    name = newName;
}

void GameObject::AddChild(GameObject* child) {
    if (child) {
        childGameObjects.push_back(child);
    }
}

void GameObject::RemoveChild(GameObject* child) {
    if (!child) {
        return;
    }
    
    auto it = std::find(childGameObjects.begin(), childGameObjects.end(), child);
    if (it != childGameObjects.end()) {
        childGameObjects.erase(it);
    }
}
