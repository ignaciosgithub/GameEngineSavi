/**
 * Editor_emergency_simple.cpp
 * Simplified emergency renderer for GameEngineSavi editor.
 */

#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../TimeManager.h"
#include "../FrameCapture.h"
#include "../Camera.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <cstdlib> // For system()

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Global editor instance
std::unique_ptr<Editor> editor;

// Frame counter
int frameCount = 0;

// Function to generate frame filename
std::string GetFrameFilename() {
    std::stringstream ss;
    ss << "frames/frame" << frameCount << ".png";
    return ss.str();
}

int main(int argc, char** argv) {
    std::cout << "Starting simplified emergency renderer..." << std::endl;
    
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Create frames directory
    system("mkdir -p frames");
    
    // Set engine condition to editor mode
    EngineCondition::SetInEditor(true);
    
    // Set up OpenGL for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Main loop
    bool running = true;
    TimeManager timeManager;
    
    while (running && frameCount < 100) { // Limit to 100 frames for testing
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Update editor
        if (editor) {
            editor->Update(deltaTime);
            editor->Render();
            
            // Capture frame to file
            std::string filename = GetFrameFilename();
            FrameCapture::CaptureViewportToFile(filename);
            std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
            
            // Increment frame counter
            frameCount++;
            
            // Small delay to not overwhelm the system
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    std::cout << "Emergency rendering complete. " << frameCount << " frames rendered." << std::endl;
    std::cout << "Run the Python viewer script to view the rendered frames." << std::endl;
    
    return 0;
}
