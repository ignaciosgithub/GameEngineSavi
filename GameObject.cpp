#include "Matrix4x4.h"
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

// AddDirectionalLight implementation
void GameObject::AddDirectionalLight(DirectionalLight light) {
    directionalLights.push_back(light);
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

// Add missing methods for editor compatibility
// Initialize method removed as per user's instruction
// GameObject is initialized via constructor

// IsEnabled() is defined inline in GameObject.h

Matrix4x4 GameObject::GetModelMatrix() const {
    // Create rotation matrix
    Matrix4x4 rotationMatrix = Matrix4x4::createRotation(rotation.x, rotation.y, rotation.z);
    
    // Create a basic model matrix
    Matrix4x4 model;
    model.identity();
    
    // Apply transformations (scale would be applied here if we had a scale method)
    
    // Apply rotation
    model = rotationMatrix * model;
    
    // Apply translation manually
    model.elements[3][0] = position.x;
    model.elements[3][1] = position.y;
    model.elements[3][2] = position.z;
    
    return model;
}

// GetMeshes() is defined inline in GameObject.h

// GetChildren() is defined inline in GameObject.h

void GameObject::Reset() {
    // Reset components
    for (auto& component : components) {
        // Call OnDisable and then OnEnable to simulate a reset
        component->OnDisable();
        component->OnEnable();
    }
}

void GameObject::Shutdown() {
    // Shutdown components
    for (auto& component : components) {
        component->OnDestroy();
    }
    
    // Clear component list
    components.clear();
}

void GameObject::SetEnabled(bool enabled) {
    this->enabled = enabled;
}
