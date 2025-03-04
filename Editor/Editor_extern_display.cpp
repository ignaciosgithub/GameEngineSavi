#include "Editor_extern_display.h"
#include "../EngineCondition.h"
#include "../ThirdParty/OpenGL/include/GL/platform_gl.h"
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
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up viewport
    glViewport(0, 0, width, height);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
    
    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Set camera view
    Vector3 pos = camera->GetPosition();
    Vector3 target = Vector3(0, 0, 0);  // Look at origin
    Vector3 up(0, 1, 0);
    
    gluLookAt(
        pos.x, pos.y, pos.z,
        target.x, target.y, target.z,
        up.x, up.y, up.z
    );
    
    // Render scene
    if (scene) {
        scene->RenderScene();
    }
}

void Editor_extern_display::Resize(int newWidth, int newHeight) {
    width = newWidth;
    height = newHeight;
    
    // Update viewport
    glViewport(0, 0, width, height);
    
    // Update projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);
}
