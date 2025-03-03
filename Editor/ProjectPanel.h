#pragma once

#include "../Vector3.h"

class ProjectPanel {
public:
    ProjectPanel(int x, int y, int width, int height);
    ~ProjectPanel();
    
    void Update(float deltaTime);
    void Render();
    void Resize(int x, int y, int width, int height);
    
private:
    int x;
    int y;
    int width;
    int height;
};
