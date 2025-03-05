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
    
    // Create projection and view matrices using the graphics API wrapper
    float fovy = 45.0f; // Field of view in degrees
    float zNear = 0.1f;
    float zFar = 1000.0f;
    
    // Create projection matrix
    Matrix4x4 projMatrix;
    projMatrix.SetPerspective(fovy, aspectRatio, zNear, zFar);
    
    // Create view matrix
    Vector3 forward = target - pos;
    forward.normalize();
    
    Vector3 side = Vector3::Cross(forward, up);
    side.normalize();
    
    Vector3 upVector = Vector3::Cross(side, forward);
    
    Matrix4x4 viewMatrix;
    viewMatrix.SetLookAt(pos, target, up);
    
    // Set matrices using the graphics API
    graphics->SetProjectionMatrix(projMatrix);
    graphics->SetViewMatrix(viewMatrix);
    
    // If the graphics API doesn't support matrix operations directly,
    // we would need to fall back to legacy OpenGL functions
    // But for now, we'll just use the graphics API wrapper
    if (!graphics->SupportsMatrixOperations()) {
        // This is handled by the graphics API wrapper
        std::cout << "Warning: Graphics API does not support matrix operations directly." << std::endl;
        std::cout << "Using graphics API wrapper for matrix operations." << std::endl;
    }
    
    // Render the scene
    scene->RenderScene();
}

void SceneViewPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}
