#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include "GUI.h"
#include <string>
#include <functional>

// Forward declaration of Vector3 for Vector3Field
class Vector3;

class TextField : public GUIElement {
private:
    std::string text;
    std::string label;
    bool isFocused;
    std::function<void(const std::string&)> onTextChanged;

public:
    TextField(float x, float y, float w, float h, const std::string& label = "", const std::string& initialText = "")
        : GUIElement(x, y, w, h), text(initialText), label(label), isFocused(false) {}
    
    void Draw() override;
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    
    void SetText(const std::string& text);
    std::string GetText() const;
    
    void SetOnTextChanged(std::function<void(const std::string&)> callback);
};

#endif // TEXTFIELD_H
