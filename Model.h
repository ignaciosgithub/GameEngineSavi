#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "Vector3.h"
#include "Shaders/Core/ShaderProgram.h"
#include "GL/gl_types.h"
#include "MonoBehaviourLike.h"

class PointLight;

class Model : public MonoBehaviourLike {
private:
    // Mesh data
    std::vector<GLfloat> vertices; // Stores the vertices
    std::vector<GLuint> indices; // Stores the indices representing triangles
    std::vector<GLfloat> texCoords; // Stores texture coordinates
    std::vector<GLfloat> normals;   // Stores vertex normals
    
    // Texture data
    std::string texturePath;
    
    // Shader program
    ShaderProgram* shaderProgram;
    
    // Calculate normal for a vertex
    Vector3 calculateNormal(size_t i) const {
        if (i + 2 < vertices.size()) {
            Vector3 v1(vertices[i], vertices[i+1], vertices[i+2]);
            Vector3 v2(vertices[i+3], vertices[i+4], vertices[i+5]);
            Vector3 v3(vertices[i+6], vertices[i+7], vertices[i+8]);
            
            Vector3 edge1 = v2 - v1;
            Vector3 edge2 = v3 - v1;
            
            return edge1.cross(edge2).normalized();
        }
        
        return Vector3(0, 1, 0);
    }
    
public:
    // Constructor
    Model();
    
    // Destructor
    ~Model();
    
    // Load model from file
    bool LoadFromFile(const std::string& filename);
    
    // Create a simple cube
    void CreateCube();
    
    // Create a simple plane
    void CreatePlane();
    
    // Create a simple sphere
    void CreateSphere(float radius, int segments);
    
    // Update the model
    virtual void Update(float deltaTime) override {}
    
    // Render the model
    void Render(const std::vector<PointLight>& lights);
    
    // Update vertices
    void UpdateVertices(const std::vector<GLfloat>& newVertices);
    
    // Set shader program
    void SetShaderProgram(ShaderProgram* program);
    
    // Get shader program
    ShaderProgram* GetShaderProgram() const;
    
    // Set texture path
    void SetTexturePath(const std::string& path);
    
    // Get texture path
    const std::string& GetTexturePath() const;
    
private:
    // OpenGL objects
    GLuint vao = 0;  // Vertex Array Object
    GLuint vbo = 0;  // Vertex Buffer Object
    GLuint ebo = 0;  // Element Buffer Object
    GLuint tbo = 0;  // Texture Coordinate Buffer Object
    GLuint nbo = 0;  // Normal Buffer Object
    
    // Initialize OpenGL objects
    void InitializeGL();
    
    // Update OpenGL buffers
    void UpdateBuffers();
    
    // Clean up OpenGL objects
    void CleanupGL();
};

#endif // MODEL_H
