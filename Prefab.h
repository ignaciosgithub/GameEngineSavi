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

    void AddChild(GameObject* child);
    void AddMesh(Model* mesh);

    void AddLight(PointLight* light);

    // Other methods for manipulating the game object...
};  