#include "HierarchyPanel.h"
#include "../GameObject.h"
#include "../Scene.h"
#include "Editor.h"
#include <iostream>
#include <algorithm>

HierarchyPanel::HierarchyPanel(int x, int y, int width, int height)
    : Panel(static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)), 
      selectedGameObject(nullptr), draggingObject(false), renamingObject(false), dragStartY(0) {
    std::cout << "Creating hierarchy panel..." << std::endl;
}

HierarchyPanel::~HierarchyPanel() {
    std::cout << "Destroying hierarchy panel..." << std::endl;
}

void HierarchyPanel::Update(float deltaTime) {
    // Update logic for animations or other time-dependent behavior
}

void HierarchyPanel::Draw() {
    Scene* scene = GetScene();
    if (!scene) {
        return;
    }
    
    Panel::Draw();
    
    std::cout << "Rendering Hierarchy Panel" << std::endl;
    
    // Draw each game object in the hierarchy
    for (auto gameObject : gameObjects) {
        DrawGameObjectHierarchy(gameObject, 0);
    }
}

void HierarchyPanel::DrawGameObjectHierarchy(GameObject* gameObject, int indentLevel) {
    if (!gameObject) {
        return;
    }
    
    float yPos = y + 20 + (indentLevel * 10);
    
    if (gameObject == selectedGameObject) {
        glColor4f(0.2f, 0.5f, 0.8f, 0.5f); // Blue highlight for selected object
        glBegin(GL_QUADS);
        glVertex2f(x + 5, yPos - 5);
        glVertex2f(x + width - 5, yPos - 5);
        glVertex2f(x + width - 5, yPos + 10);
        glVertex2f(x + 5, yPos + 10);
        glEnd();
        std::cout << "Drawing selection highlight for: " << gameObject->GetName() << std::endl;
    }
    
    for (int i = 0; i < indentLevel; i++) {
        std::cout << "  ";
    }
    
    float textX = x + 10 + (indentLevel * 10);
    
    if (renamingObject && gameObject == selectedGameObject) {
        Panel::RenderText(renameBuffer, textX, yPos, 1.0f, Vector3(1.0f, 1.0f, 0.0f));
        std::cout << "Rendering rename field for: " << renameBuffer << std::endl;
    } else {
        Panel::RenderText(gameObject->GetName(), textX, yPos, 1.0f, Vector3(0.9f, 0.9f, 0.9f));
        std::cout << "Rendering object name: " << gameObject->GetName() << std::endl;
    }
    
    for (auto& child : gameObject->GetChildren()) {
        if (child) {
            DrawGameObjectHierarchy(child, indentLevel + 1);
        }
    }
}

bool HierarchyPanel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (mouseX < x || mouseX > x + width || mouseY < y || mouseY > y + height) {
        return false;
    }
    
    Scene* scene = GetScene();
    if (!scene) {
        return false;
    }
    
    if (draggingObject) {
        if (clicked) {
            int dragDelta = mouseY - dragStartY;
            if (abs(dragDelta) > 10) {
                
                dragStartY = mouseY;
            }
        } else {
            draggingObject = false;
        }
        return true;
    }
    
    if (renamingObject) {
        if (clicked) {
            if (selectedGameObject) {
                RenameGameObject(selectedGameObject, renameBuffer);
            }
            renamingObject = false;
        }
        return true;
    }
    
    if (clicked) {
        int objectIndex = (mouseY - y - 20) / 20; // Simplified calculation
        
        if (objectIndex >= 0 && objectIndex < gameObjects.size()) {
            selectedGameObject = gameObjects[objectIndex];
            
            static int lastClickTime = 0;
            int currentTime = 0; // In a real implementation, this would be the current time
            if (currentTime - lastClickTime < 300) { // Double-click threshold
                renamingObject = true;
                renameBuffer = selectedGameObject->GetName();
            }
            lastClickTime = currentTime;
            
            draggingObject = true;
            dragStartY = mouseY;
        } else {
            selectedGameObject = nullptr;
        }
        return true;
    }
    
    return false;
}

void HierarchyPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}

void HierarchyPanel::AddGameObject(GameObject* gameObject) {
    if (gameObject && std::find(gameObjects.begin(), gameObjects.end(), gameObject) == gameObjects.end()) {
        gameObjects.push_back(gameObject);
    }
}

void HierarchyPanel::RemoveGameObject(GameObject* gameObject) {
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
        
        if (selectedGameObject == gameObject) {
            selectedGameObject = nullptr;
        }
    }
}

void HierarchyPanel::RenameGameObject(GameObject* gameObject, const std::string& newName) {
    if (gameObject) {
        std::cout << "Renamed game object to: " << newName << std::endl;
    }
}

GameObject* HierarchyPanel::GetSelectedGameObject() const {
    return selectedGameObject;
}

Scene* HierarchyPanel::GetScene() const {
    return Editor::GetInstance()->GetScene();
}
void HierarchyPanel::UpdateFromScene() {
    Scene* scene = GetScene();
    if (!scene) {
        return;
    }
    
    gameObjects.clear();
    
    for (auto& gameObject : scene->gameObjects) {
        AddGameObject(gameObject);
    }
}
