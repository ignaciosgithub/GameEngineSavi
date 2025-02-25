#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <vector>
#include <string>
#include "../platform.h"

// Forward declarations
class Model;

class KeyFrame {
public:
    float timestamp;
    std::string objFilePath;
    std::vector<GLfloat> vertexPositions;
    
    // Constructor with timestamp and vertex positions
    KeyFrame(float time, const std::vector<GLfloat>& vertices)
        : timestamp(time), vertexPositions(vertices) {}
    
    // Constructor with timestamp and OBJ file path
    KeyFrame(float time, const std::string& objPath)
        : timestamp(time), objFilePath(objPath) {}
    
    // Load vertex positions from OBJ file
    bool LoadFromOBJ();
};

#endif // KEYFRAME_H
