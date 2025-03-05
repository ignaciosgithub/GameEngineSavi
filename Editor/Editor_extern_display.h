#pragma once

#include "../Vector3.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../EngineCondition.h"
#include <iostream>

class Editor_extern_display {
public:
    Editor_extern_display(int width, int height);
    ~Editor_extern_display();
    
    void Initialize();
    void Update(float deltaTime);
    void Render();
    void Resize(int newWidth, int newHeight);
    
    Camera* GetCamera() const { return camera; }
    Scene* GetScene() const { return scene; }
    
    static Editor_extern_display* GetInstance() { return instance; }
    
private:
    int width;
    int height;
    
    static Editor_extern_display* instance;
    
    Camera* camera;
    Scene* scene;
};
