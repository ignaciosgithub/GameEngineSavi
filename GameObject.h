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
    std::shared_ptr<T> AddComponent();

    template <typename T>
    std::vector<std::shared_ptr<T>> GetComponents();
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
     void Render(const std::vector<PointLight>& lightss);

    void AddChild(GameObject* child);

    void AddMesh(Model* mesh);
    // Destructor to clean up the children
    ~GameObject() ;


    void AddLight(PointLight* light);

    // Other methods for manipulating the game object...
};    