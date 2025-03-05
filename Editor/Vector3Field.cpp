#include "Vector3Field.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#endif
#include <iostream>
#include <sstream>

Vector3Field::Vector3Field(float x, float y, float w, float h, const std::string& label, const Vector3& initialValue)
    : GUIElement(x, y, w, h), label(label), value(initialValue) {
    
    // Create text fields for X, Y, Z components
    float fieldWidth = (width - 40) / 3;
    xField = std::unique_ptr<TextField>(new TextField(x + 20, y, fieldWidth, 20, "X"));
    yField = std::unique_ptr<TextField>(new TextField(x + 20 + fieldWidth + 10, y, fieldWidth, 20, "Y"));
    zField = std::unique_ptr<TextField>(new TextField(x + 20 + 2 * (fieldWidth + 10), y, fieldWidth, 20, "Z"));
    
    // Set initial values
    std::stringstream ss;
    ss << initialValue.x;
    xField->SetText(ss.str());
    ss.str("");
    ss << initialValue.y;
    yField->SetText(ss.str());
    ss.str("");
    ss << initialValue.z;
    zField->SetText(ss.str());
    
    // Set callbacks
    xField->SetOnTextChanged([this](const std::string& text) {
        try {
            value.x = std::stof(text);
            if (onValueChanged) {
                onValueChanged(value);
            }
        } catch (...) {
            // Invalid input, ignore
        }
    });
    
    yField->SetOnTextChanged([this](const std::string& text) {
        try {
            value.y = std::stof(text);
            if (onValueChanged) {
                onValueChanged(value);
            }
        } catch (...) {
            // Invalid input, ignore
        }
    });
    
    zField->SetOnTextChanged([this](const std::string& text) {
        try {
            value.z = std::stof(text);
            if (onValueChanged) {
                onValueChanged(value);
            }
        } catch (...) {
            // Invalid input, ignore
        }
    });
}

void Vector3Field::Draw() {
    if (!visible) return;
    
    // Draw label
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y + 15);
    // Draw label text (simplified for prototype)
    
    // Draw text fields
    xField->Draw();
    yField->Draw();
    zField->Draw();
}

bool Vector3Field::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!enabled || !visible) {
        return false;
    }
    
    // Handle input for text fields
    bool handled = false;
    handled |= xField->HandleInput(mouseX, mouseY, clicked);
    handled |= yField->HandleInput(mouseX, mouseY, clicked);
    handled |= zField->HandleInput(mouseX, mouseY, clicked);
    
    return handled;
}

Vector3 Vector3Field::GetValue() const {
    return value;
}

void Vector3Field::SetValue(const Vector3& value) {
    this->value = value;
    
    // Update text fields
    std::stringstream ss;
    ss << value.x;
    xField->SetText(ss.str());
    ss.str("");
    ss << value.y;
    yField->SetText(ss.str());
    ss.str("");
    ss << value.z;
    zField->SetText(ss.str());
}

void Vector3Field::SetOnValueChanged(std::function<void(const Vector3&)> callback) {
    onValueChanged = callback;
}
