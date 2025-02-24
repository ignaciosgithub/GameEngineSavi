#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <algorithm>

class Vector3
{
public:
    float x, y, z;
   
    Vector3() : x(0), y(0), z(0) {}
   
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
   
    /* Function to calculate and return the magnitude of the vector */
    float magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
   
    /* Function to normalize the vector */
    void normalize() {
        float mag = magnitude();
        if (mag > 0.0001f) {
            x /= mag;
            y /= mag;
            z /= mag;
        }
    }
    
    /* Return a normalized copy of this vector */
    Vector3 normalized() const {
        Vector3 result = *this;
        result.normalize();
        return result;
    }
    
    float distance(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
    
    float sqdist(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return dx*dx + dy*dy + dz*dz;
    }
    
    Vector3 clamp(float min, float max) const {
        Vector3 result;
        result.x = std::max(min, std::min(max, x));
        result.y = std::max(min, std::min(max, y));
        result.z = std::max(min, std::min(max, z));
        return result;
    }
    
    /* Dot product */
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    /* Cross product */
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    /* Operator overloads */
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3 operator/(float scalar) const {
        if (scalar != 0) {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        return *this;
    }
    
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vector3& operator/=(float scalar) {
        if (scalar != 0) {
            x /= scalar;
            y /= scalar;
            z /= scalar;
        }
        return *this;
    }
    
    bool operator==(const Vector3& other) const {
        return (x == other.x && y == other.y && z == other.z);
    }
    
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};

/* Allow scalar * Vector3 */
inline Vector3 operator*(float scalar, const Vector3& vec) {
    return vec * scalar;
}

#endif // VECTOR3_H
