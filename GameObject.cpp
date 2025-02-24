#include <gl/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <gl/gl.h>
#include "Vector3.h"
#include"Matrix4x4.h"
#include<vector>
#include"Triangle.h"
#include"PointLight.h"
#include"Face.h"
#include <fstream>
#include<string>
#include <sstream>
#include<math.h>
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <memory>
#include"Model.h"
#include"MonobEhaviourLike.h"
class GameObject {
public:
    std::vector<std::shared_ptr<MonoBehaviourLike>> components;
    std::string name;
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
const std::vector<PointLight>& lights;
    //GameObject(const std::string& name = "GameObject")
       // : name(name), position(0, 0, 0), rotation(0, 0, 0), size(1, 1, 1) {}

    template <typename T>
    std::shared_ptr<T> AddComponent() {
        // Ensure that we only add types that are derived from MonoBehaviourLike
        static_assert(std::is_base_of<MonoBehaviourLike, T>::value, "T must derive from MonoBehaviourLike");
        auto component = std::make_shared<T>();
        components.push_back(component);
        return component;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> GetComponents() {
        // Ensure that we only retrieve types that are derived from MonoBehaviourLike
        static_assert(std::is_base_of<MonoBehaviourLike, T>::value, "T must derive from MonoBehaviourLike");
        std::vector<std::shared_ptr<T>> specificComponents;
        for (auto& comp : components) {
            if (auto castComp = std::dynamic_pointer_cast<T>(comp)) {
                specificComponents.push_back(castComp);
            }
        }
        return specificComponents;
    }
    std::vector<GameObject*> childGameObjects;

    std::vector<Model*> meshes;
    Model* mainMesh;
   
    // Standard GameObject constructor
    GameObject(const std::string& name,
               const Vector3& position,
               const Vector3& rotation,
               const Vector3& size,
               const std::vector<PointLight>& lights)
        : name(name), position(position), rotation(rotation), size(size), lights(lights), mainMesh(nullptr)
    {
    }
     void Render(const std::vector<PointLight>& lightss) {
        // Render the main mesh
        if (mainMesh) {
            mainMesh->Render(lightss);
        }

        // Iterate through all the additional meshes and render each
        for (auto& mesh : meshes) {
            mesh->Render(lightss);
        }

        // Recursively render each child's meshes
        for (auto& child : childGameObjects) {
            child->Render(child->lights);
        }
    }

    void AddChild(GameObject* child) {
        childGameObjects.push_back(child);
    }

    void AddMesh(Model* mesh) {
        if (!mainMesh) {
            mainMesh = mesh; // Set the first added mesh as the main mesh
        }
        meshes.push_back(mesh);
    }

    // Destructor to clean up the children
    ~GameObject() {
        for (GameObject* child : childGameObjects) {
            delete child;
        }
    }


    void AddLight(PointLight* light)
    {
        //lights.push_back(light);
    }

    // Other methods for manipulating the game object...
};    