#include "TextField.h"
#include "GUIElement.h"

// GUIElement constructor implementation
GUIElement::GUIElement(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {
}

// TextField constructor implementation
TextField::TextField(float x, float y, float width, float height, 
                    const std::string& label, const std::string& defaultText)
    : GUIElement(x, y, width, height), label(label), text(defaultText) {
}

// TextField methods
void TextField::SetText(const std::string& newText) {
    text = newText;
    if (onTextChanged) {
        onTextChanged(text);
    }
}

void TextField::SetOnTextChanged(std::function<void(const std::string&)> callback) {
    onTextChanged = callback;
}
