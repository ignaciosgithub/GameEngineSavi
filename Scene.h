#pragma once

#include <vector>
#include <memory>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "TimeManager.h"
#include "PhysicsSystem.h"
#include "CameraManager.h"
#include "Graphics/Core/IGraphicsAPI.h"

class GameObject;
class Camera;
class Model;
class ShaderProgram;

class Scene {
public:
    // Public variables for easier access
    std::vector<GameObject*> gameObjects;
    Vector3 ambientLight;
    float physicsTimeStep;
    bool createDefaultObjects;
    bool isRunning;
    
    Scene() : physicsTimeStep(1.0f / 60.0f), physicsAccumulator(0.0f), frameCount(0), createDefaultObjects(true), isRunning(false), mainCamera(nullptr), minimapCamera(nullptr) {}
    ~Scene();
    
    void Initialize();
    void CreateDefaultObjects();
    void Render();
    
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);
    GameObject* FindGameObject(const std::string& name) const;
    void AddCamera(Camera* camera);
    
    void Update(float deltaTime);
    
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera() const { return mainCamera; }
    
    void SetPhysicsTimeStep(float timeStep) { physicsTimeStep = timeStep; }
    float GetPhysicsTimeStep() const { return physicsTimeStep; }
    
    void SetGravity(const Vector3& gravity);
    Vector3 GetGravity() const;
    
    void SetCreateDefaultObjects(bool create) { createDefaultObjects = create; }
    bool GetCreateDefaultObjects() const { return createDefaultObjects; }
    
    void SetRunning(bool running) { isRunning = running; }
    bool IsRunning() const { return isRunning; }
    
    void RenderScene();
    void RenderFromCamera(Camera* camera);
    void RenderGameObject(GameObject* gameObject, const Matrix4x4& view, const Matrix4x4& projection, const Vector3& cameraPosition);
    void RenderMesh(Model* mesh, const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection, const Vector3& cameraPosition);
    void DrawDebugAxes();
    
    void SetMinimapCamera(Camera* camera);
    Camera* GetMinimapCamera() const { return minimapCamera; }
    
    void SetResolutionChangeAllowed(bool allowed) { resolutionChangeAllowed = allowed; }
    void UpdateResolution(int width, int height);
    
    void SetGlobalShaderUniforms(ShaderProgram* program);
    void UpdateLightUniforms(ShaderProgram* program);
    
    void Reset();
    void Shutdown();
    
private:
    std::unique_ptr<TimeManager> time;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::unique_ptr<CameraManager> cameraManager;
    
    float physicsAccumulator;
    int frameCount;
    bool resolutionChangeAllowed;
    
    Camera* mainCamera;
    Camera* minimapCamera;
};
