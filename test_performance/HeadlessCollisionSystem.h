#pragma once
#include "../Vector3.h"
#include "HeadlessModel.h"
#include <vector>

struct HeadlessBoundingBox {
    Vector3 min;
    Vector3 max;
    
    bool Intersects(const HeadlessBoundingBox& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
};

class HeadlessCollisionSystem {
public:
    HeadlessBoundingBox CalculateBoundingBox(const HeadlessModel* model) {
        HeadlessBoundingBox box;
        box.min = Vector3(std::numeric_limits<float>::max());
        box.max = Vector3(std::numeric_limits<float>::lowest());
        
        for (size_t i = 0; i < model->vertices.size(); i += 3) {
            Vector3 vertex(model->vertices[i], 
                         model->vertices[i+1], 
                         model->vertices[i+2]);
            box.min = Vector3::Min(box.min, vertex);
            box.max = Vector3::Max(box.max, vertex);
        }
        return box;
    }
};
