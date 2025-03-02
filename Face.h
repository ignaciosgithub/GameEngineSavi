#ifndef FACE_H
#define FACE_H

#include <vector>
#include "Triangle.h"

class Face
{
public:
    std::vector<Triangle> triangles;
    std::vector<int> vertex_indices;
    std::vector<int> texcoord_indices;
    std::vector<int> normal_indices;

    Face() {}

    Face(std::vector<Triangle> _triangles) : triangles(_triangles) {}
};

#endif // FACE_H
