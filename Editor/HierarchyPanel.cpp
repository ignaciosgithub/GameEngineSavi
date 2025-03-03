#include "HierarchyPanel.h"
#include "../GameObject.h"
#include <iostream>

HierarchyPanel::HierarchyPanel(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {
    std::cout << "Creating hierarchy panel..." << std::endl;
}

HierarchyPanel::~HierarchyPanel() {
    std::cout << "Destroying hierarchy panel..." << std::endl;
}

void HierarchyPanel::Update(float deltaTime) {
    // Update logic
}

void HierarchyPanel::Render() {
    // Render logic
}

void HierarchyPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}

void HierarchyPanel::AddGameObject(GameObject* gameObject) {
    // Add game object to hierarchy
}

void HierarchyPanel::RemoveGameObject(GameObject* gameObject) {
    // Remove game object from hierarchy
}
