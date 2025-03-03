#include "SceneViewPanel.h"
#include <iostream>

SceneViewPanel::SceneViewPanel(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {
    std::cout << "Creating scene view panel..." << std::endl;
}

SceneViewPanel::~SceneViewPanel() {
    std::cout << "Destroying scene view panel..." << std::endl;
}

void SceneViewPanel::Update(float deltaTime) {
    // Update logic
}

void SceneViewPanel::Render() {
    // Render logic
}

void SceneViewPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}
