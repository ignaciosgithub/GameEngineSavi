#include "SceneViewPanel.h"
#include "Editor.h"
#include "../Camera.h"
#include "../GameObject.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include <GL/gl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#endif
#include <iostream>

SceneViewPanel::SceneViewPanel(float x, float y, float w, float h, Editor* editor)
    : Panel(x, y, w, h), editor(editor), isDraggingCamera(false), isDraggingObject(false) {
    
}

void SceneViewPanel::Draw() {
    // Draw panel background
    Panel::Draw();
    
    // Draw panel title
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 15);
    // Draw "Scene" text (simplified for prototype)
    
    // Set up viewport for scene rendering
    glViewport(static_cast<int>(x), static_cast<int>(y + 20), static_cast<int>(width), static_cast<int>(height - 20));
    
    // Render the scene
    // In a real implementation, we would render the scene to a texture
    // and then display that texture in the panel
    // For now, we'll just draw a placeholder
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex2f(x, y + 20);
    glVertex2f(x + width, y + 20);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Reset viewport
    glViewport(0, 0, 800, 600);
}

bool SceneViewPanel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!Panel::HandleInput(mouseX, mouseY, clicked)) {
        return false;
    }
    
    // Check if mouse is over the scene view
    if (mouseX >= x && mouseX <= x + width && mouseY >= y + 20 && mouseY <= y + height) {
        if (clicked) {
            // Start camera or object manipulation
            if (editor && editor->GetSelectedObject()) {
                // Start object manipulation
                isDraggingObject = true;
                dragStartPosition = editor->GetSelectedObject()->GetPosition();
            } else {
                // Start camera manipulation
                isDraggingCamera = true;
            }
            
            lastMouseX = mouseX;
            lastMouseY = mouseY;
        } else {
            // Continue camera or object manipulation
            if (isDraggingCamera) {
                HandleCameraMovement(mouseX, mouseY);
            } else if (isDraggingObject) {
                HandleObjectManipulation(mouseX, mouseY);
            }
        }
        
        return true;
    }
    
    // Stop dragging if mouse is released
    if (!clicked) {
        isDraggingCamera = false;
        isDraggingObject = false;
    }
    
    return false;
}

void SceneViewPanel::HandleCameraMovement(int mouseX, int mouseY) {
    if (!editor || !editor->GetEditorCamera()) {
        return;
    }
    
    // Calculate mouse delta
    int deltaX = mouseX - lastMouseX;
    int deltaY = mouseY - lastMouseY;
    
    // Update camera rotation
    // In a real implementation, we would use proper camera controls
    // For now, we'll just update the camera's rotation
    Camera* camera = editor->GetEditorCamera();
    Vector3 rotation = camera->GetRotation();
    rotation.y += deltaX * 0.1f;
    rotation.x += deltaY * 0.1f;
    camera->SetRotation(rotation);
    
    // Update last mouse position
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

void SceneViewPanel::HandleObjectManipulation(int mouseX, int mouseY) {
    if (!editor || !editor->GetSelectedObject()) {
        return;
    }
    
    // Calculate mouse delta
    int deltaX = mouseX - lastMouseX;
    int deltaY = mouseY - lastMouseY;
    
    // Update object position
    // In a real implementation, we would use proper object manipulation
    // For now, we'll just update the object's position
    GameObject* object = editor->GetSelectedObject();
    Vector3 position = object->GetPosition();
    position.x += deltaX * 0.01f;
    position.y -= deltaY * 0.01f;
    object->SetPosition(position);
    
    // Update last mouse position
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}
