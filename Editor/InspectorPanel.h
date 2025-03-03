#pragma once

#include "../Vector3.h"

class GameObject;

class InspectorPanel {
public:
    InspectorPanel(int x, int y, int width, int height);
    ~InspectorPanel();
    
    void Update(float deltaTime);
    void Render();
    void Resize(int x, int y, int width, int height);
    
    void SetSelectedGameObject(GameObject* gameObject);
    
private:
    int x;
    int y;
    int width;
    int height;
    
    GameObject* selectedGameObject;
};
