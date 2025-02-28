#include "Editor.h"
#include "HierarchyPanel.h"
#include "SceneViewPanel.h"
#include "InspectorPanel.h"
#include "ProjectPanel.h"
#include "../EngineCondition.h"
#include "../ProjectSettings/ProjectSettings.h"
#include "../GUI/TextField.h"
#include <iostream>

Editor::Editor(int width, int height) 
    : windowWidth(width), 
      windowHeight(height), 
      selectedObject(nullptr), 
      isPlaying(false), 
      isDraggingObject(false) {
    
    // Create GUI
    gui = std::unique_ptr<GUI>(new GUI());
    
    // Create scene
    scene = std::unique_ptr<Scene>(new Scene());
    
    // Create editor camera
    editorCamera = std::unique_ptr<Camera>(new Camera());
    editorCamera->SetPosition(Vector3(0, 2, 5));
    editorCamera->LookAt(Vector3(0, 0, 0));
    editorCamera->fieldOfView = 45.0f;
}

Editor::~Editor() {
    // Clean up resources
}

void Editor::Initialize() {
    // Initialize the scene
    scene->Initialize();
    
    // Create editor panels
    hierarchyPanel = std::unique_ptr<HierarchyPanel>(new HierarchyPanel(10, 10, 200, windowHeight - 20, this));
    sceneViewPanel = std::unique_ptr<SceneViewPanel>(new SceneViewPanel(220, 10, windowWidth - 430, windowHeight - 20, this));
    inspectorPanel = std::unique_ptr<InspectorPanel>(new InspectorPanel(windowWidth - 200, 10, 190, windowHeight - 20, this));
    projectPanel = std::unique_ptr<ProjectPanel>(new ProjectPanel(10, windowHeight - 200, windowWidth - 20, 190, this));
    
    // Add panels to GUI
    gui->AddElement(hierarchyPanel.release());
    gui->AddElement(sceneViewPanel.release());
    gui->AddElement(inspectorPanel.release());
    gui->AddElement(projectPanel.release());
    
    // Set engine condition to editor mode
    EngineCondition::EnterEditMode();
    
    // Create default scene
    CreateDefaultScene();
    
    std::cout << "Editor initialized" << std::endl;
}

void Editor::CreateDefaultScene() {
    // Create a default point light
    GameObject* lightObj = new GameObject("Default Light");
    PointLight light;
    light.SetColor(Vector3(1.0f, 1.0f, 1.0f));
    light.SetIntensity(1.0f);
    lightObj->AddLight(light);
    lightObj->SetPosition(Vector3(0, 5, 0));
    scene->AddGameObject(lightObj);
    
    // Create a default cube
    GameObject* cubeObj = new GameObject("Default Cube");
    // Load cube model
    Model* cubeModel = new Model();
    cubeModel->LoadFromFile("test_assets/cube.obj");
    cubeObj->AddMesh(cubeModel);
    cubeObj->SetPosition(Vector3(0, 0, 0));
    scene->AddGameObject(cubeObj);
    
    std::cout << "Default scene created" << std::endl;
}

void Editor::Update(float deltaTime) {
    // Update the scene if playing
    if (isPlaying) {
        scene->Update(deltaTime);
    }
    
    // Update editor camera
    // This would handle camera movement based on input
    // For now, we'll just update the camera's view matrix
    editorCamera->Update(deltaTime);
}

void Editor::Render() {
    // Set the camera for rendering
    scene->SetMainCamera(editorCamera.get());
    
    // Render the scene
    scene->Render();
    
    // Render the GUI
    gui->Draw();
}

void Editor::HandleInput(int x, int y, bool clicked) {
    // Handle GUI input first
    gui->HandleInput(x, y, clicked);
    
    // Handle scene input if not handled by GUI
    // This would include object selection, camera movement, etc.
}

void Editor::HandleDragAndDrop(const std::string& filePath) {
    // Check if the file is an OBJ file
    if (filePath.substr(filePath.find_last_of(".") + 1) == "obj") {
        // Load the OBJ file
        GameObject* obj = LoadObjFile(filePath);
        if (obj) {
            // Add the object to the scene
            scene->AddGameObject(obj);
            // Select the new object
            SelectObject(obj);
            
            std::cout << "Loaded OBJ file: " << filePath << std::endl;
        }
    }
}

GameObject* Editor::LoadObjFile(const std::string& filePath) {
    // Create a new game object
    GameObject* obj = new GameObject(filePath.substr(filePath.find_last_of("/\\") + 1));
    
    // Load the model
    Model* model = new Model();
    if (model->LoadFromFile(filePath)) {
        // Add the model to the game object
        obj->AddMesh(model);
        return obj;
    } else {
        // Failed to load model
        delete obj;
        delete model;
        std::cout << "Failed to load OBJ file: " << filePath << std::endl;
        return nullptr;
    }
}

void Editor::SelectObject(GameObject* object) {
    selectedObject = object;
    
    // Update inspector panel with selected object
    if (inspectorPanel) {
        inspectorPanel->SetSelectedObject(object);
    }
    
    std::cout << "Selected object: " << (object ? object->GetName() : "None") << std::endl;
}

GameObject* Editor::GetSelectedObject() const {
    return selectedObject;
}

Camera* Editor::GetEditorCamera() const {
    return editorCamera.get();
}

Scene* Editor::GetScene() const {
    return scene.get();
}

void Editor::TogglePlay() {
    isPlaying = !isPlaying;
    
    if (isPlaying) {
        // Enter play mode
        EngineCondition::EnterPlayMode();
        std::cout << "Entered play mode" << std::endl;
    } else {
        // Enter edit mode
        EngineCondition::EnterEditMode();
        std::cout << "Entered edit mode" << std::endl;
    }
}

bool Editor::IsPlaying() const {
    return isPlaying;
}
