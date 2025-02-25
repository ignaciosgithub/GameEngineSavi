#ifndef INSPECTOR_PANEL_H
#define INSPECTOR_PANEL_H

#include "../GUI/GUI.h"
#include "../GUI/TextField.h"
#include "Vector3Field.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>

// Forward declarations
class Editor;
class GameObject;
class TextField;
class Vector3Field;

// Inspector panel that displays and edits object properties
class InspectorPanel : public Panel {
private:
    Editor* editor;
    GameObject* selectedObject;
    
    // UI elements
    std::unique_ptr<TextField> nameField;
    std::unique_ptr<Vector3Field> positionField;
    std::unique_ptr<Vector3Field> rotationField;
    std::unique_ptr<Vector3Field> scaleField;
    
public:
    InspectorPanel(float x, float y, float w, float h, Editor* editor);
    
    // Draw the panel
    void Draw() override;
    
    // Handle input
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    
    // Set the selected object
    void SetSelectedObject(GameObject* object);
    
    // Update UI elements with object properties
    void UpdateUIElements();
};

// Vector3Field is defined in Vector3Field.h

#endif // INSPECTOR_PANEL_H
