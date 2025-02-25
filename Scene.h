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
    float targetFPS = 60.0f;           // Target frames per second
    float physicsTimeStep = 1.0f/60.0f; // Fixed physics timestep (60Hz)
    float physicsAccumulator = 0.0f;    // Accumulator for physics updates

public:
    Scene() : isRunning(false) {}

    void Load();
    void Run();
    void Stop();
    void RenderScene();

    // Add GameObjects and Cameras to the scene
    void AddGameObject(std::unique_ptr<GameObject> gameObject);
    void AddCamera(std::unique_ptr<Camera> camera);
    
    // Frame rate configuration
    void SetTargetFPS(float fps) { targetFPS = fps > 0.0f ? fps : 60.0f; }
    float GetTargetFPS() const { return targetFPS; }
    
    // Physics system management
    void SetPhysicsSystem(std::unique_ptr<PhysicsSystem> system);
    PhysicsSystem* GetPhysicsSystem() const;
};

#endif // SCENE_H
