#include "Editor.h"
#include "HierarchyPanel.h"
#include "InspectorPanel.h"
#include "ProjectPanel.h"
#include "SceneViewPanel.h"
#include "../Camera.h"
#include "../Scene.h"
#include "../GameObject.h"
#include "../EngineCondition.h"
#include <iostream>

Editor::Editor(int width, int height) : width(width), height(height), selectedGameObject(nullptr) {
    std::cout << "Creating editor..." << std::endl;
    
    // Set engine condition to editor mode
    EngineCondition::SetInEditor(true);
    
    // Create editor camera
    editorCamera = new Camera();
    editorCamera->SetPosition(Vector3(0, 2, -5));
    editorCamera->SetRotation(Vector3(15, 0, 0));
    
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
    // Render scene
    if (scene) {
        scene->RenderScene();
    }
    
    // Render panels
    if (hierarchyPanel) {
        hierarchyPanel->Render();
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
