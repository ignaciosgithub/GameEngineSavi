


class Vector3
{
public:
    float x, y, z;
   
    Vector3() : x(0), y(0), z(0) {}
   
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
   
    /* Function to calculate and return the magnitude of the vector */
    float magnitude();
 
   
    /* Function to normalize the vector */
    void normalize();
float distance(const Vector3& other) const;
float sqdist(const Vector3& other) const;
Vector3 clamp(float min, float max) const;
};