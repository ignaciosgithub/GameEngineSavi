#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "platform.h"
#include <string>
#include <vector>
#include <memory>
#include "Vector3.h"
#include "PointLight.h"

// Forward declaration
class Model;
class MonoBehaviourLike;

class GameObject {
private:
    std::string name;
    std::vector<std::shared_ptr<MonoBehaviourLike>> components;

public:
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    std::vector<PointLight> lights;
    
    // Child GameObjects
    std::vector<GameObject*> childGameObjects;
    
    // Meshes
    std::vector<Model*> meshes;
    
    // Constructor
    GameObject(const std::string& name,
               const Vector3& position,
               const Vector3& rotation,
               const Vector3& size,
               const std::vector<PointLight>& lights)
        : name(name), position(position), rotation(rotation), size(size), lights(lights) {
    }
    
    // Render the GameObject and all its meshes
    void Render(const std::vector<PointLight>& lightss);
    
    // Add a component to the GameObject
    template<typename T>
    std::shared_ptr<T> AddComponent(std::shared_ptr<T> component);
    
    // Add a mesh to the GameObject
    void AddMesh(Model* mesh);
    
    // Get all components of a specific type
    template<typename T>
    std::vector<std::shared_ptr<T>> GetComponents();
    
    // Add a light to the GameObject
    void AddLight(PointLight light);
    
    // Get the name of the GameObject
    std::string GetName() const { return name; }
};

// Template implementation
template<typename T>
std::shared_ptr<T> GameObject::AddComponent(std::shared_ptr<T> component) {
    components.push_back(component);
    return component;
}

template<typename T>
std::vector<std::shared_ptr<T>> GameObject::GetComponents() {
    std::vector<std::shared_ptr<T>> result;
    for (auto& component : components) {
        std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(component);
        if (derived) {
            result.push_back(derived);
        }
    }
    return result;
}

#endif // GAMEOBJECT_H
