#include "HierarchyPanel.h"
#include "Editor.h"
#include "../GameObject.h"
#include "../Scene.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include <GL/gl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#endif
#include <iostream>

HierarchyPanel::HierarchyPanel(float x, float y, float w, float h, Editor* editor)
    : Panel(x, y, w, h), editor(editor), selectedIndex(-1), scrollOffset(0) {
    
}

void HierarchyPanel::Draw() {
    // Draw panel background
    Panel::Draw();
    
    // Draw panel title
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 15);
    // Draw "Hierarchy" text (simplified for prototype)
    
    // Update game object list
    UpdateGameObjectList();
    
    // Draw game objects
    int visibleItems = static_cast<int>((height - 30) / itemHeight);
    int startIndex = scrollOffset;
    int endIndex = std::min(startIndex + visibleItems, static_cast<int>(gameObjects.size()));
    
    for (int i = startIndex; i < endIndex; i++) {
        float itemY = y + 30 + (i - startIndex) * itemHeight;
        
        // Draw selection highlight
        if (i == selectedIndex) {
            glBegin(GL_QUADS);
            glColor3f(0.7f, 0.8f, 1.0f);
            glVertex2f(x + 2, itemY);
            glVertex2f(x + width - 2, itemY);
            glVertex2f(x + width - 2, itemY + itemHeight);
            glVertex2f(x + 2, itemY + itemHeight);
            glEnd();
        }
        
        // Draw object name
        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(x + 5, itemY + 15);
        // Draw object name text (simplified for prototype)
        // In a real implementation, we would use proper text rendering
        // For now, we'll just draw a placeholder
    }
    
    // Draw scroll bar if needed
    if (gameObjects.size() > visibleItems) {
        float scrollBarHeight = (visibleItems / static_cast<float>(gameObjects.size())) * (height - 30);
        float scrollBarY = y + 30 + (scrollOffset / static_cast<float>(gameObjects.size())) * (height - 30);
        
        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex2f(x + width - 10, scrollBarY);
        glVertex2f(x + width - 2, scrollBarY);
        glVertex2f(x + width - 2, scrollBarY + scrollBarHeight);
        glVertex2f(x + width - 10, scrollBarY + scrollBarHeight);
        glEnd();
    }
}

bool HierarchyPanel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!Panel::HandleInput(mouseX, mouseY, clicked)) {
        return false;
    }
    
    // Check if mouse is over an object
    if (mouseX >= x && mouseX <= x + width - 10) {
        int visibleItems = static_cast<int>((height - 30) / itemHeight);
        int startIndex = scrollOffset;
        int endIndex = std::min(startIndex + visibleItems, static_cast<int>(gameObjects.size()));
        
        for (int i = startIndex; i < endIndex; i++) {
            float itemY = y + 30 + (i - startIndex) * itemHeight;
            
            if (mouseY >= itemY && mouseY <= itemY + itemHeight) {
                if (clicked) {
                    // Select the object
                    SelectObject(i);
                }
                return true;
            }
        }
    }
    
    // Check if mouse is over scroll bar
    if (mouseX >= x + width - 10 && mouseX <= x + width) {
        // Handle scroll bar interaction
        // This would include dragging the scroll bar
        // For now, we'll just handle mouse wheel scrolling
        // in a real implementation
    }
    
    return true;
}

void HierarchyPanel::UpdateGameObjectList() {
    // Get game objects from the scene
    if (editor && editor->GetScene()) {
        // Convert from const std::vector<std::unique_ptr<GameObject>>& to std::vector<GameObject*>
        gameObjects.clear();
        const auto& sceneObjects = editor->GetScene()->GetGameObjects();
        for (const auto& obj : sceneObjects) {
            gameObjects.push_back(obj.get());
        }
    }
}

void HierarchyPanel::SelectObject(int index) {
    if (index >= 0 && index < static_cast<int>(gameObjects.size())) {
        selectedIndex = index;
        
        // Select the object in the editor
        if (editor) {
            editor->SelectObject(gameObjects[index]);
        }
    }
}
