#include "GUI.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#endif
#include <iostream>

GUIElement::GUIElement(float x, float y, float w, float h)
    : x(x), y(y), width(w), height(h), visible(true), enabled(true) {
}

void GUIElement::SetPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void GUIElement::SetSize(float w, float h) {
    this->width = w;
    this->height = h;
}

void GUIElement::SetVisible(bool visible) {
    this->visible = visible;
}

void GUIElement::SetEnabled(bool enabled) {
    this->enabled = enabled;
}

Button::Button(float x, float y, float w, float h, const std::string& text)
    : GUIElement(x, y, w, h), text(text) {
}

void Button::Draw() {
    if (!visible) return;
    
    // Draw button background
    glColor3f(0.7f, 0.7f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw button border
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw button text
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + height / 2);
    // Draw text (simplified for prototype)
}

bool Button::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!enabled || !visible) {
        return false;
    }
    
    // Check if mouse is over the button
    bool isOver = mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
    
    // Handle click
    if (isOver && clicked && onClick) {
        onClick();
        return true;
    }
    
    return isOver;
}

void Button::SetOnClick(std::function<void()> callback) {
    onClick = callback;
}

Panel::Panel(float x, float y, float w, float h)
    : GUIElement(x, y, w, h) {
}

void Panel::Draw() {
    if (!visible) return;
    
    // Draw panel background
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw panel border
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Enable scissor test for text clipping
    glEnable(GL_SCISSOR_TEST);
    glScissor(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height));
    
    // Draw children
    for (auto& child : children) {
        child->Draw();
    }
    
    // Disable scissor test
    glDisable(GL_SCISSOR_TEST);
}

bool Panel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!enabled || !visible) {
        return false;
    }
    
    // Check if mouse is over the panel
    bool isOver = mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
    
    // If mouse is over the panel, check if any child handles the input
    if (isOver) {
        for (auto& child : children) {
            if (child->HandleInput(mouseX, mouseY, clicked)) {
                return true;
            }
        }
    }
    
    return isOver;
}

void Panel::AddElement(GUIElement* element) {
    children.push_back(std::unique_ptr<GUIElement>(element));
}

void Panel::AddElement(std::unique_ptr<GUIElement> element) {
    children.push_back(std::move(element));
}

GUI::GUI() : visible(true) {
}

void GUI::Draw() {
    if (!visible) return;
    
    for (auto& element : elements) {
        element->Draw();
    }
}

void GUI::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!visible) return;
    
    for (auto& element : elements) {
        if (element->HandleInput(mouseX, mouseY, clicked)) {
            break;
        }
    }
}

void GUI::AddElement(GUIElement* element) {
    elements.push_back(std::unique_ptr<GUIElement>(element));
}

void GUI::AddElement(std::unique_ptr<GUIElement> element) {
    elements.push_back(std::move(element));
}

void GUI::SetVisible(bool visible) {
    this->visible = visible;
}
