#ifndef SCENE_VIEW_PANEL_H
#define SCENE_VIEW_PANEL_H

#include "../GUI/GUI.h"
#include "../Vector3.h"
#include <iostream>

// Forward declarations
class Editor;

// Scene view panel that displays the 3D scene
class SceneViewPanel : public Panel {
private:
    Editor* editor;
    
    // Camera control
    bool isDraggingCamera;
    int lastMouseX;
    int lastMouseY;
    
    // Object manipulation
    bool isDraggingObject;
    Vector3 dragStartPosition;
    
public:
    SceneViewPanel(float x, float y, float w, float h, Editor* editor);
    
    // Draw the panel
    void Draw() override;
    
    // Handle input
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    
    // Debug rendering state
    void DebugRenderState();
    
    // Handle camera movement
    void HandleCameraMovement(int mouseX, int mouseY);
    
    // Handle object manipulation
    void HandleObjectManipulation(int mouseX, int mouseY);
    
    // Helper function to calculate matrix determinant
    float CalculateMatrixDeterminant(const Matrix4x4& matrix);
};

#endif // SCENE_VIEW_PANEL_H
