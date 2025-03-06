/**************************
 * Includes
 *
 **************************/

#include "platform.h"
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#include "Graphics/Core/GraphicsAPIFactory.h"
#if defined(PLATFORM_WINDOWS)
#ifndef CALLBACK
#define CALLBACK __stdcall
#endif
#include <windows.h>
#include "ThirdParty/OpenGL/include/GL/win_types.h"
#endif
#include <vector>
#include <string>
#include <cstring>
#include "Vector3.h"
#include "Triangle.h"
#include "Matrix4x4.h"
#include "Raycast.h"
#include "PointLight.h"
#include "Camera.h"
#include "Face.h"
#include "Pyramid.h"
#include "Model.h"
#include "MonoBehaviourLike.h"
#include "GameObject.h"
#include "Prefab.h"
#include "TimeManager.h"
#include "Scene.h"
#include "EngineCondition.h"
#include "GUI/GUI.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <atomic>

/**************************
 * Function Declarations
 *
 **************************/
inline float toRadians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

// Global GUI instance
std::unique_ptr<GUI> gui;

// No forward declarations needed for platform-independent implementation

/**************************
 * Main Entry Point
 *
 **************************/

int main(int argc, char** argv)
{
    // Set the initial engine condition based on command line arguments
    // This would normally be set by the editor or build system
    #if defined(DEBUG_BUILD)
    // Using available EngineCondition API
    EngineCondition::isInEditor = true;
    std::cout << "Starting engine in DEBUG mode" << std::endl;
    #else
    // Using available EngineCondition API
    EngineCondition::isInEditor = true;
    std::cout << "Starting engine in RELEASE mode" << std::endl;
    #endif

    // Check for editor mode command line arguments
    // In a real implementation, this would be handled by the editor
    #if defined(PLATFORM_WINDOWS)
    if (lpCmdLine && strstr(lpCmdLine, "-editor")) {
        EngineCondition::isInEditor = true;
        std::cout << "Starting in editor mode" << std::endl;
    }
    #else
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-editor") == 0) {
            EngineCondition::isInEditor = true;
            std::cout << "Starting in editor mode" << std::endl;
            break;
        }
    }
    #endif

    // Initialize graphics API
    if (!GraphicsAPIFactory::GetInstance().Initialize()) {
        std::cout << "Failed to initialize graphics API" << std::endl;
        return 1;
    }
    
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cout << "Failed to get graphics API" << std::endl;
        return 1;
    }
    
    // Initialize GUI
    // Use C++11 compatible way to create unique_ptr
    gui.reset(new GUI());
    
    // Create editor panel
    std::unique_ptr<Panel> editorPanel(new Panel(10, 10, 200, 580));
    
    // Create play button
    std::unique_ptr<Button> playButton(new Button(20, 20, 80, 30, "Play"));
    
    // Set button click handler
    playButton->SetOnClick([]() {
        if (EngineCondition::IsInEditor()) {
            // Using available EngineCondition API
            std::cout << "Entering play mode" << std::endl;
        }
    });
    
    // Add button to panel and panel to GUI
    editorPanel->AddElement(std::move(playButton));
    gui->AddElement(std::move(editorPanel));

    // Create window using the graphics API wrapper
    if (!graphics->CreateWindow(800, 600, "GameEngineSavi Engine")) {
        std::cout << "Failed to create window" << std::endl;
        return 1;
    }
    
    // Set up the viewport
    graphics->SetViewport(0, 0, 800, 600);
    
    // Main loop
    bool running = true;
    TimeManager timeManager;
    
    while (running) {
        // Poll events
        graphics->PollEvents();
        
        // Check if window is still open
        if (!graphics->IsWindowOpen()) {
            running = false;
            continue;
        }
        
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // If we're in editor mode, show a message
        if (EngineCondition::IsInEditor()) {
            std::cout << "In editor mode..." << std::endl;
            // In a real implementation, this would trigger the compilation process
            // For now, we'll just wait a bit
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        
        // Clear the screen
        graphics->SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        graphics->Clear(true, true); // Clear color and depth buffers
        
        // Draw GUI
        gui->Draw();
        
        // Swap buffers
        graphics->SwapBuffers();
    }
    
    // Cleanup
    graphics->DestroyWindow();
    
    return 0;
}

// End of main function
