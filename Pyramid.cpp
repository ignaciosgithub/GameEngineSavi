#include "platform.h"

// Platform-specific OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#include <gl/glu.h>
#else
#include <GL/glu.h>
#endif

#include "Vector3.h"
#include "Matrix4x4.h"
#include <vector>
#include "Triangle.h"
#include "Face.h"

class Pyramid
{
public:
    std::vector<Face> faces;
    Vector3 rotation; // in degrees
    Vector3 position;

    Pyramid() {}

    Pyramid(std::vector<Face> _faces, Vector3 _rotation, Vector3 _position)
     : faces(_faces), rotation(_rotation), position(_position) {}
void rotate(float x_deg, float y_deg, float z_deg)
{
    // Calculate centroid of the pyramid
    Vector3 centroid(0, 0 ,0);
    int numVertices = 0;
    for (auto & face : faces) {
        for (auto & triangle : face.triangles) {
            for (auto & vertex : triangle.vertices) {
                centroid.x += vertex.x;
                centroid.y += vertex.y;
                centroid.z += vertex.z;
                numVertices += 1;
            }
        }
    }
    centroid.x /= numVertices;
    centroid.y /= numVertices;
    centroid.z /= numVertices;

    // Translate the pyramid so that the centroid is at the origin
    translate(-centroid.x, -centroid.y, -centroid.z);

    // Apply rotation
    rotation.x += x_deg;
    rotation.y += y_deg;
    rotation.z += z_deg;

    Matrix4x4 rotationMatrix = Matrix4x4::createRotation(rotation.x, rotation.y, rotation.z);

    // Apply the rotation matrix to each vertex of each triangle
    for (auto & face : faces) {
        for (auto & triangle : face.triangles) {
            for (auto & vertex : triangle.vertices) {
                // Multiply the vertex by the rotation matrix
                Vector3 oldVec = vertex;
                vertex.x = oldVec.x * rotationMatrix.elements[0][0] + oldVec.y * rotationMatrix.elements[1][0] + oldVec.z * rotationMatrix.elements[2][0] + rotationMatrix.elements[3][0];
                vertex.y = oldVec.x * rotationMatrix.elements[0][1] + oldVec.y * rotationMatrix.elements[1][1] + oldVec.z * rotationMatrix.elements[2][1] + rotationMatrix.elements[3][1];
                vertex.z = oldVec.x * rotationMatrix.elements[0][2] + oldVec.y * rotationMatrix.elements[1][2] + oldVec.z * rotationMatrix.elements[2][2] + rotationMatrix.elements[3][2];
            }
        }
    }

    // Translate the pyramid back to its original position
    translate(centroid.x, centroid.y, centroid.z);
}
   void rotatef(float x_deg, float y_deg, float z_deg)
{
    rotation.x += x_deg;
    rotation.y += y_deg;
    rotation.z += z_deg;

    Matrix4x4 rotationMatrix = Matrix4x4::createRotation(rotation.x, rotation.y, rotation.z);

    // Apply the rotation matrix to each vertex of each triangle
    for (auto & face : faces) {
        for (auto & triangle : face.triangles) {
            for (auto & vertex : triangle.vertices) {
                // Multiply the vertex by the rotation matrix
                Vector3 oldVec = vertex;
                vertex.x = oldVec.x * rotationMatrix.elements[0][0] + oldVec.y * rotationMatrix.elements[1][0] + oldVec.z * rotationMatrix.elements[2][0] + rotationMatrix.elements[3][0];
                vertex.y = oldVec.x * rotationMatrix.elements[0][1] + oldVec.y * rotationMatrix.elements[1][1] + oldVec.z * rotationMatrix.elements[2][1] + rotationMatrix.elements[3][1];
                vertex.z = oldVec.x * rotationMatrix.elements[0][2] + oldVec.y * rotationMatrix.elements[1][2] + oldVec.z * rotationMatrix.elements[2][2] + rotationMatrix.elements[3][2];
            }
        }
    }
}

    void translate(float x, float y, float z)
    {
        position.x += x;
        position.y += y;
        position.z += z;

        for (auto &face : faces) {
            for (auto &triangle : face.triangles) {
                for (auto &vertex : triangle.vertices) {
                    // Translate each vertex here
                    vertex.x += x;
                    vertex.y += y;
                    vertex.z += z;
                }
            }
        }
    }

    void draw(Vector3 finalColor) {  
    // Iterate through faces and their triangles
    // and call OpenGL draw routines

    for (auto &face : faces) {
        for (auto &triangle : face.triangles) {
          glBegin(GL_TRIANGLES);
          for (auto &vertex : triangle.vertices) {
             // set color for each vertex
             glColor3f(finalColor.x, finalColor.y, finalColor.z );//glColor3f(intensity,intensity,intensity); // red color
             glVertex3f(vertex.x, vertex.y, vertex.z);
          }
          glEnd();
        }
    }
}
};
