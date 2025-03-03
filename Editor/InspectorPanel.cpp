#include "InspectorPanel.h"
#include "../GameObject.h"
#include <iostream>

InspectorPanel::InspectorPanel(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height), selectedGameObject(nullptr) {
    std::cout << "Creating inspector panel..." << std::endl;
}

InspectorPanel::~InspectorPanel() {
    std::cout << "Destroying inspector panel..." << std::endl;
}

void InspectorPanel::Update(float deltaTime) {
    // Update logic
}

void InspectorPanel::Render() {
    // Render logic
}

void InspectorPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}

void InspectorPanel::SetSelectedGameObject(GameObject* gameObject) {
    selectedGameObject = gameObject;
}
