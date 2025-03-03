#include "ProjectPanel.h"
#include <iostream>

ProjectPanel::ProjectPanel(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {
    std::cout << "Creating project panel..." << std::endl;
}

ProjectPanel::~ProjectPanel() {
    std::cout << "Destroying project panel..." << std::endl;
}

void ProjectPanel::Update(float deltaTime) {
    // Update logic
}

void ProjectPanel::Render() {
    // Render logic
}

void ProjectPanel::Resize(int newX, int newY, int newWidth, int newHeight) {
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
}
