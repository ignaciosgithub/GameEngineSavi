/**
 * main_editor_window.cpp
 * Entry point for the GameEngineSavi editor with window creation.
 * 
 * This file creates an instance of the Editor class, initializes it,
 * creates a window, and runs the main loop.
 */

#include "Editor/Editor.h"
#include "TimeManager.h"
#include "Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char** argv) {
    std::cout << "Starting GameEngineSavi Editor..." << std::endl;
    
    // Initialize graphics API factory
    if (!GraphicsAPIFactory::GetInstance().Initialize()) {
        std::cout << "Failed to initialize graphics API factory" << std::endl;
        return 1;
    }
    
    // Create editor
    Editor* editor = new Editor(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Initialize editor
    editor->Initialize();
    
    // Create window
    if (!editor->CreateWindow("GameEngineSavi Editor")) {
        std::cout << "Failed to create window" << std::endl;
        delete editor;
        return 1;
    }
    
    // Run main loop
    editor->RunMainLoop();
    
    // Clean up
    editor->DestroyWindow();
    delete editor;
    
    std::cout << "Editor closed successfully." << std::endl;
    
    return 0;
}
