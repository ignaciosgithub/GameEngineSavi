#ifndef SIMPLIFIED_MODEL_H
#define SIMPLIFIED_MODEL_H

#include "Vector3.h"
#include <vector>
#include <string>

// A simplified Model class for the Linux demo that doesn't require OpenGL
class SimplifiedModel {
public:
    std::vector<float> vertices; // Stores the vertices
    std::vector<unsigned int> indices; // Stores the indices representing triangles
    Vector3 rotation, position;
    
    SimplifiedModel() {}

    SimplifiedModel(const Vector3& _position, const Vector3& _rotation)
        : position(_position), rotation(_rotation) {
    }
    
    // Create a simple cube model
    void createCube() {
        // Vertices for a cube (8 vertices)
        vertices = {
            -0.5f, -0.5f, -0.5f,  // 0
             0.5f, -0.5f, -0.5f,  // 1
             0.5f,  0.5f, -0.5f,  // 2
            -0.5f,  0.5f, -0.5f,  // 3
            -0.5f, -0.5f,  0.5f,  // 4
             0.5f, -0.5f,  0.5f,  // 5
             0.5f,  0.5f,  0.5f,  // 6
            -0.5f,  0.5f,  0.5f   // 7
        };
        
        // Indices for 12 triangles (6 faces)
        indices = {
            0, 1, 2, 0, 2, 3,  // Front face
            4, 5, 6, 4, 6, 7,  // Back face
            0, 1, 5, 0, 5, 4,  // Bottom face
            2, 3, 7, 2, 7, 6,  // Top face
            0, 3, 7, 0, 7, 4,  // Left face
            1, 2, 6, 1, 6, 5   // Right face
        };
    }
    
    // Create a simple pyramid model
    void createPyramid() {
        // Vertices for a pyramid (5 vertices)
        vertices = {
            -0.5f, -0.5f, -0.5f,  // 0 - Base
             0.5f, -0.5f, -0.5f,  // 1 - Base
             0.5f, -0.5f,  0.5f,  // 2 - Base
            -0.5f, -0.5f,  0.5f,  // 3 - Base
             0.0f,  0.5f,  0.0f   // 4 - Top
        };
        
        // Indices for 6 triangles (5 faces)
        indices = {
            0, 1, 2, 0, 2, 3,  // Base (2 triangles)
            0, 1, 4,           // Side 1
            1, 2, 4,           // Side 2
            2, 3, 4,           // Side 3
            3, 0, 4            // Side 4
        };
    }
    
    // Calculate bounding box
    void getBoundingBox(Vector3& min, Vector3& max) const {
        if (vertices.empty()) {
            min = Vector3(0, 0, 0);
            max = Vector3(0, 0, 0);
            return;
        }
        
        min = Vector3(vertices[0], vertices[1], vertices[2]);
        max = Vector3(vertices[0], vertices[1], vertices[2]);
        
        for (size_t i = 0; i < vertices.size(); i += 3) {
            Vector3 vertex(vertices[i], vertices[i+1], vertices[i+2]);
            
            // Apply position offset
            vertex += position;
            
            // Update min and max
            min.x = std::min(min.x, vertex.x);
            min.y = std::min(min.y, vertex.y);
            min.z = std::min(min.z, vertex.z);
            
            max.x = std::max(max.x, vertex.x);
            max.y = std::max(max.y, vertex.y);
            max.z = std::max(max.z, vertex.z);
        }
    }
};

#endif // SIMPLIFIED_MODEL_H
