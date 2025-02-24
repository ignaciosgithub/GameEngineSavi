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

    // Other methods for manipulating the game object...
};  