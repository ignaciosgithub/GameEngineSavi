/**
 * Editor_emergency_png.cpp
 * Emergency renderer for GameEngineSavi editor with proper PNG output.
 */

#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../TimeManager.h"
#include "../FrameCapture_png.h"
#include "../Camera.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <cstdlib> // For system()
#include <fstream>

// Window dimensions - REDUCED RESOLUTION
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Global editor instance
std::unique_ptr<Editor> editor;

// Frame counter
int frameCount = 0;

// Function to generate frame filename
std::string GetFrameFilename() {
    std::stringstream ss;
    ss << "frames/frame" << frameCount << ".png";
    std::string filename = ss.str();
    std::cout << "Generating filename: " << filename << std::endl;
    return filename;
}

int main(int argc, char** argv) {
    std::cout << "Starting PNG emergency renderer..." << std::endl;
    
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Create frames directory
    system("mkdir -p frames");
    system("rm -f frames/*.png"); // Clean up any existing frames
    
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
    
    while (running && frameCount < 50) { // Limit to 50 frames for testing
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Update editor
        if (editor) {
            editor->Update(deltaTime);
            editor->Render();
            
            // Capture frame to file
            std::string filename = GetFrameFilename();
            bool success = FrameCapture_PNG::CaptureViewportToFile(filename);
            
            if (success) {
                std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
                
                // Verify the file was written correctly
                std::ifstream file(filename, std::ios::binary);
                if (file.is_open()) {
                    file.seekg(0, std::ios::end);
                    size_t fileSize = file.tellg();
                    file.close();
                    std::cout << "Frame file size: " << fileSize << " bytes" << std::endl;
                    
                    if (fileSize < 100) {
                        std::cerr << "Warning: Frame file is suspiciously small!" << std::endl;
                    }
                } else {
                    std::cerr << "Error: Could not open frame file for verification!" << std::endl;
                }
            } else {
                std::cerr << "Error: Failed to capture frame " << frameCount << std::endl;
            }
            
            // Increment frame counter
            frameCount++;
            
            // Small delay to ensure file writes complete
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }
    
    std::cout << "Emergency rendering complete. " << frameCount << " frames rendered." << std::endl;
    std::cout << "Run the Python viewer script to view the rendered frames." << std::endl;
    
    return 0;
}
