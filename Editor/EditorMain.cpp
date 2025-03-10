#include <iostream>
#include <string>
#include <vector>

// Include our centralized OpenGL header
#include "../ThirdParty/OpenGL/include/GL/gl_definitions.h"

#include "EditorMain.h"
#include "../Scene.h"
#include "../GameObject.h"
#include "../Debugger.h"
#include "Editor.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../TimeManager.h"

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Global editor instance
Editor* editor = nullptr;

int main(int argc, char** argv) {
    std::cout << "Starting Game Engine Editor..." << std::endl;
    
    // Initialize graphics API factory first
    if (!GraphicsAPIFactory::GetInstance().Initialize()) {
        std::cout << "Failed to initialize graphics API factory" << std::endl;
        return 1;
    }
    
    // Create editor
    editor = new Editor(WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Create window first
    if (!editor->CreateWindow("GameEngineSavi Editor")) {
        std::cout << "Failed to create window" << std::endl;
        delete editor;
        return 1;
    }
    
    // Initialize editor after window creation
    editor->Initialize();
    
    // Initialize the editor
    InitializeEditor();
    
    // Run main loop
    editor->RunMainLoop();
    
    // Clean up
    editor->DestroyWindow();
    delete editor;
    
    std::cout << "Editor closed successfully." << std::endl;
    
    return 0;
}

void InitializeEditor() {
    // Initialize the editor
    std::cout << "Initializing editor..." << std::endl;
    
    // Create a default scene
    if (editor && editor->GetScene()) {
        editor->GetScene()->CreateDefaultObjects();
    }
    
    std::cout << "Editor initialized successfully" << std::endl;
}

void RenderScene() {
    // This function is now handled by Editor::Render() and Scene::RenderScene()
    if (editor) {
        editor->Render();
    }
}

void Resize(int width, int height) {
    if (editor) {
        editor->Resize(width, height);
    }
}

void KeyboardInput(unsigned char key, int x, int y) {
    // This function is now handled by the graphics API's event handling
    std::cout << "Keyboard input: " << key << " at (" << x << ", " << y << ")" << std::endl;
}

void MouseInput(int button, int state, int x, int y) {
    // This function is now handled by the graphics API's event handling
    std::cout << "Mouse input: button " << button << ", state " << state << " at (" << x << ", " << y << ")" << std::endl;
}

void MouseMotion(int x, int y) {
    // This function is now handled by the graphics API's event handling
    std::cout << "Mouse motion: (" << x << ", " << y << ")" << std::endl;
}
