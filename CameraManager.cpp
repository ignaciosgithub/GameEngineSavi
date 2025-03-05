#include "CameraManager.h"
#include <iostream>
#include "platform.h"

// Platform-specific OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#else
#include <GL/gl.h>
#endif

// Constructor
CameraManager::CameraManager() : mainCamera(nullptr) {
    // Initialize camera manager
}

// Destructor
CameraManager::~CameraManager() {
    // Clean up
    cameras.clear();
    mainCamera = nullptr;
}

// Set the main camera
void CameraManager::SetMainCamera(Camera* camera) {
    if (camera) {
        // Add the camera to the list if it's not already there
        bool found = false;
        for (auto& cam : cameras) {
            if (cam == camera) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            cameras.push_back(camera);
        }
        
        // Set as main camera
        mainCamera = camera;
    }
}

// Get the main camera
Camera* CameraManager::GetMainCamera() const {
    return mainCamera;
}

// Add a camera to the manager
void CameraManager::AddCamera(Camera* camera) {
    if (camera) {
        // Check if the camera is already in the list
        for (auto& cam : cameras) {
            if (cam == camera) {
                return;
            }
        }
        
        // Add the camera to the list
        cameras.push_back(camera);
        
        // If this is the first camera, set it as the main camera
        if (!mainCamera) {
            mainCamera = camera;
        }
    }
}

// Remove a camera from the manager
void CameraManager::RemoveCamera(Camera* camera) {
    if (camera) {
        // Remove the camera from the list
        for (auto it = cameras.begin(); it != cameras.end(); ++it) {
            if (*it == camera) {
                cameras.erase(it);
                break;
            }
        }
        
        // If the main camera was removed, set a new main camera
        if (mainCamera == camera) {
            mainCamera = cameras.empty() ? nullptr : cameras[0];
        }
    }
}

// Update all cameras
void CameraManager::Update(float deltaTime) {
    // Update all cameras
    for (auto& camera : cameras) {
        if (camera && camera->IsEnabled()) {
            camera->Update(deltaTime);
        }
    }
}

// Setup the viewport for a camera
void CameraManager::SetupViewport(Camera* camera) {
    if (!camera) {
        return;
    }
    
    // Get the viewport settings
    float x = camera->GetViewportX();
    float y = camera->GetViewportY();
    float width = camera->GetViewportWidth();
    float height = camera->GetViewportHeight();
    
    // Convert to pixel coordinates
    // Note: We're using a default window size of 800x600 if not specified
    int windowWidth = 800;
    int windowHeight = 600;
    
    int pixelX = static_cast<int>(x * windowWidth);
    int pixelY = static_cast<int>(y * windowHeight);
    int pixelWidth = static_cast<int>(width * windowWidth);
    int pixelHeight = static_cast<int>(height * windowHeight);
    
    // Set the viewport
    glViewport(pixelX, pixelY, pixelWidth, pixelHeight);
}

// Set minimap camera (compatibility method)
void CameraManager::SetMinimapCamera(Camera* camera) {
    // For compatibility with Scene.cpp
    // Just add the camera to the list
    AddCamera(camera);
}

// Get active cameras (compatibility method)
std::vector<Camera*> CameraManager::GetActiveCameras() const {
    // For compatibility with Scene.cpp
    return cameras;
}

// Render from all cameras (compatibility method)
void CameraManager::RenderFromAllCameras() {
    // For compatibility with Scene.cpp
    // This would normally render from all cameras, but we'll just use the main camera
    if (mainCamera) {
        SetupViewport(mainCamera);
    }
}

// Update viewports (compatibility method)
void CameraManager::UpdateViewports(int width, int height) {
    // For compatibility with Scene.cpp
    // This would normally update all camera viewports, but we'll just use default values
    windowWidth = width;
    windowHeight = height;
}

// Set resolution change allowed (compatibility method)
void CameraManager::SetResolutionChangeAllowed(bool allowed) {
    // For compatibility with Scene.cpp
    // This would normally set whether resolution changes are allowed
    // We'll just ignore this for now
}
