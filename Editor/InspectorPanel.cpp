#include "InspectorPanel.h"
#include "Editor.h"
#include "../GameObject.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include <GL/gl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#endif
#include <iostream>
#include <sstream>

InspectorPanel::InspectorPanel(float x, float y, float w, float h, Editor* editor)
    : Panel(x, y, w, h), editor(editor), selectedObject(nullptr) {
    
    // Create UI elements
    nameField = std::unique_ptr<TextField>(new TextField(x + 10, y + 30, w - 20, 20, "Name"));
    positionField = std::unique_ptr<Vector3Field>(new Vector3Field(x + 10, y + 60, w - 20, 20, "Position"));
    rotationField = std::unique_ptr<Vector3Field>(new Vector3Field(x + 10, y + 90, w - 20, 20, "Rotation"));
    scaleField = std::unique_ptr<Vector3Field>(new Vector3Field(x + 10, y + 120, w - 20, 20, "Scale"));
    
    // Set callbacks
    nameField->SetOnTextChanged([this](const std::string& text) {
        if (selectedObject) {
            selectedObject->SetName(text);
        }
    });
    
    positionField->SetOnValueChanged([this](const Vector3& value) {
        if (selectedObject) {
            selectedObject->SetPosition(value);
        }
    });
    
    rotationField->SetOnValueChanged([this](const Vector3& value) {
        if (selectedObject) {
            selectedObject->SetRotation(value);
        }
    });
    
    scaleField->SetOnValueChanged([this](const Vector3& value) {
        if (selectedObject) {
            selectedObject->SetScale(value);
        }
    });
}

void InspectorPanel::Draw() {
    // Draw panel background
    Panel::Draw();
    
    // Draw panel title
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 15);
    // Draw "Inspector" text (simplified for prototype)
    
    // Draw UI elements if an object is selected
    if (selectedObject) {
        nameField->Draw();
        positionField->Draw();
        rotationField->Draw();
        scaleField->Draw();
    } else {
        // Draw "No object selected" text
        glColor3f(0.5f, 0.5f, 0.5f);
        glRasterPos2f(x + 10, y + 50);
        // Draw "No object selected" text (simplified for prototype)
    }
}

bool InspectorPanel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!Panel::HandleInput(mouseX, mouseY, clicked)) {
        return false;
    }
    
    // Handle UI element input if an object is selected
    if (selectedObject) {
        if (nameField->HandleInput(mouseX, mouseY, clicked)) {
            return true;
        }
        
        if (positionField->HandleInput(mouseX, mouseY, clicked)) {
            return true;
        }
        
        if (rotationField->HandleInput(mouseX, mouseY, clicked)) {
            return true;
        }
        
        if (scaleField->HandleInput(mouseX, mouseY, clicked)) {
            return true;
        }
    }
    
    return true;
}

void InspectorPanel::SetSelectedObject(GameObject* object) {
    selectedObject = object;
    
    // Update UI elements with object properties
    UpdateUIElements();
}

void InspectorPanel::UpdateUIElements() {
    if (selectedObject) {
        nameField->SetText(selectedObject->GetName());
        positionField->SetValue(selectedObject->GetPosition());
        rotationField->SetValue(selectedObject->GetRotation());
        scaleField->SetValue(selectedObject->GetScale());
    }
}

// Vector3Field implementation is now in Vector3Field.cpp
