#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>

// Include our centralized OpenGL header
#include "../ThirdParty/OpenGL/include/GL/gl_definitions.h"

#include "EditorMain.h"
#include "../Scene.h"
#include "../GameObject.h"
#include "../Debugger.h"
#include "Editor.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../TimeManager.h"
#include "../FrameCapture.h"

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
    bool windowCreated = editor->CreateWindow("GameEngineSavi Editor");
    if (!windowCreated) {
        std::cout << "Failed to create window, continuing with headless rendering..." << std::endl;
        
        // Create frames directory for headless rendering
        system("mkdir -p frames");
    }
    
    // Initialize editor after window creation attempt
    editor->Initialize();
    
    // If window creation failed, we'll still render frames but save them to files
    if (!windowCreated) {
        std::cout << "Running in headless mode, frames will be saved to 'frames' directory" << std::endl;
    }
    
    // Initialize the editor
    InitializeEditor();
    
    // Run main loop
    if (windowCreated) {
        // Normal window-based main loop
        editor->RunMainLoop();
    } else {
        // Headless rendering main loop
        bool running = true;
        TimeManager timeManager;
        int frameCount = 0;
        
        while (running && frameCount < 100) { // Limit to 100 frames for testing
            // Update time
            timeManager.Update();
            float deltaTime = timeManager.GetDeltaTime();
            
            // Update and render
            editor->Update(deltaTime);
            editor->Render();
            
            // Capture frame to file
            std::stringstream ss;
            ss << "frames/frame" << frameCount << ".png";
            std::string filename = ss.str();
            FrameCapture::CaptureViewportToFile(filename);
            std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
            
            // Increment frame counter
            frameCount++;
            
            // Small delay to not overwhelm the system
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "Headless rendering complete. " << frameCount << " frames rendered." << std::endl;
    }
    
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
