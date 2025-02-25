#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

class Camera
{
public:
    Vector3 position;
    Vector3 lookDirection;
    float fieldOfView;
   
    Camera() : position(0, 0, 0), lookDirection(0, 1, -1), fieldOfView(90){}
   
    Camera(Vector3 pos, Vector3 dir, float fov) : position(pos), lookDirection(dir), fieldOfView(fov) {}
};

#endif // CAMERA_H
