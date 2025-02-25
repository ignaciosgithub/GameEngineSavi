#include "GUI.h"
#include <GL/gl.h>

GUIElement::GUIElement(float x, float y, float w, float h)
    : x(x), y(y), width(w), height(h), visible(true), enabled(true) {}

void GUIElement::SetPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

void GUIElement::SetSize(float w, float h) {
    width = w;
    height = h;
}

void GUIElement::SetVisible(bool visible) {
    this->visible = visible;
}

void GUIElement::SetEnabled(bool enabled) {
    this->enabled = enabled;
}

Button::Button(float x, float y, float w, float h, const std::string& text)
    : GUIElement(x, y, w, h), text(text) {}

void Button::Draw() {
    if (!visible) return;
    
    // Draw button background
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw button border
    glBegin(GL_LINE_LOOP);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw text (simplified for prototype)
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + height/2);
    
    // Note: We'll need to handle text rendering differently based on platform
    // For now, we'll use a simplified approach that will be replaced later
    // with proper text rendering
    #ifdef PLATFORM_WINDOWS
    // On Windows, we would use wglUseFontBitmaps or similar
    // This is a placeholder for now
    #else
    // On Linux, we would use XFT or similar
    // This is a placeholder for now
    #endif
    
    // For the prototype, we'll skip actual text rendering
    // as it requires platform-specific code
}

bool Button::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!enabled || !visible) return false;
    
    bool inside = mouseX >= x && mouseX <= x + width &&
                 mouseY >= y && mouseY <= y + height;
                 
    if (inside && clicked && onClick) {
        onClick();
        return true;
    }
    
    return false;
}

void Button::SetOnClick(std::function<void()> callback) {
    onClick = callback;
}

Panel::Panel(float x, float y, float w, float h)
    : GUIElement(x, y, w, h) {}

void Panel::Draw() {
    if (!visible) return;
    
    // Draw panel background
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw children
    for (auto& element : children) {
        element->Draw();
    }
}

bool Panel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!enabled || !visible) return false;
    
    for (auto& element : children) {
        if (element->HandleInput(mouseX, mouseY, clicked)) {
            return true;
        }
    }
    
    return false;
}

void Panel::AddElement(std::unique_ptr<GUIElement> element) {
    children.push_back(std::move(element));
}

GUI::GUI() : visible(true) {}

void GUI::Draw() {
    if (!visible) return;
    
    // Save OpenGL state
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 800, 600, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Draw all elements
    for (auto& element : elements) {
        element->Draw();
    }
    
    // Restore OpenGL state
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glPopAttrib();
}

void GUI::HandleInput(int x, int y, bool clicked) {
    if (!visible) return;
    
    for (auto& element : elements) {
        if (element->HandleInput(x, y, clicked)) {
            break;
        }
    }
}

void GUI::AddElement(std::unique_ptr<GUIElement> element) {
    elements.push_back(std::move(element));
}

void GUI::SetVisible(bool visible) {
    this->visible = visible;
}
