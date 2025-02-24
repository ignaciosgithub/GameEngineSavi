#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vector3.h"

class PointLight {
public:
    Vector3 position;
    Vector3 color;
    float intensity;
    float range;
    
    PointLight() : position(0, 0, 0), color(1, 1, 1), intensity(1.0f), range(10.0f) {}
    
    PointLight(const Vector3& pos, const Vector3& col, float intens = 1.0f, float rng = 10.0f)
        : position(pos), color(col), intensity(intens), range(rng) {}
    
    // Calculate intensity at a given squared distance
    float SQIntensity(float sqDistance) const {
        // Prevent division by zero
        if (sqDistance < 0.0001f) {
            return intensity;
        }
        
        // Calculate attenuation based on squared distance
        float attenuation = 1.0f / (1.0f + 0.09f * sqDistance + 0.032f * sqDistance * sqDistance);
        return intensity * attenuation;
    }
};

#endif // POINTLIGHT_H
