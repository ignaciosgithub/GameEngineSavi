#pragma once
#include "../Vector3.h"
#include <vector>

class HeadlessModel {
public:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    Vector3 position;
    Vector3 scale;
    Vector3 rotation;

    HeadlessModel() : position(0,0,0), scale(1,1,1), rotation(0,0,0) {}
};
