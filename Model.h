#ifndef MODEL_H
#define MODEL_H

#include "platform.h"
#include "Vector3.h"
#include "PointLight.h"
#include <vector>
#include <string>

// Forward declaration for Texture
class Texture;

class Model {
public:
    std::vector<GLfloat> vertices; // Stores the vertices
    std::vector<GLuint> indices; // Stores the indices representing triangles
    std::vector<GLfloat> texCoords; // Stores texture coordinates
    bool high_poly;
    Vector3 rotation, position;
    int frameCounter;
    Vector3 pcolorsum;
    std::vector<Vector3> colourmap;
    
    Texture* albedoTexture;
    Texture* normalTexture;
    Texture* opacityTexture;
    
    Model() : albedoTexture(nullptr), normalTexture(nullptr), opacityTexture(nullptr), frameCounter(0), high_poly(false) {}

    Model(std::string filePath, Vector3 _rotation, Vector3 _position)
        : rotation(_rotation), position(_position), albedoTexture(nullptr), 
          normalTexture(nullptr), opacityTexture(nullptr), frameCounter(0), high_poly(false) {
            loadOBJ(filePath);
    }

    void loadOBJ(std::string path);

    // Texture methods
    void loadTexture(const std::string& path, const std::string& type);
    void setTiling(float x, float y);

    // Calculate normal for a vertex at index i
    Vector3 calculateNormal(size_t i) const {
        // Simple normal calculation for a triangle
        if (i + 2 < vertices.size()) {
            Vector3 v1(vertices[i], vertices[i+1], vertices[i+2]);
            Vector3 v2(vertices[i+3], vertices[i+4], vertices[i+5]);
            Vector3 v3(vertices[i+6], vertices[i+7], vertices[i+8]);
            
            Vector3 edge1 = v2 - v1;
            Vector3 edge2 = v3 - v1;
            
            // Cross product to get normal
            Vector3 normal = edge1.cross(edge2);
            normal.normalize();
            return normal;
        }
        return Vector3(0, 1, 0); // Default normal
    }

    // methods for translation, rotation and drawing ...
    void translate(const Vector3& vec) {
        position += vec;
    }
    
    void Render(const std::vector<PointLight>& pointLights);
    void draw(const std::vector<PointLight>& pointLights);
    void rotate();
    void drawcomp(const std::vector<PointLight>& pointLights);
};

#endif // MODEL_H
