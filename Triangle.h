#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"

class Triangle
{
public:
    Vector3 vertices[3];
   
    Triangle() {}
   
    Triangle(Vector3 v1, Vector3 v2, Vector3 v3);
};

#endif // TRIANGLE_H
