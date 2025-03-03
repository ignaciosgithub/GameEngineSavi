#pragma once

#include "../Vector3.h"

class GameObject;

class HierarchyPanel {
public:
    HierarchyPanel(int x, int y, int width, int height);
    ~HierarchyPanel();
    
    void Update(float deltaTime);
    void Render();
    void Resize(int x, int y, int width, int height);
    
    void AddGameObject(GameObject* gameObject);
    void RemoveGameObject(GameObject* gameObject);
    
private:
    int x;
    int y;
    int width;
    int height;
};
