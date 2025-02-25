#ifndef VECTOR3FIELD_H
#define VECTOR3FIELD_H

#include "../GUI/GUI.h"
#include "../Vector3.h"
#include "../GUI/TextField.h"
#include <memory>
#include <functional>
#include <string>

// Vector3 field for editing Vector3 properties
class Vector3Field : public GUIElement {
private:
    std::string label;
    Vector3 value;
    std::function<void(const Vector3&)> onValueChanged;
    
    // UI elements
    std::unique_ptr<TextField> xField;
    std::unique_ptr<TextField> yField;
    std::unique_ptr<TextField> zField;
    
public:
    Vector3Field(float x, float y, float w, float h, const std::string& label, const Vector3& initialValue = Vector3());
    
    // Draw the field
    void Draw() override;
    
    // Handle input
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    
    // Get the value
    Vector3 GetValue() const;
    
    // Set the value
    void SetValue(const Vector3& value);
    
    // Set the callback for value changes
    void SetOnValueChanged(std::function<void(const Vector3&)> callback);
};

#endif // VECTOR3FIELD_H
