#include "TextField.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include <GL/gl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#endif
#include <iostream>

void TextField::Draw() {
    if (!visible) return;
    
    // Draw background
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw border
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    
    // Draw label
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x - 50, y + 15);
    // Draw label text (simplified for prototype)
    
    // Draw text
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 15);
    // Draw text (simplified for prototype)
}

bool TextField::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!enabled || !visible) {
        return false;
    }
    
    // Check if mouse is over the text field
    bool isOver = mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
    
    // Handle click
    if (isOver && clicked) {
        isFocused = true;
        return true;
    } else if (clicked) {
        isFocused = false;
    }
    
    return isOver;
}

void TextField::SetText(const std::string& text) {
    this->text = text;
    if (onTextChanged) {
        onTextChanged(text);
    }
}

std::string TextField::GetText() const {
    return text;
}

void TextField::SetOnTextChanged(std::function<void(const std::string&)> callback) {
    onTextChanged = callback;
}
