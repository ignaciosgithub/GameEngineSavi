#pragma once

#include <vector>
#include <memory>
#include "Vector3.h"
#include "Matrix4x4.h"
#include "PointLight.h"

class GameObject;
class Camera;
class Model;
class ShaderProgram;

class Scene {
public:
    Scene() : frameCount(0), createDefaultObjects(true), isRunning(false), mainCamera(nullptr) {}
    ~Scene() {}
    
    void Initialize() {}
    void CreateDefaultObjects();
    
    // Minimal implementation
    std::vector<PointLight> pointLights;
    int frameCount;
    bool createDefaultObjects;
    bool isRunning;
    Camera* mainCamera;
};
