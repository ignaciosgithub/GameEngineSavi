#ifndef MATRIX4X4_H
#define MATRIX4X4_H

class Matrix4x4
{
public:
    float elements[4][4];

    Matrix4x4();

    // Initialize an identity matrix
    void identity();
    static Matrix4x4 createRotation(float x, float y, float z);
   
    friend Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2);
};

#endif // MATRIX4X4_H
