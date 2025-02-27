#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"
#include <vector>
#include <memory>

class CameraManager {
private:
    std::vector<Camera*> activeCameras;
    Camera* mainCamera;
    Camera* minimapCamera;
    bool resolutionChangeAllowed;
    int windowWidth;
    int windowHeight;

public:
    CameraManager();
    ~CameraManager();
    
    void AddCamera(Camera* camera);
    void RemoveCamera(Camera* camera);
    void SetMainCamera(Camera* camera);
    void SetMinimapCamera(Camera* camera);
    void UpdateViewports(int width, int height);
    void SetResolutionChangeAllowed(bool allowed);
    bool IsResolutionChangeAllowed() const;
    void RenderFromAllCameras();
    
    // Get window dimensions
    int GetWindowWidth() const { return windowWidth; }
    int GetWindowHeight() const { return windowHeight; }
    
    // Get cameras
    Camera* GetMainCamera() const { return mainCamera; }
    Camera* GetMinimapCamera() const { return minimapCamera; }
    const std::vector<Camera*>& GetActiveCameras() const { return activeCameras; }
};

#endif // CAMERA_MANAGER_H
