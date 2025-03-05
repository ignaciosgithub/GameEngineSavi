#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vector3.h"

class PointLight {
private:
    Vector3 position;
    Vector3 color;
    float intensity;
    float range;
    
public:
    // Constructor
    PointLight() : position(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), intensity(1.0f), range(10.0f) {}
    
    // Constructor with parameters
    PointLight(const Vector3& pos, const Vector3& col, float intens, float rng)
        : position(pos), color(col), intensity(intens), range(rng) {}
    
    // Copy constructor
    PointLight(const PointLight& other)
        : position(other.position), color(other.color), intensity(other.intensity), range(other.range) {}
    
    // Assignment operator
    PointLight& operator=(const PointLight& other) {
        if (this != &other) {
            position = other.position;
            color = other.color;
            intensity = other.intensity;
            range = other.range;
        }
        return *this;
    }
    
    // Setter methods
    void SetColor(const Vector3& col) { color = col; }
    void SetIntensity(float intens) { intensity = intens; }
    void SetRange(float rng) { range = rng; }
    void SetPosition(const Vector3& pos) { position = pos; }
    
    // Getter methods
    Vector3 GetPosition() const { return position; }
    Vector3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }
    float GetRange() const { return range; }
};

#endif // POINTLIGHT_H
