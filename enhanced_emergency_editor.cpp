/**
 * enhanced_emergency_frame_saver.cpp
 * Enhanced emergency editor for the GameEngineSavi engine.
 * 
 * This version supports panel traversal and hotkeys, saving frames to disk
 * instead of displaying them on screen. It also tracks panel information and
 * hotkey state.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>
#include <map>
#include <functional>

// Include stb_image_write.h
#include "ThirdParty/stb/stb_image_write.h"

// Panel types
enum PanelType {
    HIERARCHY_PANEL = 0,
    SCENE_VIEW_PANEL = 1,
    INSPECTOR_PANEL = 2,
    PROJECT_PANEL = 3
};

// Camera position and rotation for scene navigation
struct Camera {
    float posX = 0.0f;
    float posY = 2.0f;
    float posZ = 5.0f;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;
};

// Hotkey state
struct HotkeyState {
    bool ctrlPressed = false;
    bool shiftPressed = false;
    bool altPressed = false;
    bool wPressed = false;
    bool aPressed = false;
    bool sPressed = false;
    bool dPressed = false;
    bool spacePressed = false;
    int lastPanelHotkey = 0; // 1-4 for panel hotkeys
};

// Global state
PanelType activePanel = SCENE_VIEW_PANEL; // Default to scene view panel
HotkeyState hotkeyState;
Camera camera;
bool isPlaying = false;

// Function to save panel information to a file
void SavePanelInfo() {
    std::ofstream panelFile("frames/panel_info.txt");
    if (panelFile.is_open()) {
        panelFile << "Active Panel: " << activePanel << std::endl;
        
        switch (activePanel) {
            case HIERARCHY_PANEL:
                panelFile << "Hierarchy Panel" << std::endl;
                break;
            case SCENE_VIEW_PANEL:
                panelFile << "Scene View Panel" << std::endl;
                break;
            case INSPECTOR_PANEL:
                panelFile << "Inspector Panel" << std::endl;
                break;
            case PROJECT_PANEL:
                panelFile << "Project Panel" << std::endl;
                break;
        }
        
        panelFile.close();
        std::cout << "Panel information saved to frames/panel_info.txt" << std::endl;
    } else {
        std::cerr << "Failed to save panel information" << std::endl;
    }
}

// Function to save hotkey state to a file
void SaveHotkeyState() {
    std::ofstream hotkeyFile("frames/hotkey_state.txt");
    if (hotkeyFile.is_open()) {
        hotkeyFile << "Ctrl: " << (hotkeyState.ctrlPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "Shift: " << (hotkeyState.shiftPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "Alt: " << (hotkeyState.altPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "W: " << (hotkeyState.wPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "A: " << (hotkeyState.aPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "S: " << (hotkeyState.sPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "D: " << (hotkeyState.dPressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "Space: " << (hotkeyState.spacePressed ? "Pressed" : "Released") << std::endl;
        hotkeyFile << "Last Panel Hotkey: " << hotkeyState.lastPanelHotkey << std::endl;
        hotkeyFile << "Play Mode: " << (isPlaying ? "Playing" : "Editing") << std::endl;
        
        hotkeyFile.close();
        std::cout << "Hotkey state saved to frames/hotkey_state.txt" << std::endl;
    } else {
        std::cerr << "Failed to save hotkey state" << std::endl;
    }
}

// Function to process hotkeys
void ProcessHotkeys(const std::string& input) {
    for (char c : input) {
        switch (c) {
            case '1':
                if (hotkeyState.ctrlPressed) {
                    activePanel = HIERARCHY_PANEL;
                    hotkeyState.lastPanelHotkey = 1;
                    std::cout << "Switched to Hierarchy Panel" << std::endl;
                }
                break;
            case '2':
                if (hotkeyState.ctrlPressed) {
                    activePanel = SCENE_VIEW_PANEL;
                    hotkeyState.lastPanelHotkey = 2;
                    std::cout << "Switched to Scene View Panel" << std::endl;
                }
                break;
            case '3':
                if (hotkeyState.ctrlPressed) {
                    activePanel = INSPECTOR_PANEL;
                    hotkeyState.lastPanelHotkey = 3;
                    std::cout << "Switched to Inspector Panel" << std::endl;
                }
                break;
            case '4':
                if (hotkeyState.ctrlPressed) {
                    activePanel = PROJECT_PANEL;
                    hotkeyState.lastPanelHotkey = 4;
                    std::cout << "Switched to Project Panel" << std::endl;
                }
                break;
            case 'c':
                hotkeyState.ctrlPressed = true;
                break;
            case 'C':
                hotkeyState.ctrlPressed = false;
                break;
            case 's':
                hotkeyState.shiftPressed = true;
                break;
            case 'S':
                hotkeyState.shiftPressed = false;
                break;
            case 'a':
                hotkeyState.altPressed = true;
                break;
            case 'A':
                hotkeyState.altPressed = false;
                break;
            case 'w':
                hotkeyState.wPressed = true;
                if (activePanel == SCENE_VIEW_PANEL) {
                    // Move camera forward
                    camera.posZ -= 0.5f * (hotkeyState.shiftPressed ? 2.0f : 1.0f);
                    std::cout << "Camera moved forward to Z=" << camera.posZ << std::endl;
                }
                break;
            case 'W':
                hotkeyState.wPressed = false;
                break;
            case 'd':
                hotkeyState.dPressed = true;
                if (activePanel == SCENE_VIEW_PANEL) {
                    // Move camera right
                    camera.posX += 0.5f * (hotkeyState.shiftPressed ? 2.0f : 1.0f);
                    std::cout << "Camera moved right to X=" << camera.posX << std::endl;
                }
                break;
            case 'D':
                hotkeyState.dPressed = false;
                break;
            case 'a':
                hotkeyState.aPressed = true;
                if (activePanel == SCENE_VIEW_PANEL) {
                    // Move camera left
                    camera.posX -= 0.5f * (hotkeyState.shiftPressed ? 2.0f : 1.0f);
                    std::cout << "Camera moved left to X=" << camera.posX << std::endl;
                }
                break;
            case 'A':
                hotkeyState.aPressed = false;
                break;
            case 's':
                hotkeyState.sPressed = true;
                if (activePanel == SCENE_VIEW_PANEL) {
                    // Move camera backward
                    camera.posZ += 0.5f * (hotkeyState.shiftPressed ? 2.0f : 1.0f);
                    std::cout << "Camera moved backward to Z=" << camera.posZ << std::endl;
                }
                break;
            case 'S':
                hotkeyState.sPressed = false;
                break;
            case ' ':
                hotkeyState.spacePressed = !hotkeyState.spacePressed;
                isPlaying = hotkeyState.spacePressed;
                std::cout << "Play mode " << (isPlaying ? "enabled" : "disabled") << std::endl;
                break;
        }
    }
    
    // Save the updated state
    SavePanelInfo();
    SaveHotkeyState();
}

// Function to create a simulated frame with panel layout
bool CreateSimulatedFrame(const std::string& filename, int width, int height, int frameNumber) {
    std::cout << "Creating simulated frame: " << filename << " (" << width << "x" << height << ")" << std::endl;
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        std::cerr << "Invalid image dimensions: " << width << "x" << height << std::endl;
        return false;
    }
    
    // Create pixel data
    std::vector<unsigned char> pixels(width * height * 4);
    
    // Generate a simple colored image with panel layout
    float rotationAngle = frameNumber * 36.0f; // 360 degrees / 10 frames = 36 degrees per frame
    float angleRad = rotationAngle * 3.14159f / 180.0f;
    
    // Panel dimensions
    int hierarchyWidth = 200;
    int inspectorWidth = 300;
    int projectHeight = 200;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Determine which panel this pixel belongs to
            bool isHierarchyPanel = (x < hierarchyWidth);
            bool isInspectorPanel = (x >= width - inspectorWidth);
            bool isProjectPanel = (y >= height - projectHeight && x >= hierarchyWidth && x < width - inspectorWidth);
            bool isSceneViewPanel = (!isHierarchyPanel && !isInspectorPanel && !isProjectPanel);
            
            // Panel colors
            unsigned char r = 50, g = 50, b = 50, a = 255;
            
            // Highlight active panel
            if ((isHierarchyPanel && activePanel == HIERARCHY_PANEL) ||
                (isSceneViewPanel && activePanel == SCENE_VIEW_PANEL) ||
                (isInspectorPanel && activePanel == INSPECTOR_PANEL) ||
                (isProjectPanel && activePanel == PROJECT_PANEL)) {
                r = 70; g = 70; b = 90; // Slightly brighter for active panel
            }
            
            // Draw panel borders
            bool isBorder = 
                (x == hierarchyWidth - 1) || 
                (x == width - inspectorWidth - 1) || 
                (y == height - projectHeight - 1 && x >= hierarchyWidth && x < width - inspectorWidth);
            
            if (isBorder) {
                r = 100; g = 100; b = 100; // Gray border
            }
            
            // Draw panel titles
            bool isTitle = (y < 20);
            if (isTitle) {
                if (isHierarchyPanel) {
                    r = 80; g = 80; b = 100; // Hierarchy title
                } else if (isSceneViewPanel) {
                    r = 80; g = 100; b = 80; // Scene view title
                } else if (isInspectorPanel) {
                    r = 100; g = 80; b = 80; // Inspector title
                }
            }
            
            if (isProjectPanel && y < height - projectHeight + 20) {
                r = 100; g = 100; b = 80; // Project title
            }
            
            // Draw 3D scene in scene view panel
            if (isSceneViewPanel && !isTitle) {
                // Normalize coordinates to [-1, 1] for the scene view area
                float nx = (float)(x - hierarchyWidth) / (width - hierarchyWidth - inspectorWidth) * 2.0f - 1.0f;
                float ny = (float)y / (height - projectHeight) * 2.0f - 1.0f;
                
                // Apply camera transformation (simplified)
                float cx = nx + camera.posX * 0.1f;
                float cy = ny - camera.posY * 0.1f;
                float cz = camera.posZ * 0.1f;
                
                // Simple perspective projection
                float distance = sqrt(cx*cx + cy*cy + cz*cz);
                
                // Apply rotation (simplified)
                float angle = atan2(cy, cx) + angleRad;
                float radius = sqrt(cx*cx + cy*cy);
                float rx = radius * cos(angle);
                float ry = radius * sin(angle);
                
                // Calculate lighting (simplified)
                float lightFactor = std::max(0.2f, 1.0f - distance);
                
                // Calculate color based on position and lighting
                // Cube visualization (simplified)
                if (distance < 0.5f) {
                    // Inside cube area - use different colors for different faces
                    if (rx > 0 && ry > 0) {
                        r = static_cast<int>(255 * lightFactor);
                        g = static_cast<int>(0 * lightFactor);
                        b = static_cast<int>(0 * lightFactor);
                    } else if (rx < 0 && ry > 0) {
                        r = static_cast<int>(0 * lightFactor);
                        g = static_cast<int>(255 * lightFactor);
                        b = static_cast<int>(0 * lightFactor);
                    } else if (rx < 0 && ry < 0) {
                        r = static_cast<int>(0 * lightFactor);
                        g = static_cast<int>(0 * lightFactor);
                        b = static_cast<int>(255 * lightFactor);
                    } else {
                        r = static_cast<int>(255 * lightFactor);
                        g = static_cast<int>(255 * lightFactor);
                        b = static_cast<int>(0 * lightFactor);
                    }
                } else {
                    // Background
                    r = static_cast<int>(30 * lightFactor);
                    g = static_cast<int>(30 * lightFactor);
                    b = static_cast<int>(40 * lightFactor);
                }
            }
            
            // Draw hierarchy panel content
            if (isHierarchyPanel && !isTitle) {
                // Draw some fake hierarchy items
                if (y > 30 && y < 50) {
                    r = 200; g = 200; b = 200; // Default Light
                } else if (y > 60 && y < 80) {
                    r = 200; g = 200; b = 200; // Default Cube
                }
            }
            
            // Draw inspector panel content
            if (isInspectorPanel && !isTitle) {
                // Draw some fake inspector items
                if (y > 30 && y < 50) {
                    r = 200; g = 200; b = 200; // Transform section
                } else if (y > 60 && y < 80) {
                    r = 200; g = 200; b = 200; // Position fields
                } else if (y > 90 && y < 110) {
                    r = 200; g = 200; b = 200; // Rotation fields
                } else if (y > 120 && y < 140) {
                    r = 200; g = 200; b = 200; // Scale fields
                }
            }
            
            // Draw project panel content
            if (isProjectPanel) {
                // Draw some fake project items
                if (x > hierarchyWidth + 20 && x < hierarchyWidth + 100 && y > height - projectHeight + 30 && y < height - projectHeight + 50) {
                    r = 200; g = 200; b = 200; // Models folder
                } else if (x > hierarchyWidth + 120 && x < hierarchyWidth + 200 && y > height - projectHeight + 30 && y < height - projectHeight + 50) {
                    r = 200; g = 200; b = 200; // Textures folder
                }
            }
            
            // Ensure color values are in valid range
            r = std::min(255, std::max(0, (int)r));
            g = std::min(255, std::max(0, (int)g));
            b = std::min(255, std::max(0, (int)b));
            
            // Set pixel color (RGBA format)
            int index = (y * width + x) * 4;
            pixels[index] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = a;
        }
    }
    
    // Save the frame to file using stb_image_write
    int result = stbi_write_png(
        filename.c_str(),
        width,
        height,
        4,  // RGBA components
        pixels.data(),
        width * 4  // Stride in bytes
    );
    
    if (result == 0) {
        std::cerr << "Failed to write PNG file: " << filename << std::endl;
        return false;
    }
    
    // Verify the file was written correctly
    std::ifstream verifyFile(filename, std::ios::binary);
    if (!verifyFile.is_open()) {
        std::cerr << "Warning: Could not verify file was written: " << filename << std::endl;
        return true; // Still return true as stb_image_write reported success
    }
    
    verifyFile.seekg(0, std::ios::end);
    size_t fileSize = verifyFile.tellg();
    verifyFile.close(); // Explicitly close the verification file stream
    
    if (fileSize < 100) {
        std::cerr << "Warning: Generated file is suspiciously small: " << fileSize << " bytes" << std::endl;
    } else {
        std::cout << "Successfully wrote " << fileSize << " bytes to " << filename << std::endl;
    }
    
    return true;
}

int main(int argc, char** argv) {
    std::cout << "Starting enhanced emergency editor..." << std::endl;
    std::cout << "This editor saves frames to disk and supports panel traversal and hotkeys." << std::endl;
    std::cout << "Default panel is Scene View Panel for 3D traversal." << std::endl;
    std::cout << "Use Ctrl+1-4 to switch between panels:" << std::endl;
    std::cout << "  Ctrl+1: Hierarchy Panel" << std::endl;
    std::cout << "  Ctrl+2: Scene View Panel (default)" << std::endl;
    std::cout << "  Ctrl+3: Inspector Panel" << std::endl;
    std::cout << "  Ctrl+4: Project Panel" << std::endl;
    std::cout << "Use WASD to navigate in Scene View Panel:" << std::endl;
    std::cout << "  W: Move forward" << std::endl;
    std::cout << "  A: Move left" << std::endl;
    std::cout << "  S: Move backward" << std::endl;
    std::cout << "  D: Move right" << std::endl;
    std::cout << "  Shift: Hold to move faster" << std::endl;
    std::cout << "  Space: Toggle play/pause mode" << std::endl;
    
    // Create frames directory
    system("mkdir -p frames");
    
    // Initialize panel info and hotkey state
    SavePanelInfo();
    SaveHotkeyState();
    
    // Generate frames
    const int numFrames = 10;
    const int width = 1280;
    const int height = 720;
    
    // Simulate user input for demonstration
    std::vector<std::string> simulatedInputs = {
        "c2C",      // Ctrl+2 (Scene View Panel)
        "w",        // Move forward
        "w",        // Move forward
        "d",        // Move right
        "c1C",      // Ctrl+1 (Hierarchy Panel)
        "c3C",      // Ctrl+3 (Inspector Panel)
        "c4C",      // Ctrl+4 (Project Panel)
        "c2C",      // Ctrl+2 (Scene View Panel)
        " ",        // Toggle play mode
        "s"         // Move backward
    };
    
    for (int i = 0; i < numFrames; i++) {
        std::stringstream ss;
        ss << "frames/frame" << i << ".png";
        std::string filename = ss.str();
        
        std::cout << "Creating frame " << i << "..." << std::endl;
        
        // Process simulated input for this frame
        if (i < simulatedInputs.size()) {
            std::cout << "Processing input: " << simulatedInputs[i] << std::endl;
            ProcessHotkeys(simulatedInputs[i]);
        }
        
        // Create simulated frame
        if (CreateSimulatedFrame(filename, width, height, i)) {
            std::cout << "Frame " << i << " created successfully." << std::endl;
        } else {
            std::cerr << "Failed to create frame " << i << "." << std::endl;
        }
        
        // Small delay to not overwhelm the system
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Enhanced emergency editor complete. " << numFrames << " frames created." << std::endl;
    std::cout << "Panel information saved to frames/panel_info.txt" << std::endl;
    std::cout << "Hotkey state saved to frames/hotkey_state.txt" << std::endl;
    std::cout << "Use the frame_reader.py script to view the frames in sequence." << std::endl;
    
    return 0;
}
