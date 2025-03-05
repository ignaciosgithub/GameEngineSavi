#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#include <GL/glu.h>
#include <windows.h>
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
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <algorithm>
#include"Model.h"
#include"MonobEhaviourLike.h"
#include"GameObject.h"
class Prefab
{
public:
    std::vector<GameObject*> childGameObjects;
    std::string name;

    std::vector<Model*> meshes;
    Model* mainMesh;
    std::vector<PointLight*> lights;

    Prefab()
        : name("Unnamed"), mainMesh(nullptr)
    {
    }

    void AddChild(GameObject* child)
    {
        childGameObjects.push_back(child);
    }

    void AddMesh(Model* mesh)
    {
        if (mainMesh == nullptr)
            mainMesh = mesh;
        meshes.push_back(mesh);
    }

    void AddLight(PointLight* light)
    {
        lights.push_back(light);
    }

    // Game object manipulation methods
    void Move(const Vector3& position)
    {
        for (auto& child : childGameObjects) {
            child->transform.position += position;
        }
    }

    void Rotate(const Vector3& rotation)
    {
        for (auto& child : childGameObjects) {
            child->transform.rotation += rotation;
        }
    }

    void Resize(const Vector3& scale)
    {
        for (auto& child : childGameObjects) {
            child->transform.scale *= scale;
        }
    }

    GameObject* Instantiate(const Vector3& position, const Vector3& rotation = Vector3(0, 0, 0), const Vector3& scale = Vector3(1, 1, 1))
    {
        GameObject* newObject = new GameObject();
        newObject->transform.position = position;
        newObject->transform.rotation = rotation;
        newObject->transform.scale = scale;
        
        // Copy meshes
        for (auto mesh : meshes) {
            newObject->AddMesh(mesh);
        }
        
        // Copy lights
        for (auto light : lights) {
            PointLight* newLight = new PointLight(*light);
            newObject->AddLight(newLight);
        }
        
        return newObject;
    }

    void Destroy(GameObject* gameObject)
    {
        // Remove from child list
        auto it = std::find(childGameObjects.begin(), childGameObjects.end(), gameObject);
        if (it != childGameObjects.end()) {
            childGameObjects.erase(it);
            delete gameObject;
        }
    }
};        