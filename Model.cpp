#include <gl/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <gl/gl.h>

#include<vector>
#include"Vector3.h"
#include"Triangle.h"
#include"Matrix4x4.h"
#include"Raycast.h"
#include"PointLight.h"
#include"Camera.h"
#include"Face.h"
#include"Pyramid.h"


#include <fstream>
#include<string>
#include <sstream>
#include<math.h>
inline float toRadians(float degrees) {
    return degrees * static_cast<float>(3.14f) / 180.0f;
}
class Model {
public:
    std::vector<GLfloat> vertices; // Stores the vertices
    std::vector<GLuint> indices; // Stores the indices representing triangles
    bool high_poly;
    Vector3 rotation, position;
    int frameCounter =0;
    Vector3 pcolorsum;
    //std::vector<Vector3> colourmap;
std::vector<Vector3> colourmap;
    Model(){}

    Model(std::string filePath, Vector3 _rotation, Vector3 _position)
        : rotation(_rotation), position (_position) {
            loadOBJ(filePath);
    }

    void loadOBJ(std::string path) {
        std::ifstream file(path);
        std::string line;
       
        std::vector<Vector3> tempVertices;
       
        while (std::getline(file, line)) {
           
            if (line.substr(0, 2) == "v ") {
                std::istringstream s(line.substr(2));
                Vector3 v;
                s >> v.x; s >> v.y; s >> v.z;
                tempVertices.push_back(v);
            }
            else if (line.substr(0, 2) == "f ") {
                std::istringstream s(line.substr(2));
                GLuint a, b, c; // to store mesh index
                s >> a; s >> b; s >> c;

                a--; b--; c--;  // in wavefront obj all indices start at 1, not zero

                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(c);
            }
        }

        for(GLuint i=0;i<indices.size();i+=3){
            Vector3 v1 = tempVertices[indices[i]];
            Vector3 v2 = tempVertices[indices[i+1]];
            Vector3 v3 = tempVertices[indices[i+2]];

            vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z);
            vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
            vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
                          colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
             colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        }
       
}

    // methods for translation, rotation and drawing ...
      void translate(const Vector3& vec) {
        for(size_t i=0; i<vertices.size(); i+=3) {
            vertices[i]   += vec.x;
            vertices[i+1] += vec.y;
            vertices[i+2] += vec.z;
        }
    }
void Render(const std::vector<PointLight>& pointLights) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
         glShadeModel(GL_FLAT);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glBegin(GL_TRIANGLES);
        for(size_t i = 0; i < vertices.size(); i+=3) {
Vector3 colorSum(0, 0, 0);
            if (i % 5 == frameCounter % 5) {
                Vector3 vertex = Vector3(vertices[i], vertices[i+1], vertices[i+2]);
                Vector3 colorSum(0, 0, 0);
                for (const auto& light : pointLights) {
                    float distanceToLight = light.position.sqdist(vertex);
                    float intensity = light.SQIntensity(distanceToLight);

                    Vector3 colorContribution;
                    colorContribution.x = light.color.x * intensity;
                    colorContribution.z = light.color.z * intensity;
                    colorContribution.y = light.color.y * intensity;

                    colorSum.x += colorContribution.x;
                    colorSum.y += colorContribution.y;
                    colorSum.z += colorContribution.z;
                    pcolorsum = colorSum;
                   
                colorSum = colorSum.clamp(0.0f, 1.0f);
                glColor3f(colorSum.x, colorSum.y, colorSum.z);
                glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
                }
               
}else{
colorSum = pcolorsum;

                colorSum = colorSum.clamp(0.0f, 1.0f);
                glColor3f(colorSum.x, colorSum.y, colorSum.z);
                glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
}

            }
       
        glEnd();
        glDisableClientState(GL_VERTEX_ARRAY);
        ++frameCounter;
    }


void draw(const std::vector<PointLight>& pointLights) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

    glBegin(GL_TRIANGLES);
    for(size_t i = 0; i < vertices.size(); i+=3) {
        Vector3 vertex = Vector3(vertices[i], vertices[i+1], vertices[i+2]);

        Vector3 colorSum(0, 0, 0);
        for (const auto& light : pointLights) {
            // Calculate distance to the light source
            float distanceToLight;
distanceToLight = light.position.distance(vertex);

            // Calculate color contribution from this light
            float intensity = light.Intensity(distanceToLight);
            Vector3 colorContribution;
colorContribution.x = light.color.x * intensity;
colorContribution.z = light.color.z * intensity;
colorContribution.y = light.color.y * intensity;
            // Sum the color contributions
            colorSum.x += colorContribution.x;
            colorSum.y += colorContribution.y;
            colorSum.z += colorContribution.z;
           
        }

        // Ensure that the maximum color component value is 1.0
        colorSum = colorSum.clamp(0.0f, 1.0f);
       
        glColor3f(colorSum.x, colorSum.y, colorSum.z);
        glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
    }
        glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);}
    bool firstRun = true;
 void rotate() {
    float radX = toRadians(rotation.x);
    float radY = toRadians(rotation.y);
    float radZ = toRadians(rotation.z);

    float cosX = cos(radX);
    float sinX = sin(radX);
    float cosY = cos(radY);
    float sinY = sin(radY);
    float cosZ = cos(radZ);
    float sinZ = sin(radZ);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        // Retrieve original vertex positions
        float x = vertices[i], y = vertices[i + 1], z = vertices[i + 2];
        
        // Rotate around X axis
        float x1 = x;
        float y1 = y * cosX - z * sinX;
        float z1 = y * sinX + z * cosX;
        
        // Rotate around Y axis
        float x2 = x1 * cosY + z1 * sinY;
        float y2 = y1;
        float z2 = z1 * cosY - x1 * sinY;
        
        // Rotate around Z axis
        float xFinal = x2 * cosZ - y2 * sinZ;
        float yFinal = x2 * sinZ + y2 * cosZ;
        float zFinal = z2;

        // Apply rotational transformation
        vertices[i] = xFinal;
        vertices[i + 1] = yFinal;
        vertices[i + 2] = zFinal;
    }
}

    void drawcomp(const std::vector<PointLight>& pointLights) {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
         glShadeModel(GL_FLAT);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glBegin(GL_TRIANGLES);
        for(size_t i = 0; i < vertices.size(); i+=3) {
Vector3 colorSum(0, 0, 0);
            if (i % 5 == frameCounter % 5) {
                Vector3 vertex = Vector3(vertices[i], vertices[i+1], vertices[i+2]);
                Vector3 colorSum(0, 0, 0);
                for (const auto& light : pointLights) {
                    float distanceToLight = light.position.sqdist(vertex);
                    float intensity = light.SQIntensity(distanceToLight);

                    Vector3 colorContribution;
                    colorContribution.x = light.color.x * intensity;
                    colorContribution.z = light.color.z * intensity;
                    colorContribution.y = light.color.y * intensity;

                    colorSum.x += colorContribution.x;
                    colorSum.y += colorContribution.y;
                    colorSum.z += colorContribution.z;
                    pcolorsum = colorSum;
                   
                colorSum = colorSum.clamp(0.0f, 1.0f);
                glColor3f(colorSum.x, colorSum.y, colorSum.z);
                glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
                }
               
}else{
colorSum = pcolorsum;

                colorSum = colorSum.clamp(0.0f, 1.0f);
                glColor3f(colorSum.x, colorSum.y, colorSum.z);
                glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
}

            }
       
        glEnd();
        glDisableClientState(GL_VERTEX_ARRAY);
        ++frameCounter;
    }
};
//};