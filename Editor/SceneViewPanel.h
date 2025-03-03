#pragma once

#include "../Vector3.h"

class SceneViewPanel {
public:
    SceneViewPanel(int x, int y, int width, int height);
    ~SceneViewPanel();
    
    void Update(float deltaTime);
    void Render();
    void Resize(int x, int y, int width, int height);
    
private:
    int x;
    int y;
    int width;
    int height;
};
