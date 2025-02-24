#include<math.h>
class Matrix4x4
{
public:
    float elements[4][4];

    Matrix4x4()
    {
        identity();
    }

    // Initialize an identity matrix
    void identity()
    {
        for(int i = 0; i<4; ++i)
            for(int j = 0; j<4; ++j)
                elements[i][j] = (i==j) ? 1.0f : 0.0f;
    }

    static Matrix4x4 createRotation(float x, float y, float z)
    {
        Matrix4x4 rx, ry, rz;
        const float xRad = x * /*M_PI*/ 3.14159265358979323846f / 180.0f;
        const float yRad = y * /*M_PI*/ 3.14159265358979323846f / 180.0f;
        const float zRad = z * /*M_PI*/ 3.14159265358979323846f / 180.0f;

        rx.identity();
        ry.identity();
        rz.identity();

        rx.elements[1][1] = cos(xRad); rx.elements[1][2] = -sin(xRad);
        rx.elements[2][1] = sin(xRad); rx.elements[2][2] = cos(xRad);

        ry.elements[0][0] = cos(yRad); ry.elements[0][2] = sin(yRad);
        ry.elements[2][0] = -sin(yRad); ry.elements[2][2] = cos(yRad);

        rz.elements[0][0] = cos(zRad); rz.elements[0][1] = -sin(zRad);
        rz.elements[1][0] = sin(zRad); rz.elements[1][1] = cos(zRad);

        // Now we multiply the three matrices
        Matrix4x4 result = rz * ry * rx;
        return result;
    }
   
    friend Matrix4x4 operator*(const Matrix4x4 &m1, const Matrix4x4 &m2)
    {
        Matrix4x4 result;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                result.elements[i][j] = 0;
                for (int k = 0; k < 4; k++)
                {
                    result.elements[i][j] += m1.elements[i][k] * m2.elements[k][j];
                }
            }
        }
        return result;
    }
};