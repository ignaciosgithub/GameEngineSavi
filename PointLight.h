float q_rsqrt(float number);
class PointLight
{
public:
    // Position and color of the light
    Vector3 position;
    Vector3 color;
   
    // Light constants
    float constant;
    float linear;
    float quadratic;
   
    PointLight() : position(0, 0, 0), color(0, 0.1f, 0.1f), constant(1.0f), linear(0.09f), quadratic(0.032f) {}
   
    PointLight(float _constant, float _linear, float _quadratic)
        : constant(_constant), linear(_linear), quadratic(_quadratic) {}
   
    // Light intensity calculation
    float Intensity(float distance) const;
        float SQIntensity(float sqdistance) const;
};
