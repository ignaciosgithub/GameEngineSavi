#include <atomic>
class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::unique_ptr<Camera>> cameras;
    std::unique_ptr<Time> time;
    std::atomic<bool> isRunning;

public:
    Scene() : isRunning(false){
	};

    void Load();

    void Run();

    void Stop();
    void RenderScene();

    // Add GameObjects and Cameras to the scene
    void AddGameObject(std::unique_ptr<GameObject> gameObject);
    void AddCamera(std::unique_ptr<Camera> camera);
};