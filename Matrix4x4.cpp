#include "Matrix4x4.h"
#include <cmath>

// Constructor
Matrix4x4::Matrix4x4() {
    // Initialize to identity matrix
    identity();
}

// Initialize an identity matrix
void Matrix4x4::identity() {
    // Set all elements to 0
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            elements[i][j] = 0.0f;
        }
    }
    
    // Set diagonal elements to 1
    elements[0][0] = 1.0f;
    elements[1][1] = 1.0f;
    elements[2][2] = 1.0f;
    elements[3][3] = 1.0f;
}

// Create a rotation matrix
Matrix4x4 Matrix4x4::createRotation(float x, float y, float z) {
    Matrix4x4 result;
    result.identity();
    
    // Convert angles from degrees to radians
    float radX = x * 3.14159f / 180.0f;
    float radY = y * 3.14159f / 180.0f;
    float radZ = z * 3.14159f / 180.0f;
    
    // Calculate sine and cosine values
    float sinX = sin(radX);
    float cosX = cos(radX);
    float sinY = sin(radY);
    float cosY = cos(radY);
    float sinZ = sin(radZ);
    float cosZ = cos(radZ);
    
    // Apply rotations (order: Z, Y, X)
    // Rotation around Z
    result.elements[0][0] = cosZ;
    result.elements[0][1] = -sinZ;
    result.elements[1][0] = sinZ;
    result.elements[1][1] = cosZ;
    
    // Rotation around Y
    Matrix4x4 rotY;
    rotY.identity();
    rotY.elements[0][0] = cosY;
    rotY.elements[0][2] = sinY;
    rotY.elements[2][0] = -sinY;
    rotY.elements[2][2] = cosY;
    
    // Rotation around X
    Matrix4x4 rotX;
    rotX.identity();
    rotX.elements[1][1] = cosX;
    rotX.elements[1][2] = -sinX;
    rotX.elements[2][1] = sinX;
    rotX.elements[2][2] = cosX;
    
    // Combine rotations
    result = rotX * rotY * result;
    
    return result;
}

// Matrix multiplication
Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2) {
    Matrix4x4 result;
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.elements[i][j] = 0.0f;
            for (int k = 0; k < 4; k++) {
                result.elements[i][j] += m1.elements[i][k] * m2.elements[k][j];
            }
        }
    }
    return result;
}

void Matrix4x4::translate(float x, float y, float z)
{
    Matrix4x4 translationMatrix;
    translationMatrix.identity();
    
    translationMatrix.elements[0][3] = x;
    translationMatrix.elements[1][3] = y;
    translationMatrix.elements[2][3] = z;
    
    *this = translationMatrix * (*this);
}

void Matrix4x4::rotateX(float angle)
{
    const float rad = angle * 3.14159265358979323846f / 180.0f;
    Matrix4x4 rotationMatrix;
    rotationMatrix.identity();
    
    rotationMatrix.elements[1][1] = cos(rad);
    rotationMatrix.elements[1][2] = -sin(rad);
    rotationMatrix.elements[2][1] = sin(rad);
    rotationMatrix.elements[2][2] = cos(rad);
    
    *this = rotationMatrix * (*this);
}

void Matrix4x4::rotateY(float angle)
{
    const float rad = angle * 3.14159265358979323846f / 180.0f;
    Matrix4x4 rotationMatrix;
    rotationMatrix.identity();
    
    rotationMatrix.elements[0][0] = cos(rad);
    rotationMatrix.elements[0][2] = sin(rad);
    rotationMatrix.elements[2][0] = -sin(rad);
    rotationMatrix.elements[2][2] = cos(rad);
    
    *this = rotationMatrix * (*this);
}

void Matrix4x4::rotateZ(float angle)
{
    const float rad = angle * 3.14159265358979323846f / 180.0f;
    Matrix4x4 rotationMatrix;
    rotationMatrix.identity();
    
    rotationMatrix.elements[0][0] = cos(rad);
    rotationMatrix.elements[0][1] = -sin(rad);
    rotationMatrix.elements[1][0] = sin(rad);
    rotationMatrix.elements[1][1] = cos(rad);
    
    *this = rotationMatrix * (*this);
}

// Set up a perspective projection matrix
void Matrix4x4::SetPerspective(float fovy, float aspectRatio, float zNear, float zFar) {
    // Clear the matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            elements[i][j] = 0.0f;
        }
    }
    
    // Calculate perspective matrix elements
    float f = 1.0f / tan(fovy * 0.5f * 3.14159f / 180.0f);
    float rangeInv = 1.0f / (zNear - zFar);
    
    elements[0][0] = f / aspectRatio;
    elements[1][1] = f;
    elements[2][2] = (zNear + zFar) * rangeInv;
    elements[2][3] = -1.0f;
    elements[3][2] = 2.0f * zNear * zFar * rangeInv;
    elements[3][3] = 0.0f;
}

// Set up a view matrix using look-at parameters
void Matrix4x4::SetLookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
    // Calculate forward, right, and up vectors
    Vector3 forward = center - eye;
    forward.normalize();
    
    Vector3 right = forward.cross(up);
    right.normalize();
    
    Vector3 upVector = right.cross(forward);
    upVector.normalize();
    
    // Clear the matrix
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            elements[i][j] = 0.0f;
        }
    }
    
    // Set matrix elements
    elements[0][0] = right.x;
    elements[0][1] = right.y;
    elements[0][2] = right.z;
    elements[1][0] = upVector.x;
    elements[1][1] = upVector.y;
    elements[1][2] = upVector.z;
    elements[2][0] = -forward.x;
    elements[2][1] = -forward.y;
    elements[2][2] = -forward.z;
    elements[3][0] = -right.dot(eye);
    elements[3][1] = -upVector.dot(eye);
    elements[3][2] = forward.dot(eye);
    elements[3][3] = 1.0f;
}
