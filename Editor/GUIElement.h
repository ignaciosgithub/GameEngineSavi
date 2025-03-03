#pragma once

class GUIElement {
public:
    GUIElement(float x, float y, float width, float height);
    virtual ~GUIElement() = default;
    
    float x, y, width, height;
};
