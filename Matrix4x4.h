#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3.h"

class Matrix4x4 {
public:
    // Matrix elements (row-major order)
    float elements[4][4];
    
    // Constructor
    Matrix4x4();
    
    // Initialize an identity matrix
    void identity();
    static Matrix4x4 createRotation(float x, float y, float z);
    
    // Transformation methods
    void translate(float x, float y, float z);
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);
    
    // Projection and view matrix methods
    void SetPerspective(float fovy, float aspectRatio, float zNear, float zFar);
    void SetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up);
   
    friend Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2);
};

#endif // MATRIX4X4_H
