#pragma once

#include <vector>
#include "Camera.h"
#include "ThirdParty/OpenGL/include/GL/platform_gl.h"

class CameraManager {
public:
    CameraManager();
    ~CameraManager();
    
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera() const;
    
    void AddCamera(Camera* camera);
    void RemoveCamera(Camera* camera);
    
    void Update(float deltaTime);
    void SetupViewport(Camera* camera);
    
    // Compatibility methods for Scene.cpp
    void SetMinimapCamera(Camera* camera);
    std::vector<Camera*> GetActiveCameras() const;
    void RenderFromAllCameras();
    void UpdateViewports(int width, int height);
    void SetResolutionChangeAllowed(bool allowed);
    
private:
    Camera* mainCamera;
    std::vector<Camera*> cameras;
    int windowWidth = 800;
    int windowHeight = 600;
};
