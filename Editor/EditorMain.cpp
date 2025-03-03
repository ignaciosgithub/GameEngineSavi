#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <memory>

#include "Editor.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "ProjectPanel.h"
#include "SceneViewPanel.h"
#include "Vector3Field.h"

#include "../GameObject.h"
#include "../Camera.h"
#include "../Vector3.h"
#include "../Matrix4x4.h"
#include "../ThirdParty/OpenGL/include/GL/platform_gl.h"

// Global variables
Editor* editor = nullptr;
Camera* camera = nullptr;
bool isRunning = true;
bool isMouseDown = false;
int lastMouseX = 0;
int lastMouseY = 0;
float cameraSpeed = 0.1f;
float cameraRotationSpeed = 0.5f;

// Function prototypes
void Initialize();
void Update();
void Render();
void HandleInput();
void Cleanup();

int main(int argc, char** argv) {
    std::cout << "Starting GameEngineSavi Editor..." << std::endl;
    
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // Initialize the editor
    Initialize();
    
    // Main loop
    while (isRunning) {
        // Update
        Update();
        
        // Render
        Render();
        
        // Sleep to limit frame rate
        // In a real application, we would use a proper timer
        // but for this simple example, we'll just sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    // Clean up resources
    Cleanup();
    
    return 0;
}

void Initialize() {
    // Initialize OpenGL
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Create the editor
    editor = new Editor(1280, 720);
    
    // Get the editor camera
    camera = editor->GetEditorCamera();
    
    std::cout << "Editor initialized successfully." << std::endl;
}

void Update() {
    // Handle input
    HandleInput();
    
    // Update the editor
    editor->Update(0.016f); // Assuming 60 FPS
}

void Render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render the editor
    editor->Render();
    
    // Swap buffers (in a real application, this would be handled by the windowing system)
    // For this simple example, we'll just simulate it
    glFlush();
    std::cout << "Frame rendered." << std::endl;
}

void HandleInput() {
    // Camera movement
    if (isMouseDown) {
        // Camera rotation
        Vector3 rotation = camera->GetRotation();
        
        // Update rotation based on mouse movement
        rotation.y += (lastMouseX - 640) * cameraRotationSpeed;
        rotation.x += (lastMouseY - 360) * cameraRotationSpeed;
        
        // Clamp pitch to prevent flipping
        rotation.x = std::max(-89.0f, std::min(89.0f, rotation.x));
        
        // Apply rotation
        camera->SetRotation(rotation);
    }
}

void Cleanup() {
    // Delete the editor
    if (editor) {
        delete editor;
        editor = nullptr;
    }
    
    std::cout << "Editor cleaned up successfully." << std::endl;
}
