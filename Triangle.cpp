#include "Vector3.h"

class Triangle
{
public:
    Vector3 vertices[3];
   
    Triangle() {}
   
    Triangle(Vector3 v1, Vector3 v2, Vector3 v3)
    {
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;
    }
};