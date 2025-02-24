#import<math.h>
class Vector3
{
public:
    float x, y, z;
   
    Vector3() : x(0), y(0), z(0) {}
   
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
   
    /* Function to calculate and return the magnitude of the vector */
    float magnitude()
    {
        return sqrt(x*x + y*y + z*z);
    }    
   
    /* Function to normalize the vector */
    void normalize()
    {
        float mag = magnitude();
        x /= mag;
        y /= mag;
        z /= mag;
    }
float distance(const Vector3& other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}
float sqdist(const Vector3& other) const
{
    float dx = x - other.x;
    float dy = y - other.y;
    float dz = z - other.z;
    return dx*dx + dy*dy + dz*dz;

}
 Vector3 clamp(float min, float max) const {
        Vector3 result;
        result.x = std::fmax(min, std::fmin(max, x));
        result.y = std::fmax(min, std::fmin(max, y));
        result.z = std::fmax(min, std::fmin(max, z));
        return result;
    }
};