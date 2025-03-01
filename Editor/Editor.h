#ifndef EDITOR_H
#define EDITOR_H

#include <memory>
#include <vector>
#include <string>
#include "../GUI/GUI.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../GameObject.h"
#include "../Model.h"
#include "../PointLight.h"

// Forward declarations
class HierarchyPanel;
class SceneViewPanel;
class InspectorPanel;
class ProjectPanel;

// Main Editor class that manages the editor UI and functionality
class Editor {
private:
    std::unique_ptr<GUI> gui;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Camera> editorCamera;
    
    // Editor panels
    std::unique_ptr<HierarchyPanel> hierarchyPanel;
    std::unique_ptr<SceneViewPanel> sceneViewPanel;
    std::unique_ptr<InspectorPanel> inspectorPanel;
    std::unique_ptr<ProjectPanel> projectPanel;
    
    // Selected object in the scene
    GameObject* selectedObject;
    
    // Editor state
    bool isPlaying;
    bool isDraggingObject;
    std::string draggedFilePath;
    
    // Window dimensions
    int windowWidth;
    int windowHeight;
    
public:
    Editor(int width, int height);
    ~Editor();
    
    // Initialize the editor
    void Initialize();
    
    // Create a default scene with a point light and a cube
    void CreateDefaultScene();
    
    // Update the editor
    void Update(float deltaTime);
    
    // Render the editor
    void Render();
    
    // Handle input
    void HandleInput(int x, int y, bool clicked);
    
    // Handle drag and drop
    void HandleDragAndDrop(const std::string& filePath);
    
    // Load an OBJ file
    GameObject* LoadObjFile(const std::string& filePath);
    
    // Select an object in the scene
    void SelectObject(GameObject* object);
    
    // Get the selected object
    GameObject* GetSelectedObject() const;
    
    // Get the editor camera
    Camera* GetEditorCamera() const;
    
    // Get the scene
    Scene* GetScene() const;
    
    // Play/pause the scene
    void TogglePlay();
    
    // Check if the scene is playing
    bool IsPlaying() const;
    
    // Capture a screenshot of the editor view
    bool CaptureScreenshot(const std::string& filename);
};

#endif // EDITOR_H
