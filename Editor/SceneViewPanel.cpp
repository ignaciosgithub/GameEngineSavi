#include "SceneViewPanel.h"
#include "Editor.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>

SceneViewPanel::SceneViewPanel(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {
    std::cout << "Creating scene view panel..." << std::endl;
}

SceneViewPanel::~SceneViewPanel() {
    std::cout << "Destroying scene view panel..." << std::endl;
}

void SceneViewPanel::Update(float deltaTime) {
    // Get the editor instance
    Editor* editor = Editor::GetInstance();
    if (!editor) {
        return;
    }
    
    // Get the scene from the editor
    Scene* scene = editor->GetScene();
    if (!scene) {
        return;
    }
    
    // Update scene view logic
    // This is handled by the editor's main update loop
}

void SceneViewPanel::Render() {
    // Get the editor instance
    Editor* editor = Editor::GetInstance();
    if (!editor) {
        return;
    }
    
    // Get the scene from the editor
    Scene* scene = editor->GetScene();
    if (!scene) {
        return;
    }
    
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return;
    }
    
    // Set up viewport for scene view panel
    graphics->SetViewport(x, y, width, height);
    
    // Clear the viewport (depth buffer only)
    graphics->Clear(false, true);
    
    // Get the camera from the editor
    Camera* camera = editor->GetEditorCamera();
    if (!camera) {
        return;
    }
    
    // Set up camera matrices
    // Note: Modern graphics APIs don't use the fixed-function pipeline
    // This is a temporary solution until we implement a proper camera system
    // that works with the graphics API wrapper
    
    // Set perspective projection
    float aspectRatio = (float)width / (float)height;
    
    // Set camera view
    Vector3 pos = camera->GetPosition();
    Vector3 target = pos + camera->lookDirection;
    Vector3 up(0, 1, 0);
    
    // For now, we'll use the legacy OpenGL functions for matrix operations
    // In a future update, these should be replaced with proper matrix operations
    // using the graphics API wrapper
#ifndef PLATFORM_WINDOWS
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Replacement for gluPerspective
    float fovy = 1.0f / tan(45.0f * 0.5f * 3.14159f / 180.0f);
    float zNear = 0.1f;
    float zFar = 1000.0f;
    
    float projMatrix[16] = {
        fovy / aspectRatio, 0, 0, 0,
        0, fovy, 0, 0,
        0, 0, (zFar + zNear) / (zNear - zFar), -1,
        0, 0, (2 * zFar * zNear) / (zNear - zFar), 0
    };
    
    glLoadMatrixf(projMatrix);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Replacement for gluLookAt
    Vector3 forward = target - pos;
    forward.normalize();
    
    Vector3 side = Vector3::Cross(forward, up);
    side.normalize();
    
    Vector3 upVector = Vector3::Cross(side, forward);
    
    float viewMatrix[16] = {
        side.x, upVector.x, -forward.x, 0,
        side.y, upVector.y, -forward.y, 0,
        side.z, upVector.z, -forward.z, 0,
        -Vector3::Dot(side, pos), -Vector3::Dot(upVector, pos), Vector3::Dot(forward, pos), 1
    };
    
    glLoadMatrixf(viewMatrix);
#endif
    
    // Render the scene
    scene->RenderScene();
}

void SceneViewPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}
