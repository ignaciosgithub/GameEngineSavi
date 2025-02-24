#include"Vector3.h"
#include"Triangle.h"
#include<vector>
class Face
{
public:
    std::vector<Triangle> triangles;

    Face() {}

    Face(std::vector<Triangle> _triangles) : triangles(_triangles) {}
};