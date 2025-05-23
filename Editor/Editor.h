#pragma once

#include "../Vector3.h"

class Camera;
class Scene;
class GameObject;
class HierarchyPanel;
class InspectorPanel;
class ProjectPanel;
class SceneViewPanel;

class Editor {
public:
    Editor(int width, int height);
    ~Editor();
    
    void Update(float deltaTime);
    void Render();
    void Resize(int newWidth, int newHeight);
    void HandleInput(int x, int y, bool clicked);
    
    Camera* GetEditorCamera() const { return editorCamera; }
    Scene* GetScene() const { return scene; }
    
    void SetSelectedGameObject(GameObject* gameObject);
    GameObject* GetSelectedGameObject() const { return selectedGameObject; }
    
    static Editor* GetInstance() { return instance; }
    
    void Initialize();
    
    // Window management methods
    bool CreateWindow(const char* title = "GameEngineSavi Editor");
    void DestroyWindow();
    bool IsWindowOpen() const;
    void RunMainLoop();
    
private:
    int width;
    int height;
    
    static Editor* instance;
    
    Camera* editorCamera;
    Scene* scene;
    
    HierarchyPanel* hierarchyPanel;
    InspectorPanel* inspectorPanel;
    ProjectPanel* projectPanel;
    SceneViewPanel* sceneViewPanel;
    
    GameObject* selectedGameObject;
    
    // Window management
    bool windowOpen;
};
