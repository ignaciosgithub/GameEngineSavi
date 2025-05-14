#pragma once

#include "../Vector3.h"
#include "../GUI/GUI.h"
#include <vector>
#include <string>

class GameObject;
class Scene;

class HierarchyPanel : public Panel {
public:
    HierarchyPanel(int x, int y, int width, int height);
    ~HierarchyPanel();
    
    void Update(float deltaTime);
    void Draw() override;  // Renamed from Render to match GUIElement interface
    void Resize(int x, int y, int width, int height);
    bool HandleInput(int x, int y, bool clicked) override;
    
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);
    void RenameGameObject(GameObject* gameObject, const std::string& newName);
    GameObject* GetSelectedGameObject() const;
    void UpdateFromScene();
    
private:
    std::vector<GameObject*> gameObjects;
    GameObject* selectedGameObject;
    bool draggingObject;
    bool renamingObject;
    std::string renameBuffer;
    int dragStartY;
    void DrawGameObjectHierarchy(GameObject* gameObject, int indentLevel);
    Scene* GetScene() const;
};
