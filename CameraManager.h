#pragma once

#include <vector>
#include <memory>
#include "Camera.h"

class CameraManager {
public:
    CameraManager();
    ~CameraManager();

    // Camera management
    void SetMainCamera(Camera* camera);
    Camera* GetMainCamera() const;
    
    void SetMinimapCamera(Camera* camera);
    Camera* GetMinimapCamera() const;
    
    void AddCamera(Camera* camera);
    void RemoveCamera(Camera* camera);
    
    // Get all active cameras
    std::vector<Camera*> GetActiveCameras() const;
    
    // Render from all active cameras
    void RenderFromAllCameras();
    
    // Resolution management
    void SetResolutionChangeAllowed(bool allowed);
    bool IsResolutionChangeAllowed() const;
    
    // Update viewports when resolution changes
    void UpdateViewports(int width, int height);
    
    // Window dimensions
    int GetWindowWidth() const;
    int GetWindowHeight() const;

private:
    Camera* mainCamera;
    Camera* minimapCamera;
    std::vector<Camera*> cameras;
    
    bool resolutionChangeAllowed;
    int windowWidth;
    int windowHeight;
    
    // Implementation of viewport setup
    void SetupViewport(Camera* camera);
};
