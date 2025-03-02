#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
public:
    float x, y;
    
    Vector2() : x(0), y(0) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    
    Vector2 operator/(float scalar) const {
        if (scalar != 0) {
            return Vector2(x / scalar, y / scalar);
        }
        return *this;
    }
    
    float magnitude() const {
        return sqrt(x * x + y * y);
    }
    
    Vector2 normalized() const {
        float mag = magnitude();
        if (mag > 0) {
            return Vector2(x / mag, y / mag);
        }
        return Vector2(0, 0);
    }
};

#endif // VECTOR2_H
