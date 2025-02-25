#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <atomic>
#include "GameObject.h"
#include "Camera.h"
#include "Time.h"
#include "PhysicsSystem.h"

class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::unique_ptr<Camera>> cameras;
    std::unique_ptr<Time> time;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::atomic<bool> isRunning;
    unsigned int frameCount = 0; // For debug information

public:
    Scene() : isRunning(false) {}

    void Load();
    void Run();
    void Stop();
    void RenderScene();

    // Add GameObjects and Cameras to the scene
    void AddGameObject(std::unique_ptr<GameObject> gameObject);
    void AddCamera(std::unique_ptr<Camera> camera);
    
    // Physics system management
    void SetPhysicsSystem(std::unique_ptr<PhysicsSystem> system);
    PhysicsSystem* GetPhysicsSystem() const;
};

#endif // SCENE_H
