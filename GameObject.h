#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "platform.h"
#include <string>
#include <vector>
#include <memory>
#include "Vector3.h"
#include "PointLight.h"
#include "Matrix4x4.h"

// Forward declaration
class Model;
class MonoBehaviourLike;

class GameObject {
private:
    std::string name;
    std::vector<std::shared_ptr<MonoBehaviourLike>> components;
    bool enabled = true;
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    std::vector<PointLight> lights;
    
    // Child GameObjects
    std::vector<GameObject*> childGameObjects;
    
    // Meshes
    std::vector<Model*> meshes;
    
    // Public method to update components
    void UpdateComponents(float deltaTime);
    
    
    // Constructors
    GameObject() : name("GameObject"), position(0,0,0), rotation(0,0,0), size(1,1,1) {}
    
    GameObject(const std::string& name,
               const Vector3& position = Vector3(),
               const Vector3& rotation = Vector3(),
               const Vector3& size = Vector3(1,1,1),
               const std::vector<PointLight>& lights = std::vector<PointLight>())
        : name(name), position(position), rotation(rotation), size(size), lights(lights) {
    }
    
    // Render the GameObject and all its meshes
    void Render(const std::vector<PointLight>& sceneLights);
    
    // Add a component to the GameObject
    template<typename T>
    std::shared_ptr<T> AddComponent(std::shared_ptr<T> component) {
        components.push_back(component);
        return component;
    }
    
    // Add a component by raw pointer
    template<typename T>
    T* AddComponent(T* component) {
        components.push_back(std::shared_ptr<MonoBehaviourLike>(component));
        return component;
    }
    
    // Add a mesh to the GameObject
    void AddMesh(Model* mesh) {
        meshes.push_back(mesh);
    }
    
    // Get all components of a specific type
    template<typename T>
    std::vector<std::shared_ptr<T>> GetComponents() {
        std::vector<std::shared_ptr<T>> result;
        for (auto& component : components) {
            std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(component);
            if (derived) {
                result.push_back(derived);
            }
        }
        return result;
    }
    
    // Add a light to the GameObject
    void AddLight(PointLight light);
    
    // Get the name of the GameObject
    std::string GetName() const { return name; }
    
    // Get position, rotation, and scale
    Vector3 GetPosition() const;
    Vector3 GetRotation() const;
    Vector3 GetScale() const;
    
    // Set position, rotation, and scale
    void SetPosition(const Vector3& pos);
    void SetRotation(const Vector3& rot);
    void SetScale(const Vector3& scale);
    
    // Set name
    void SetName(const std::string& newName);
    
    // Child management
    void AddChild(GameObject* child);
    void RemoveChild(GameObject* child);
    
    // Mesh management
    void RemoveMesh(Model* mesh);
    
    // Component management
    void AddComponent(std::shared_ptr<MonoBehaviourLike> component);
    void RemoveComponent(std::shared_ptr<MonoBehaviourLike> component);
    
    // Editor support methods
    size_t GetMeshCount() const { return meshes.size(); }
    Model* GetMesh(size_t index) const {
        if (index < meshes.size()) {
            return meshes[index];
        }
        return nullptr;
    }
    
    // Missing methods for editor compatibility
    bool IsEnabled() const { return enabled; }
    void SetEnabled(bool enabled);
    Matrix4x4 GetModelMatrix() const;
    std::vector<Model*> GetMeshes() const { return meshes; }
    std::vector<GameObject*> GetChildren() const { return childGameObjects; }
    void Reset();
    void Shutdown();
};

#endif // GAMEOBJECT_H
