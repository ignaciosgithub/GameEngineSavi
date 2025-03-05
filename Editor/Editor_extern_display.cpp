#include "Editor_extern_display.h"
#include "../EngineCondition.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include <iostream>

// Initialize static instance
Editor_extern_display* Editor_extern_display::instance = nullptr;

Editor_extern_display::Editor_extern_display(int width, int height) : width(width), height(height) {
    std::cout << "Creating external display editor..." << std::endl;
    
    // Set instance
    instance = this;
    
    // Set engine condition to editor mode
    EngineCondition::SetInEditor(true);
    
    // Create camera
    camera = new Camera();
    camera->SetPosition(Vector3(0, 2, 5));
    camera->LookAt(Vector3(0, 0, 0));
    camera->fieldOfView = 60.0f;
    camera->SetNearPlane(0.1f);
    camera->SetFarPlane(1000.0f);
    
    // Create scene
    scene = new Scene();
    scene->Initialize();
    
    // Set main camera
    scene->SetMainCamera(camera);
    
    std::cout << "External display editor created successfully." << std::endl;
}

Editor_extern_display::~Editor_extern_display() {
    std::cout << "Destroying external display editor..." << std::endl;
    
    // Delete camera
    if (camera) {
        delete camera;
        camera = nullptr;
    }
    
    // Delete scene
    if (scene) {
        delete scene;
        scene = nullptr;
    }
    
    // Reset engine condition
    EngineCondition::SetInEditor(false);
    
    std::cout << "External display editor destroyed successfully." << std::endl;
}

void Editor_extern_display::Initialize() {
    // Create default objects in the scene
    if (scene) {
        scene->CreateDefaultObjects();
    }
}

void Editor_extern_display::Update(float deltaTime) {
    // Update camera
    if (camera) {
        // Update camera logic if needed
    }
    
    // Update scene
    if (scene) {
        scene->Update(deltaTime);
    }
}

void Editor_extern_display::Render() {
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return;
    }
    
    // Clear the screen
    graphics->SetClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    graphics->Clear(true, true);
    
    // Set up viewport
    graphics->SetViewport(0, 0, width, height);
    
    // Set up projection matrix
    Matrix4x4 projMatrix;
    projMatrix.SetPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
    
    // Set up view matrix
    Vector3 pos = camera->GetPosition();
    Vector3 target = Vector3(0, 0, 0);  // Look at origin
    Vector3 up(0, 1, 0);
    
    Matrix4x4 viewMatrix;
    viewMatrix.SetLookAt(pos, target, up);
    
    // Set matrices using the graphics API
    graphics->SetProjectionMatrix(projMatrix);
    graphics->SetViewMatrix(viewMatrix);
    
    // Render scene
    if (scene) {
        scene->RenderScene();
    }
}

void Editor_extern_display::Resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return;
    }
    
    // Update viewport
    graphics->SetViewport(0, 0, width, height);
    
    // Update projection matrix
    Matrix4x4 projMatrix;
    projMatrix.SetPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
    graphics->SetProjectionMatrix(projMatrix);
}
