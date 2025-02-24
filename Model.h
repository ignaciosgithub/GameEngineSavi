#include <windows.h>
#include <gl/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <gl/gl.h>
#include<math.h>
#include <vector>
#include<string>
#include "Texture.h"

class Model {
public:
    std::vector<GLfloat> vertices; // Stores the vertices
    std::vector<GLuint> indices; // Stores the indices representing triangles
    std::vector<GLfloat> texCoords; // Stores texture coordinates
    bool high_poly;
    Vector3 rotation, position;
    int frameCounter;
    Vector3 pcolorsum;
    //std::vector<Vector3> colourmap;
    std::vector<Vector3> colourmap;
    
    Texture* albedoTexture;
    Texture* normalTexture;
    Texture* opacityTexture;
    
    Model() : albedoTexture(nullptr), normalTexture(nullptr), opacityTexture(nullptr) {}

    Model(std::string filePath, Vector3 _rotation, Vector3 _position)
        : rotation(_rotation), position(_position), albedoTexture(nullptr), 
          normalTexture(nullptr), opacityTexture(nullptr) {
            loadOBJ(filePath);
    }

    void loadOBJ(std::string path);

    // Texture methods
    void loadTexture(const std::string& path, const std::string& type);
    void setTiling(float x, float y);

    // methods for translation, rotation and drawing ...
    void translate(const Vector3& vec);
    void Render(const std::vector<PointLight>& pointLights);
    void draw(const std::vector<PointLight>& pointLights);
    void rotate();
    void drawcomp(const std::vector<PointLight>& pointLights);
};
