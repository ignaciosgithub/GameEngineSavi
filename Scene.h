#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <atomic>
#include <string>
#include "GameObject.h"
#include "Camera.h"
#include "EngineTime.h"
#include "PhysicsSystem.h"
#include "PointLight.h"
#include "Shaders/Core/ShaderProgram.h"
#include "CameraManager.h"
#include "SceneSerializer.h"

class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::unique_ptr<Camera>> cameras;
    std::vector<PointLight> pointLights;
    std::unique_ptr<EngineTime> time;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::unique_ptr<CameraManager> cameraManager;
    std::atomic<bool> isRunning;
    unsigned int frameCount = 0; // For debug information
    float targetFPS = 60.0f;           // Target frames per second
    float physicsTimeStep = 1.0f/60.0f; // Fixed physics timestep (60Hz)
    float physicsAccumulator = 0.0f;    // Accumulator for physics updates
    Camera* mainCamera = nullptr;       // Main camera for rendering
    Camera* minimapCamera = nullptr;    // Minimap camera for top-down view
    std::string currentScenePath;       // Path to the current scene file
    
    // Private method for cleaning up resources during scene transitions
    void CleanupResources();

public:
    Scene() : isRunning(false) {}

    void Initialize();
    void Load();
    void Run();
    void Stop();
    void Update(float deltaTime);
    void Render();
    void RenderScene();
    
    // Camera management
    void SetMainCamera(Camera* camera);
    void SetMinimapCamera(Camera* camera);
    void EnableCamera(Camera* camera);
    void DisableCamera(Camera* camera);
    void SetResolutionChangeAllowed(bool allowed);
    void UpdateResolution(int width, int height);

    // Add GameObjects and Cameras to the scene
    void AddGameObject(std::unique_ptr<GameObject> gameObject);
    void AddGameObject(GameObject* gameObject);
    void AddCamera(std::unique_ptr<Camera> camera);
    void AddCamera(Camera* camera);
    
    // Light management
    void AddPointLight(const PointLight& light);
    void RemovePointLight(size_t index);
    const std::vector<PointLight>& GetPointLights() const { return pointLights; }
    
    // Shader management
    void SetGlobalShaderUniforms(Shaders::ShaderProgram* program);
    void UpdateLightUniforms(Shaders::ShaderProgram* program);
    
    // Frame rate configuration
    void SetTargetFPS(float fps) { targetFPS = fps > 0.0f ? fps : 60.0f; }
    float GetTargetFPS() const { return targetFPS; }
    
    // Physics system management
    void SetPhysicsSystem(std::unique_ptr<PhysicsSystem> system);
    PhysicsSystem* GetPhysicsSystem() const;
    
    // Camera access
    Camera* GetMainCamera() const { return mainCamera; }
    Camera* GetMinimapCamera() const { return minimapCamera; }
    CameraManager* GetCameraManager() const { return cameraManager.get(); }
    
    // Scene transition methods
    void UnloadScene();
    void LoadScene(const std::string& scenePath);
    void TransferObject(GameObject* obj, const std::string& targetScenePath);
    
    // GameObject finding
    GameObject* FindGameObject(const std::string& name) const;
    
    // Debug rendering
    void DrawDebugAxes();
};

#endif // SCENE_H
