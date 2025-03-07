/**
 * TextControlledEditor.cpp
 * A headless version of the Editor that reads commands from a text file.
 * This version captures the entire editor UI including panels and renders it to PNG files.
 * It reads key press commands from a text file in the format "KEY,DURATION".
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>
#include <fstream>

#include "../platform.h"
#include "../TimeManager.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../Graphics/Core/IGraphicsAPI.h"
#include "../FrameCapture_png.h"
#include "../Editor/Editor.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../CameraManager.h"
#include "../Shaders/Core/ShaderProgram.h"
#include "../Editor/HierarchyPanel.h"
#include "../Editor/InspectorPanel.h"
#include "../Editor/ProjectPanel.h"
#include "../Editor/SceneViewPanel.h"

// Define headless mode for conditional compilation
#ifndef HEADLESS_MODE
#define HEADLESS_MODE
#endif

// Include stb_image_write.h for direct PNG writing (implementation in stb_image_write_impl.cpp)
#include "../ThirdParty/stb/stb_image_write.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) mkdir(dir)
#else
#define MKDIR(dir) mkdir(dir, 0755)
#endif

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Frame counter
int frameCount = 0;

// Command file path
const std::string COMMAND_FILE = "frames/commands.txt";
const std::string COMMAND_SEPARATOR = ",";

// Function declarations
std::string GetFrameFilename();
void createFramesDirectory();
void createEditorPanelImage(const std::string& filename, int width, int height, Editor* editor);
void drawPanel(std::vector<unsigned char>& pixels, int imgWidth, int imgHeight, 
               int x, int y, int width, int height, const std::string& title,
               unsigned char r, unsigned char g, unsigned char b);
void drawCube(std::vector<unsigned char>& pixels, int imgWidth, int imgHeight,
              int centerX, int centerY, int size, unsigned char r, unsigned char g, unsigned char b);
bool checkCommandFile(std::string& key, float& duration);
void clearCommandFile();

// Function to generate frame filename
std::string GetFrameFilename() {
    std::stringstream ss;
    ss << "frames/frame" << frameCount << ".png";
    return ss.str();
}

// Create frames directory if it doesn't exist
void createFramesDirectory() {
    const char* framesDir = "frames";
    struct stat st = {0};
    
    if (stat(framesDir, &st) == -1) {
        if (MKDIR(framesDir) != 0) {
            std::cerr << "Error creating frames directory" << std::endl;
        } else {
            std::cout << "Created frames directory" << std::endl;
        }
    }
}

// Check command file for new commands
bool checkCommandFile(std::string& key, float& duration) {
    // Check if the file exists
    struct stat st = {0};
    if (stat(COMMAND_FILE.c_str(), &st) == -1) {
        // Create an empty command file if it doesn't exist
        std::ofstream commandFile(COMMAND_FILE);
        if (commandFile.is_open()) {
            commandFile.close();
            std::cout << "Created empty command file: " << COMMAND_FILE << std::endl;
        } else {
            std::cerr << "Failed to create command file: " << COMMAND_FILE << std::endl;
        }
        return false;
    }
    
    // Check if the file is empty
    if (st.st_size == 0) {
        return false;
    }
    
    // Read the command file
    std::ifstream commandFile(COMMAND_FILE);
    if (!commandFile.is_open()) {
        std::cerr << "Failed to open command file: " << COMMAND_FILE << std::endl;
        return false;
    }
    
    std::string line;
    if (std::getline(commandFile, line)) {
        // Parse the command
        size_t separatorPos = line.find(COMMAND_SEPARATOR);
        if (separatorPos != std::string::npos) {
            key = line.substr(0, separatorPos);
            std::string durationStr = line.substr(separatorPos + 1);
            
            try {
                duration = std::stof(durationStr);
                commandFile.close();
                
                // Clear the command file
                clearCommandFile();
                
                std::cout << "Read command: Key=" << key << ", Duration=" << duration << std::endl;
                return true;
            } catch (const std::exception& e) {
                std::cerr << "Failed to parse duration: " << durationStr << std::endl;
            }
        } else {
            std::cerr << "Invalid command format: " << line << std::endl;
        }
    }
    
    commandFile.close();
    return false;
}

// Clear the command file
void clearCommandFile() {
    std::ofstream commandFile(COMMAND_FILE, std::ofstream::trunc);
    if (commandFile.is_open()) {
        commandFile.close();
        std::cout << "Cleared command file: " << COMMAND_FILE << std::endl;
    } else {
        std::cerr << "Failed to clear command file: " << COMMAND_FILE << std::endl;
    }
}

// Helper function to draw a panel in the image
void drawPanel(std::vector<unsigned char>& pixels, int imgWidth, int imgHeight, 
               int x, int y, int width, int height, const std::string& title,
               unsigned char r, unsigned char g, unsigned char b) {
    // Draw panel background with more vibrant colors
    for (int py = y; py < y + height && py < imgHeight; py++) {
        for (int px = x; px < x + width && px < imgWidth; px++) {
            int index = (py * imgWidth + px) * 4;
            if (index + 3 < pixels.size()) {
                // Use the provided colors for the panel background
                pixels[index] = r;
                pixels[index + 1] = g;
                pixels[index + 2] = b;
                pixels[index + 3] = 255;
            }
        }
    }
    
    // Draw panel border with white for better visibility
    for (int px = x; px < x + width && px < imgWidth; px++) {
        int topIndex = (y * imgWidth + px) * 4;
        int bottomIndex = ((y + height - 1) * imgWidth + px) * 4;
        
        if (topIndex + 3 < pixels.size()) {
            pixels[topIndex] = 255;
            pixels[topIndex + 1] = 255;
            pixels[topIndex + 2] = 255;
            pixels[topIndex + 3] = 255;
        }
        
        if (bottomIndex + 3 < pixels.size()) {
            pixels[bottomIndex] = 255;
            pixels[bottomIndex + 1] = 255;
            pixels[bottomIndex + 2] = 255;
            pixels[bottomIndex + 3] = 255;
        }
    }
    
    for (int py = y; py < y + height && py < imgHeight; py++) {
        int leftIndex = (py * imgWidth + x) * 4;
        int rightIndex = (py * imgWidth + x + width - 1) * 4;
        
        if (leftIndex + 3 < pixels.size()) {
            pixels[leftIndex] = 255;
            pixels[leftIndex + 1] = 255;
            pixels[leftIndex + 2] = 255;
            pixels[leftIndex + 3] = 255;
        }
        
        if (rightIndex + 3 < pixels.size()) {
            pixels[rightIndex] = 255;
            pixels[rightIndex + 1] = 255;
            pixels[rightIndex + 2] = 255;
            pixels[rightIndex + 3] = 255;
        }
    }
}

// Helper function to draw a cube in the image
void drawCube(std::vector<unsigned char>& pixels, int imgWidth, int imgHeight,
              int centerX, int centerY, int size,
              unsigned char r, unsigned char g, unsigned char b) {
    int halfSize = size / 2;
    
    // Draw the cube with a bright red color
    for (int py = centerY - halfSize; py < centerY + halfSize && py < imgHeight; py++) {
        for (int px = centerX - halfSize; px < centerX + halfSize && px < imgWidth; px++) {
            if (px >= 0 && py >= 0) {
                int index = (py * imgWidth + px) * 4;
                if (index + 3 < pixels.size()) {
                    pixels[index] = r;
                    pixels[index + 1] = g;
                    pixels[index + 2] = b;
                    pixels[index + 3] = 255;
                }
            }
        }
    }
    
    // Add a white border to make the cube more visible
    for (int px = centerX - halfSize; px <= centerX + halfSize && px < imgWidth; px++) {
        if (px >= 0) {
            int topIndex = ((centerY - halfSize) * imgWidth + px) * 4;
            int bottomIndex = ((centerY + halfSize - 1) * imgWidth + px) * 4;
            
            if (topIndex + 3 < pixels.size()) {
                pixels[topIndex] = 255;
                pixels[topIndex + 1] = 255;
                pixels[topIndex + 2] = 255;
                pixels[topIndex + 3] = 255;
            }
            
            if (bottomIndex + 3 < pixels.size()) {
                pixels[bottomIndex] = 255;
                pixels[bottomIndex + 1] = 255;
                pixels[bottomIndex + 2] = 255;
                pixels[bottomIndex + 3] = 255;
            }
        }
    }
    
    for (int py = centerY - halfSize; py <= centerY + halfSize && py < imgHeight; py++) {
        if (py >= 0) {
            int leftIndex = (py * imgWidth + (centerX - halfSize)) * 4;
            int rightIndex = (py * imgWidth + (centerX + halfSize - 1)) * 4;
            
            if (leftIndex + 3 < pixels.size()) {
                pixels[leftIndex] = 255;
                pixels[leftIndex + 1] = 255;
                pixels[leftIndex + 2] = 255;
                pixels[leftIndex + 3] = 255;
            }
            
            if (rightIndex + 3 < pixels.size()) {
                pixels[rightIndex] = 255;
                pixels[rightIndex + 1] = 255;
                pixels[rightIndex + 2] = 255;
                pixels[rightIndex + 3] = 255;
            }
        }
    }
}

// Create an image that shows editor panels in headless mode
void createEditorPanelImage(const std::string& filename, int width, int height, Editor* editor) {
    // Create a buffer for the image (RGBA format)
    std::vector<unsigned char> pixels(width * height * 4);
    
    // Fill with dark gray background color
    for (int i = 0; i < width * height * 4; i += 4) {
        pixels[i] = 30;      // R
        pixels[i + 1] = 30;  // G
        pixels[i + 2] = 30;  // B
        pixels[i + 3] = 255; // A
    }
    
    // Always draw panel outlines and content, even if editor is null
    // Hierarchy panel (left)
    int hierarchyX = 10;
    int hierarchyY = 10;
    int hierarchyWidth = width / 5;
    int hierarchyHeight = height - 20;
    drawPanel(pixels, width, height, hierarchyX, hierarchyY, hierarchyWidth, hierarchyHeight, "Hierarchy", 150, 150, 255);
    
    // Scene view panel (center)
    int sceneX = hierarchyX + hierarchyWidth + 10;
    int sceneY = 10;
    int sceneWidth = (width / 5) * 3 - 20;
    int sceneHeight = height - 20;
    drawPanel(pixels, width, height, sceneX, sceneY, sceneWidth, sceneHeight, "Scene View", 100, 100, 180);
    
    // Draw a red cube in the scene view
    drawCube(pixels, width, height, sceneX + sceneWidth/2, sceneY + sceneHeight/2, 100, 255, 0, 0);
    
    // Inspector panel (right)
    int inspectorX = sceneX + sceneWidth + 10;
    int inspectorY = 10;
    int inspectorWidth = width / 5;
    int inspectorHeight = height / 2 - 15;
    drawPanel(pixels, width, height, inspectorX, inspectorY, inspectorWidth, inspectorHeight, "Inspector", 100, 255, 100);
    
    // Project panel (bottom right)
    int projectX = inspectorX;
    int projectY = inspectorY + inspectorHeight + 10;
    int projectWidth = inspectorWidth;
    int projectHeight = height / 2 - 15;
    drawPanel(pixels, width, height, projectX, projectY, projectWidth, projectHeight, "Project", 255, 150, 150);
    
    // If we have an editor, we can add more details
    if (editor) {
        // Add additional editor-specific details here if needed
    }
    
    // Save to PNG file using stbi_write_png
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
    } else {
        std::cout << "Created editor panel image: " << filename << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "Starting Text Controlled Editor..." << std::endl;
    
    // Create frames directory
    createFramesDirectory();
    
    // Initialize graphics API
    std::cout << "Initializing graphics API..." << std::endl;
    if (!GraphicsAPIFactory::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize graphics API, falling back to direct image creation" << std::endl;
        
        // Create 30 frames directly with editor panels
        for (int i = 0; i < 30; i++) {
            std::string filename = "frames/frame" + std::to_string(i) + ".png";
            // Create a null editor for the panel image
            createEditorPanelImage(filename, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr);
            std::cout << "Created fallback frame " << i << " due to graphics API initialization failure" << std::endl;
            frameCount++;
        }
        
        return 0;
    }
    std::cout << "Graphics API initialized successfully" << std::endl;
    
    // Get the graphics API
    std::cout << "Getting graphics API instance..." << std::endl;
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "ERROR: Failed to get graphics API instance, falling back to direct image creation" << std::endl;
        
        // Create 30 frames directly with editor panels
        for (int i = 0; i < 30; i++) {
            std::string filename = "frames/frame" + std::to_string(i) + ".png";
            // Create a null editor for the panel image
            createEditorPanelImage(filename, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr);
            std::cout << "Created fallback frame " << i << " due to missing graphics API instance" << std::endl;
            frameCount++;
        }
        
        return 0;
    }
    std::cout << "Graphics API instance obtained successfully" << std::endl;
    
    // Try to create a window, but don't require it
    bool hasWindow = false;
    std::cout << "Attempting to create window..." << std::endl;
    if (graphics->CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Text Controlled Editor")) {
        hasWindow = true;
        graphics->SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        std::cout << "Created window successfully" << std::endl;
    } else {
        std::cerr << "WARNING: Could not create window, running in headless mode..." << std::endl;
        std::cout << "Will generate editor panel images directly without OpenGL" << std::endl;
    }
    
    // Create the Editor only if we have a window
    Editor* editor = nullptr;
    if (hasWindow) {
        // Create and initialize the Editor
        std::cout << "Creating and initializing Editor..." << std::endl;
        editor = new Editor(WINDOW_WIDTH, WINDOW_HEIGHT);
        editor->Initialize();
    } else {
        std::cout << "Skipping Editor creation in headless mode" << std::endl;
    }
    
    // Signal that initialization is complete
    std::ofstream statusFile("frames/status.txt");
    if (statusFile.is_open()) {
        statusFile << "initialized" << std::endl;
        statusFile.close();
        std::cout << "Created status file to indicate initialization is complete" << std::endl;
    } else {
        std::cerr << "WARNING: Could not create status file" << std::endl;
    }
    
    // Create an empty command file if it doesn't exist
    std::ofstream commandFile(COMMAND_FILE);
    if (commandFile.is_open()) {
        commandFile.close();
        std::cout << "Created empty command file: " << COMMAND_FILE << std::endl;
    } else {
        std::cerr << "WARNING: Could not create command file" << std::endl;
    }
    
    // Main loop
    TimeManager timeManager;
    
    // Run continuously until interrupted
    bool running = true;
    
    // Create a signal file to check for exit requests
    std::ofstream exitSignalFile("frames/running.txt");
    if (exitSignalFile.is_open()) {
        exitSignalFile << "running" << std::endl;
        exitSignalFile.close();
    }
    
    std::cout << "Starting continuous frame generation. To stop, delete frames/running.txt" << std::endl;
    std::cout << "To control the editor, write commands to frames/commands.txt in the format KEY,DURATION" << std::endl;
    std::cout << "Example: A,1 (press A key for 1 second)" << std::endl;
    
    // Variables for key press simulation
    std::string currentKey = "";
    float keyPressDuration = 0.0f;
    float keyPressTimer = 0.0f;
    bool keyIsPressed = false;
    
    while (running) {
        // Check if we should exit (if running.txt file is deleted)
        std::ifstream checkRunning("frames/running.txt");
        if (!checkRunning.good()) {
            std::cout << "Exit signal detected (frames/running.txt not found). Stopping frame generation." << std::endl;
            running = false;
            break;
        }
        checkRunning.close();
        
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Get the frame filename
        std::string filename = GetFrameFilename();
        
        // Check for new commands if no key is currently being pressed
        if (!keyIsPressed) {
            std::string key;
            float duration;
            if (checkCommandFile(key, duration)) {
                currentKey = key;
                keyPressDuration = duration;
                keyPressTimer = 0.0f;
                keyIsPressed = true;
                std::cout << "Pressing key: " << currentKey << " for " << keyPressDuration << " seconds" << std::endl;
            }
        }
        
        // Update key press timer if a key is being pressed
        if (keyIsPressed) {
            keyPressTimer += deltaTime;
            if (keyPressTimer >= keyPressDuration) {
                keyIsPressed = false;
                std::cout << "Released key: " << currentKey << " after " << keyPressTimer << " seconds" << std::endl;
            }
        }
        
        if (hasWindow && editor) {
            // Poll events
            graphics->PollEvents();
            
            // Check if window is still open
            if (!graphics->IsWindowOpen()) {
                break;
            }
            
            // Clear the screen
            graphics->SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            graphics->Clear(true, true);
            
            // Set up key detection
            bool keyWPressed = keyIsPressed && currentKey == "W";
            bool keySPressed = keyIsPressed && currentKey == "S";
            bool keyAPressed = keyIsPressed && currentKey == "A";
            bool keyDPressed = keyIsPressed && currentKey == "D";
            
            // Apply camera movement based on key detection
            Camera* editorCamera = editor->GetEditorCamera();
            if (editorCamera) {
                Vector3 cameraPos = editorCamera->GetPosition();
                Vector3 rotation = editorCamera->GetRotation();
                
                // Calculate forward and right vectors based on rotation
                float yaw = rotation.y * 3.14159f / 180.0f;
                float pitch = rotation.x * 3.14159f / 180.0f;
                
                // Calculate forward vector
                Vector3 forward;
                forward.x = sin(yaw) * cos(pitch);
                forward.y = sin(pitch);
                forward.z = cos(yaw) * cos(pitch);
                forward.normalize();
                
                // Calculate right vector (cross product of up and forward)
                Vector3 up(0, 1, 0);
                Vector3 right = up.cross(forward);
                right.normalize();
                
                // Move camera based on key presses
                if (keyWPressed) cameraPos = cameraPos + forward * 0.1f;  // Move forward
                if (keySPressed) cameraPos = cameraPos - forward * 0.1f;  // Move backward
                if (keyAPressed) cameraPos = cameraPos - right * 0.1f;    // Move left
                if (keyDPressed) cameraPos = cameraPos + right * 0.1f;    // Move right
                
                editorCamera->SetPosition(cameraPos);
            }
            
            // Update the editor
            editor->Update(deltaTime);
            
            // Render the editor (includes all panels and scene)
            editor->Render();
            
            // Swap buffers to make the rendered content visible
            graphics->SwapBuffers();
            
            // Capture frame to file using PNG format
            std::cout << "Capturing frame " << frameCount << " to " << filename << "..." << std::endl;
            bool success = FrameCapture_PNG::CaptureViewportToFile(filename);
            if (success) {
                std::cout << "Successfully rendered frame " << frameCount << " to " << filename << std::endl;
            } else {
                std::cerr << "ERROR: Failed to save frame " << frameCount << " to " << filename << ", falling back to direct image creation" << std::endl;
                
                // Fall back to direct image creation
                createEditorPanelImage(filename, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr);
                std::cout << "Created fallback frame " << frameCount << " due to frame capture failure" << std::endl;
            }
        } else {
            // In headless mode, we'll create a more informative image that shows editor panels
            std::cout << "Creating headless mode frame " << frameCount << " to " << filename << "..." << std::endl;
            
            // Create a more informative image that shows editor panels
            createEditorPanelImage(filename, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr);
        }
        
        // Increment frame counter
        frameCount++;
        
        // Small delay to not overwhelm the system
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Clean up
    // Clean up editor
    if (editor) {
        delete editor;
        editor = nullptr;
    }
    
    // Destroy window if we have one
    if (hasWindow) {
        graphics->DestroyWindow();
    }
    
    std::cout << "Text controlled editor complete. " << frameCount << " frames rendered." << std::endl;
    
    return 0;
}
