#ifndef PYRAMID_H
#define PYRAMID_H

#include <vector>
#include "Vector3.h"
#include "Model.h"
#include "Triangle.h"

// Forward declaration
class Face;

class Pyramid : public Model {
public:
    std::vector<Triangle> triangles;
    
    Pyramid() {}
    
    Pyramid(const std::vector<Triangle>& _triangles, Vector3 _rotation, Vector3 _position)
        : Model(), triangles(_triangles) {
        // Store rotation and position in member variables
    }
    
    // Constructor that takes faces (for compatibility)
    Pyramid(const std::vector<Face>& faces, Vector3 _rotation, Vector3 _position)
        : Model() {
        // Store rotation and position in member variables
        // Convert faces to triangles if needed
    }
    
    void draw(const std::vector<PointLight>& pointLights) {
        // Rendering code for pyramid
        // This is a simplified version for the Linux demo
    }
};

#endif // PYRAMID_H
