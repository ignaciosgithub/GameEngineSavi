#include "CameraManager.h"
#include <algorithm>
#include <iostream>

CameraManager::CameraManager()
    : mainCamera(nullptr)
    , minimapCamera(nullptr)
    , resolutionChangeAllowed(true)
    , windowWidth(800)
    , windowHeight(600)
{
    std::cout << "CameraManager initialized" << std::endl;
}

CameraManager::~CameraManager()
{
    // Note: We don't own the cameras, just references to them
    mainCamera = nullptr;
    minimapCamera = nullptr;
    cameras.clear();
}

void CameraManager::SetMainCamera(Camera* camera)
{
    mainCamera = camera;
    
    // Add to cameras list if not already present
    if (camera && std::find(cameras.begin(), cameras.end(), camera) == cameras.end()) {
        cameras.push_back(camera);
    }
}

Camera* CameraManager::GetMainCamera() const
{
    return mainCamera;
}

void CameraManager::SetMinimapCamera(Camera* camera)
{
    minimapCamera = camera;
    
    // Add to cameras list if not already present
    if (camera && std::find(cameras.begin(), cameras.end(), camera) == cameras.end()) {
        cameras.push_back(camera);
    }
}

Camera* CameraManager::GetMinimapCamera() const
{
    return minimapCamera;
}

void CameraManager::AddCamera(Camera* camera)
{
    if (camera && std::find(cameras.begin(), cameras.end(), camera) == cameras.end()) {
        cameras.push_back(camera);
    }
}

void CameraManager::RemoveCamera(Camera* camera)
{
    if (camera) {
        cameras.erase(std::remove(cameras.begin(), cameras.end(), camera), cameras.end());
        
        // If removing main or minimap camera, clear those references
        if (mainCamera == camera) {
            mainCamera = nullptr;
        }
        
        if (minimapCamera == camera) {
            minimapCamera = nullptr;
        }
    }
}

std::vector<Camera*> CameraManager::GetActiveCameras() const
{
    std::vector<Camera*> activeCameras;
    
    // Add all enabled cameras
    for (auto camera : cameras) {
        if (camera && camera->IsEnabled()) {
            activeCameras.push_back(camera);
        }
    }
    
    return activeCameras;
}

void CameraManager::RenderFromAllCameras()
{
    // Get all active cameras
    auto activeCameras = GetActiveCameras();
    
    // Render from each camera
    for (auto camera : activeCameras) {
        if (camera) {
            // Set up viewport for this camera
            SetupViewport(camera);
            
            // Render from this camera
            // Note: Actual rendering is handled by the Scene class
        }
    }
}

void CameraManager::SetResolutionChangeAllowed(bool allowed)
{
    resolutionChangeAllowed = allowed;
}

bool CameraManager::IsResolutionChangeAllowed() const
{
    return resolutionChangeAllowed;
}

void CameraManager::UpdateViewports(int width, int height)
{
    // Only update if resolution change is allowed
    if (!resolutionChangeAllowed) {
        std::cout << "Resolution change not allowed" << std::endl;
        return;
    }
    
    // Update window dimensions
    windowWidth = width;
    windowHeight = height;
    
    // Update viewports for all cameras
    for (auto camera : cameras) {
        if (camera) {
            SetupViewport(camera);
        }
    }
    
    std::cout << "Viewports updated to " << width << "x" << height << std::endl;
}

int CameraManager::GetWindowWidth() const
{
    return windowWidth;
}

int CameraManager::GetWindowHeight() const
{
    return windowHeight;
}

void CameraManager::SetupViewport(Camera* camera)
{
    if (!camera) {
        return;
    }
    
    // Get viewport settings from camera
    float x = camera->GetViewportX();
    float y = camera->GetViewportY();
    float width = camera->GetViewportWidth();
    float height = camera->GetViewportHeight();
    
    // Convert normalized coordinates to pixel coordinates
    int pixelX = static_cast<int>(x * windowWidth);
    int pixelY = static_cast<int>(y * windowHeight);
    int pixelWidth = static_cast<int>(width * windowWidth);
    int pixelHeight = static_cast<int>(height * windowHeight);
    
    // Set viewport
    glViewport(pixelX, pixelY, pixelWidth, pixelHeight);
}
