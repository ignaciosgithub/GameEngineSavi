#include "Editor.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "ProjectPanel.h"
#include "SceneViewPanel.h"
#include "../Camera.h"
#include "../Scene.h"
#include "../GameObject.h"
#include "../EngineCondition.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../TimeManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

// Initialize static instance
Editor* Editor::instance = nullptr;

Editor::Editor(int width, int height) : width(width), height(height), selectedGameObject(nullptr), windowOpen(false) {
    std::cout << "Creating editor..." << std::endl;
    
    // Set instance
    instance = this;
    
    // Set engine condition to editor mode
    EngineCondition::SetInEditor(true);
    
    // Create editor camera
    editorCamera = new Camera();
    editorCamera->SetPosition(Vector3(0, 2, 5));
    editorCamera->LookAt(Vector3(0, 0, 0));
    
    // Create panels
    hierarchyPanel = new HierarchyPanel(0, 0, 200, height);
    inspectorPanel = new InspectorPanel(width - 300, 0, 300, height);
    projectPanel = new ProjectPanel(200, height - 200, width - 500, 200);
    sceneViewPanel = new SceneViewPanel(200, 0, width - 500, height - 200);
    
    // Create scene
    scene = new Scene();
    scene->Initialize();
    
    // Set main camera
    scene->SetMainCamera(editorCamera);
    
    std::cout << "Editor created successfully." << std::endl;
}

Editor::~Editor() {
    std::cout << "Destroying editor..." << std::endl;
    
    // Delete panels
    if (hierarchyPanel) {
        delete hierarchyPanel;
        hierarchyPanel = nullptr;
    }
    
    if (inspectorPanel) {
        delete inspectorPanel;
        inspectorPanel = nullptr;
    }
    
    if (projectPanel) {
        delete projectPanel;
        projectPanel = nullptr;
    }
    
    if (sceneViewPanel) {
        delete sceneViewPanel;
        sceneViewPanel = nullptr;
    }
    
    // Delete camera
    if (editorCamera) {
        delete editorCamera;
        editorCamera = nullptr;
    }
    
    // Delete scene
    if (scene) {
        delete scene;
        scene = nullptr;
    }
    
    // Reset engine condition
    EngineCondition::SetInEditor(false);
    
    std::cout << "Editor destroyed successfully." << std::endl;
}

void Editor::Initialize() {
    
    if (scene) {
        scene->CreateDefaultObjects();
    }
}

void Editor::Update(float deltaTime) {
    // Update camera
    if (editorCamera) {
        // Update camera logic
    }
    
    // Update scene
    if (scene) {
        scene->Update(deltaTime);
    }
    
    // Update panels
    if (hierarchyPanel) {
        // Update hierarchy panel with scene objects before updating
        hierarchyPanel->UpdateFromScene();
        hierarchyPanel->Update(deltaTime);
    }
    
    if (inspectorPanel) {
        inspectorPanel->Update(deltaTime);
    }
    
    if (projectPanel) {
        projectPanel->Update(deltaTime);
    }
    
    if (sceneViewPanel) {
        sceneViewPanel->Update(deltaTime);
    }
}

void Editor::Render() {
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cout << "Failed to get graphics API" << std::endl;
        return;
    }
    
    // Clear the screen
    graphics->SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    graphics->Clear(true, true); // Clear color and depth buffers
    
    // Render scene
    if (scene) {
        scene->RenderScene();
    }
    
    // Set up 2D rendering for GUI elements
    graphics->Begin2D();
    
    // Render panels
    if (hierarchyPanel) {
        hierarchyPanel->Draw(); // Using Draw() instead of Render() to match Panel interface
    }
    
    if (inspectorPanel) {
        inspectorPanel->Render();
    }
    
    if (projectPanel) {
        projectPanel->Render();
    }
    
    if (sceneViewPanel) {
        sceneViewPanel->Render();
    }
    
    graphics->End2D();
}

void Editor::Resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    
    // Resize panels
    if (hierarchyPanel) {
        hierarchyPanel->Resize(0, 0, 200, height);
    }
    
    if (inspectorPanel) {
        inspectorPanel->Resize(width - 300, 0, 300, height);
    }
    
    if (projectPanel) {
        projectPanel->Resize(200, height - 200, width - 500, 200);
    }
    
    if (sceneViewPanel) {
        sceneViewPanel->Resize(200, 0, width - 500, height - 200);
    }
}

void Editor::SetSelectedGameObject(GameObject* gameObject) {
    selectedGameObject = gameObject;
    
    // Update inspector panel
    if (inspectorPanel) {
        inspectorPanel->SetSelectedGameObject(gameObject);
    }
}

bool Editor::CreateWindow(const char* title) {
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cout << "Failed to get graphics API" << std::endl;
        return false;
    }
    
    // Create window
    if (!graphics->CreateWindow(width, height, title)) {
        std::cout << "Failed to create window" << std::endl;
        return false;
    }
    
    // Set up the viewport
    graphics->SetViewport(0, 0, width, height);
    
    windowOpen = true;
    return true;
}

void Editor::DestroyWindow() {
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cout << "Failed to get graphics API" << std::endl;
        return;
    }
    
    // Destroy window
    graphics->DestroyWindow();
    windowOpen = false;
}

bool Editor::IsWindowOpen() const {
    return windowOpen;
}

void Editor::HandleInput(int x, int y, bool clicked) {
    
    // Check if the mouse is over the hierarchy panel
    if (hierarchyPanel) {
        if (x >= 0 && x < 200 && y >= 0 && y < height) {
            if (hierarchyPanel->HandleInput(x, y, clicked)) {
                // Update selected game object from hierarchy panel
                GameObject* selectedObject = hierarchyPanel->GetSelectedGameObject();
                if (selectedObject != selectedGameObject) {
                    SetSelectedGameObject(selectedObject);
                }
                return;
            }
        }
    }
    
    // Check if the mouse is over the inspector panel
    if (inspectorPanel) {
        if (x >= width - 300 && x < width && y >= 0 && y < height) {
            return;
        }
    }
    
    // Check if the mouse is over the project panel
    if (projectPanel) {
        if (x >= 200 && x < width - 300 && y >= height - 200 && y < height) {
            return;
        }
    }
    
    // Check if the mouse is over the scene view panel
    if (sceneViewPanel) {
        if (x >= 200 && x < width - 300 && y >= 0 && y < height - 200) {
            return;
        }
    }
}

void Editor::RunMainLoop() {
    std::cout << "Editor::RunMainLoop - Starting main loop" << std::endl;
    
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cout << "Editor::RunMainLoop - Failed to get graphics API" << std::endl;
        return;
    }
    
    std::cout << "Editor::RunMainLoop - Graphics API obtained successfully" << std::endl;
    
    std::cout << "Editor::RunMainLoop - Performing initial render" << std::endl;
    float initialDeltaTime = 0.016f;
    Update(initialDeltaTime);
    Render();
    graphics->SwapBuffers();
    
    std::atomic<int> mouseX(0);
    std::atomic<int> mouseY(0);
    std::atomic<bool> mouseClicked(false);
    
    std::thread eventPollingThread([this, graphics, &mouseX, &mouseY, &mouseClicked]() {
        std::cout << "Event polling thread started" << std::endl;
        while (windowOpen) {
            graphics->PollEvents();
            
            // Get mouse position and button state from graphics API
            int x = 0, y = 0;
            bool clicked = false;
            
            // In a real implementation, we would get these from the graphics API
            graphics->GetMousePosition(x, y);
            clicked = graphics->IsMouseButtonPressed(0); // Left mouse button
            
            // Update atomic variables for main thread to use
            mouseX.store(x);
            mouseY.store(y);
            mouseClicked.store(clicked);
            
            // Check if window is still open
            if (!graphics->IsWindowOpen()) {
                std::cout << "Event polling thread - Window closed, exiting" << std::endl;
                windowOpen = false;
                break;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Higher polling rate
        }
        std::cout << "Event polling thread exited" << std::endl;
    });
    
    const auto frameTime = std::chrono::milliseconds(16); // Target ~60 FPS
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto lastInputTime = lastFrameTime;
    
    std::cout << "Editor::RunMainLoop - Starting render loop" << std::endl;
    while (windowOpen) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime);
        auto inputElapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastInputTime);
        
        if (inputElapsedTime >= std::chrono::milliseconds(8)) {
            HandleInput(mouseX.load(), mouseY.load(), mouseClicked.load());
            lastInputTime = currentTime;
        }
        
        if (elapsedTime < frameTime) {
            std::this_thread::sleep_for(frameTime - elapsedTime);
            currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFrameTime);
        }
        
        float deltaTime = elapsedTime.count() / 1000.0f;
        lastFrameTime = currentTime;
        
        if (scene && scene->GetTimeManager()) {
            scene->GetTimeManager()->Update();
            deltaTime = scene->GetTimeManager()->GetDeltaTime();
        }
        
        // Update editor
        Update(deltaTime);
        
        // Render editor - this now happens independently of input processing
        Render();
        
        // Swap buffers
        graphics->SwapBuffers();
        
        std::cout << "Frame rendered." << std::endl;
    }
    
    if (eventPollingThread.joinable()) {
        eventPollingThread.join();
    }
}
