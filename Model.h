#ifndef MODEL_H
#define MODEL_H

#include "platform.h"
#include "Vector3.h"
#include "PointLight.h"
#include "Shaders/Core/ShaderProgram.h"
#include "MonoBehaviourLike.h"
#include <vector>
#include <string>
#include <functional>

// Forward declaration for Texture
class Texture;

class Model : public MonoBehaviourLike {
public:
    std::vector<GLfloat> vertices; // Stores the vertices
    std::vector<GLuint> indices; // Stores the indices representing triangles
    std::vector<GLfloat> texCoords; // Stores texture coordinates
    std::vector<GLfloat> normals;   // Stores vertex normals
    bool high_poly;
    int frameCounter;
    Vector3 rotation, position;
    Vector3 pcolorsum;
    std::vector<Vector3> colourmap;
    
    Texture* albedoTexture;
    Texture* normalTexture;
    Texture* opacityTexture;
    
    // Callback for vertex updates
    std::function<void(const Model*)> onVerticesUpdated;
    
    // Fixed initialization order to match declaration order
    Model() : high_poly(false), frameCounter(0), 
              albedoTexture(nullptr), normalTexture(nullptr), opacityTexture(nullptr),
              shaderProgram(nullptr) {}

    // Constructor with default arguments
    Model(const std::string& filePath, const Vector3& _rotation = Vector3(), const Vector3& _position = Vector3())
        : high_poly(false), frameCounter(0), rotation(_rotation), position(_position),
          albedoTexture(nullptr), normalTexture(nullptr), opacityTexture(nullptr),
          shaderProgram(nullptr) {
            loadOBJ(filePath);
    }

    void loadOBJ(std::string path);
    
    // Load model from file
    bool LoadFromFile(const std::string& path) {
        loadOBJ(path);
        return true; // For now, assume loading always succeeds
    }

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

    // Translation method - declaration only, implementation in Model.cpp
    void translate(const Vector3& vec);
    
    // Animation and vertex update methods
    void UpdateVertices(const std::vector<GLfloat>& newVertices);
    
    // Shader methods
    void SetShaderProgram(Shaders::ShaderProgram* program);
    Shaders::ShaderProgram* GetShaderProgram() const { return shaderProgram; }
    void UpdateUniforms(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);
    
    // Rendering methods
    void Render(const std::vector<PointLight>& pointLights);
    void draw(const std::vector<PointLight>& pointLights);
    void rotate();
    void drawcomp(const std::vector<PointLight>& pointLights);
    
    // MonoBehaviourLike overrides
    virtual void Update(float deltaTime) override {}
    
private:
    Shaders::ShaderProgram* shaderProgram;  // Shader program for rendering
    
    // OpenGL buffer objects
    GLuint vao = 0;  // Vertex Array Object
    GLuint vbo = 0;  // Vertex Buffer Object
    GLuint ebo = 0;  // Element Buffer Object
    GLuint tbo = 0;  // Texture Coordinate Buffer Object
    GLuint nbo = 0;  // Normal Buffer Object
    
    // Initialize OpenGL buffers
    void InitializeBuffers();
    void UpdateBuffers();
};

#endif // MODEL_H
