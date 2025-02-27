#include "CameraManager.h"
#include <GL/gl.h>
#include <algorithm>
#include <iostream>

CameraManager::CameraManager() 
    : mainCamera(nullptr), 
      minimapCamera(nullptr), 
      resolutionChangeAllowed(false), 
      windowWidth(800), 
      windowHeight(600) {
}

CameraManager::~CameraManager() {
    // We don't own the camera pointers, so we don't delete them
    activeCameras.clear();
}

void CameraManager::AddCamera(Camera* camera) {
    if (!camera) {
        return;
    }
    
    // Check if camera is already in the list
    auto it = std::find(activeCameras.begin(), activeCameras.end(), camera);
    if (it == activeCameras.end() && camera->enabled) {
        activeCameras.push_back(camera);
    }
}

void CameraManager::RemoveCamera(Camera* camera) {
    if (!camera) {
        return;
    }
    
    auto it = std::find(activeCameras.begin(), activeCameras.end(), camera);
    if (it != activeCameras.end()) {
        activeCameras.erase(it);
    }
    
    // If we're removing the main or minimap camera, clear those pointers
    if (mainCamera == camera) {
        mainCamera = nullptr;
    }
    
    if (minimapCamera == camera) {
        minimapCamera = nullptr;
    }
}

void CameraManager::SetMainCamera(Camera* camera) {
    if (!camera) {
        return;
    }
    
    mainCamera = camera;
    
    // Set main camera viewport to full screen by default
    camera->SetViewport(0.0f, 0.0f, 1.0f, 1.0f);
    
    // Make sure the camera is in the active list
    AddCamera(camera);
}

void CameraManager::SetMinimapCamera(Camera* camera) {
    if (!camera) {
        return;
    }
    
    minimapCamera = camera;
    
    // Set minimap camera viewport to bottom left corner by default
    camera->SetViewport(0.0f, 0.0f, 0.25f, 0.25f);
    
    // Make sure the camera is in the active list
    AddCamera(camera);
}

void CameraManager::UpdateViewports(int width, int height) {
    if (width <= 0 || height <= 0) {
        return;
    }
    
    if (!resolutionChangeAllowed) {
        // If resolution changes aren't allowed, we still need to update our internal
        // window size, but we won't actually change the window size
        windowWidth = width;
        windowHeight = height;
        return;
    }
    
    // Update window dimensions
    windowWidth = width;
    windowHeight = height;
    
    // Update aspect ratio for all cameras
    for (auto camera : activeCameras) {
        if (camera && camera->enabled) {
            camera->UpdateAspectRatio(windowWidth, windowHeight);
        }
    }
}

void CameraManager::SetResolutionChangeAllowed(bool allowed) {
    resolutionChangeAllowed = allowed;
}

bool CameraManager::IsResolutionChangeAllowed() const {
    return resolutionChangeAllowed;
}

void CameraManager::RenderFromAllCameras() {
    // First, render from the main camera (if it exists and is enabled)
    if (mainCamera && mainCamera->enabled) {
        // Calculate viewport dimensions in pixels
        int vpX = static_cast<int>(mainCamera->viewportX * windowWidth);
        int vpY = static_cast<int>(mainCamera->viewportY * windowHeight);
        int vpWidth = static_cast<int>(mainCamera->viewportWidth * windowWidth);
        int vpHeight = static_cast<int>(mainCamera->viewportHeight * windowHeight);
        
        // Set viewport
        glViewport(vpX, vpY, vpWidth, vpHeight);
        
        // Render scene from main camera
        // Note: The actual rendering will be done by the Scene class
        // This just sets up the viewport
    }
    
    // Then, render from the minimap camera (if it exists and is enabled)
    if (minimapCamera && minimapCamera->enabled) {
        // Calculate viewport dimensions in pixels
        int vpX = static_cast<int>(minimapCamera->viewportX * windowWidth);
        int vpY = static_cast<int>(minimapCamera->viewportY * windowHeight);
        int vpWidth = static_cast<int>(minimapCamera->viewportWidth * windowWidth);
        int vpHeight = static_cast<int>(minimapCamera->viewportHeight * windowHeight);
        
        // Set viewport
        glViewport(vpX, vpY, vpWidth, vpHeight);
        
        // Render scene from minimap camera
        // Note: The actual rendering will be done by the Scene class
        // This just sets up the viewport
    }
    
    // Finally, render from any other active cameras
    for (auto camera : activeCameras) {
        // Skip main and minimap cameras as they've already been rendered
        if (camera == mainCamera || camera == minimapCamera) {
            continue;
        }
        
        if (camera && camera->enabled) {
            // Calculate viewport dimensions in pixels
            int vpX = static_cast<int>(camera->viewportX * windowWidth);
            int vpY = static_cast<int>(camera->viewportY * windowHeight);
            int vpWidth = static_cast<int>(camera->viewportWidth * windowWidth);
            int vpHeight = static_cast<int>(camera->viewportHeight * windowHeight);
            
            // Set viewport
            glViewport(vpX, vpY, vpWidth, vpHeight);
            
            // Render scene from this camera
            // Note: The actual rendering will be done by the Scene class
            // This just sets up the viewport
        }
    }
}
