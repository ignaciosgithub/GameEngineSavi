#pragma once

#include <vector>
#include <memory>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "TimeManager.h"
#include "PhysicsSystem.h"
#include "CameraManager.h"

class GameObject;
class Camera;
class Model;
class ShaderProgram;

class Scene {
public:
    Scene() : physicsTimeStep(1.0f / 60.0f), physicsAccumulator(0.0f), frameCount(0), createDefaultObjects(true), isRunning(false), mainCamera(nullptr), minimapCamera(nullptr) {}
    ~Scene();
    
    void Initialize();
    void CreateDefaultObjects();
    void Render();
    
    void AddGameObject(GameObject gameObject);
    void RemoveGameObject(GameObject* gameObject);
    GameObject* FindGameObject(const std::string& name) const;
    void AddCamera(Camera* camera);
    void AddCamera(std::unique_ptr<Camera> camera);
    
    void Update(float deltaTime);
    
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera() const;
    
    void SetPhysicsTimeStep(float timeStep);
    float GetPhysicsTimeStep() const;
    
    void SetGravity(const Vector3& gravity);
    Vector3 GetGravity() const;
    
    void SetCreateDefaultObjects(bool create);
    bool GetCreateDefaultObjects() const;
    
    void SetRunning(bool running);
    bool IsRunning() const;
    
    void RenderScene();
    void RenderFromCamera(Camera* camera);
    void RenderGameObject(GameObject* gameObject, const Matrix4x4& view, const Matrix4x4& projection, const Vector3& cameraPosition);
    void RenderMesh(Model* mesh, const Matrix4x4& model, const Matrix4x4& view, const Matrix4x4& projection, const Vector3& cameraPosition);
    void DrawDebugAxes();
    
    void SetMinimapCamera(Camera* camera);
    Camera* GetMinimapCamera() const;
    
    void SetResolutionChangeAllowed(bool allowed);
    void UpdateResolution(int width, int height);
    
    void SetGlobalShaderUniforms(ShaderProgram* program);
    void UpdateLightUniforms(ShaderProgram* program);
    
    void Reset();
    void Shutdown();
    
private:
    std::vector<GameObject*> gameObjects;
    std::unique_ptr<TimeManager> time;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::unique_ptr<CameraManager> cameraManager;
    
    float physicsTimeStep;
    float physicsAccumulator;
    int frameCount;
    bool createDefaultObjects;
    bool isRunning;
    bool resolutionChangeAllowed;
    
    Camera* mainCamera;
    Camera* minimapCamera;
    
    Vector3 ambientLight;
};
