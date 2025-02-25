#ifndef HIERARCHY_PANEL_H
#define HIERARCHY_PANEL_H

#include "../GUI/GUI.h"
#include <vector>
#include <string>

// Forward declarations
class Editor;
class GameObject;

// Hierarchy panel that displays the scene hierarchy
class HierarchyPanel : public Panel {
private:
    Editor* editor;
    std::vector<GameObject*> gameObjects;
    int selectedIndex;
    
    // Scroll position
    int scrollOffset;
    
    // Item height
    const int itemHeight = 20;
    
public:
    HierarchyPanel(float x, float y, float w, float h, Editor* editor);
    
    // Draw the panel
    void Draw() override;
    
    // Handle input
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    
    // Update the game object list
    void UpdateGameObjectList();
    
    // Select an object by index
    void SelectObject(int index);
};

#endif // HIERARCHY_PANEL_H
