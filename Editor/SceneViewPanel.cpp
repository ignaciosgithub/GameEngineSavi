#include "SceneViewPanel.h"
#include "Editor.h"
#include "../Scene.h"
#include "../Camera.h"
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
    
    // Set up viewport for scene view panel
    glViewport(x, y, width, height);
    
    // Clear the viewport
    glClear(GL_DEPTH_BUFFER_BIT);
    
    // Get the camera from the editor
    Camera* camera = editor->GetEditorCamera();
    if (!camera) {
        return;
    }
    
    // Set up camera matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    // Set perspective projection
    float aspectRatio = (float)width / (float)height;
    gluPerspective(45.0f, aspectRatio, 0.1f, 1000.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set camera view
    Vector3 pos = camera->GetPosition();
    Vector3 target = pos + camera->lookDirection;
    Vector3 up(0, 1, 0);
    
    gluLookAt(
        pos.x, pos.y, pos.z,
        target.x, target.y, target.z,
        up.x, up.y, up.z
    );
    
    // Render the scene
    scene->RenderScene();
}

void SceneViewPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}
