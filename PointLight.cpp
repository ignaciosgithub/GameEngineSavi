#include"Vector3.h"
float q_rsqrt(float number)
{
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y  = number;
  i  = * ( long * ) &y;                       // evil floating point bit level hacking
  i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
  y  = * ( float * ) &i;
  y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
  // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

  return y;
}
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
    float Intensity(float distance) const{
        return 1.0f / (constant + linear * distance + quadratic * (distance * distance));
    }
        float SQIntensity(float sqdistance) const{
        return 1.0f / (constant + linear * 1/q_rsqrt(sqdistance)+ quadratic * (sqdistance));
    }
};
