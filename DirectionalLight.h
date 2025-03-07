#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include "Vector3.h"

class DirectionalLight {
private:
    Vector3 direction;
    Vector3 color;
    float intensity;
    
public:
    // Constructor
    DirectionalLight() : direction(0.0f, -1.0f, 0.0f), color(1.0f, 1.0f, 1.0f), intensity(1.0f) {}
    
    // Constructor with parameters
    DirectionalLight(const Vector3& dir, const Vector3& col, float intens)
        : direction(dir), color(col), intensity(intens) {}
    
    // Copy constructor
    DirectionalLight(const DirectionalLight& other)
        : direction(other.direction), color(other.color), intensity(other.intensity) {}
    
    // Assignment operator
    DirectionalLight& operator=(const DirectionalLight& other) {
        if (this != &other) {
            direction = other.direction;
            color = other.color;
            intensity = other.intensity;
        }
        return *this;
    }
    
    // Setter methods
    void SetDirection(const Vector3& dir) { direction = dir.normalized(); }
    void SetColor(const Vector3& col) { color = col; }
    void SetIntensity(float intens) { intensity = intens; }
    
    // Getter methods
    Vector3 GetDirection() const { return direction; }
    Vector3 GetColor() const { return color; }
    float GetIntensity() const { return intensity; }
};

#endif // DIRECTIONALLIGHT_H
