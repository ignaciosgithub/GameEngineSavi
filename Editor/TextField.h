#pragma once

#include <string>
#include <functional>
#include "GUIElement.h"

class TextField : public GUIElement {
public:
    TextField(float x, float y, float width, float height, 
             const std::string& label, const std::string& defaultText);
    
    void SetText(const std::string& newText);
    void SetOnTextChanged(std::function<void(const std::string&)> callback);
    
private:
    std::string label;
    std::string text;
    std::function<void(const std::string&)> onTextChanged;
};
