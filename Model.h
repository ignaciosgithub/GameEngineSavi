#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <functional>
#include <cmath>
#include "Vector3.h"
#include "Vector2.h"
#include "Matrix4x4.h"
#include "Triangle.h"
// Face.h is already included elsewhere, so we'll just forward declare it
class Face;
#include "Shaders/Core/ShaderProgram.h"
#include "MonoBehaviourLike.h"
#include "Texture.h"
#include "platform.h"
#include "Graphics/Core/IGraphicsAPI.h"
#include <map>

// Forward declarations
class PointLight;
class DirectionalLight;

class Model : public MonoBehaviourLike {
public:
    // Mesh data
    std::vector<float> vertices; // Stores the vertices
    std::vector<unsigned int> indices; // Stores the indices representing triangles
    std::vector<float> texCoords; // Stores texture coordinates
    std::vector<float> normals;   // Stores vertex normals
    
    // Position and rotation
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    
    // Constructor
    Model();
    
    // Destructor
    ~Model();
    
    // Load model from file
    bool LoadFromFile(const std::string& filename);
    
    // Load model from OBJ file
    void loadOBJ(std::string path);
    
    // Load texture
    void loadTexture(const std::string& path, const std::string& type);
    
    // Set texture tiling
    void setTiling(float x, float y);
    
    // Translate model
    void translate(const Vector3& vec);
    
    // Rotate model
    void rotate();
    
    // Draw model (compatibility method)
    void draw(const std::vector<PointLight>& pointLights);
    
    // Draw model (compatibility method)
    void drawcomp(const std::vector<PointLight>& pointLights);
    
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
    
    // Render the model with directional lights
    void Render(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& directionalLights);
    
    // Render the model with directional lights and specific view/projection matrices
    void Render(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& directionalLights, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);
    
    // Update uniforms
    void UpdateUniforms(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix);
    
    // Update vertices
    void UpdateVertices(const std::vector<float>& newVertices);
    
    // Initialize buffers
    void InitializeBuffers();
    
    // Set shader program
    void SetShaderProgram(ShaderProgram* program);
    
    // Get shader program
    ShaderProgram* GetShaderProgram() const { return shaderProgram; }
    
    // Set texture path
    void SetTexturePath(const std::string& path) { texturePath = path; }
    
    // Get texture path
    const std::string& GetTexturePath() const { return texturePath; }
    
private:
    // Texture data
    std::string texturePath;
    
    // Shader program
    ShaderProgram* shaderProgram = nullptr;
    
    // Textures
    Texture* albedoTexture = nullptr;
    Texture* normalTexture = nullptr;
    Texture* opacityTexture = nullptr;
    
    // Callback for when vertices are updated
    std::function<void(Model*)> onVerticesUpdated;
    
    // Graphics API objects
    unsigned int vao = 0;  // Vertex Array Object
    unsigned int vbo = 0;  // Vertex Buffer Object
    unsigned int ebo = 0;  // Element Buffer Object
    unsigned int tbo = 0;  // Texture Coordinate Buffer Object
    unsigned int nbo = 0;  // Normal Buffer Object
    
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
    
    // Initialize graphics objects
    void InitializeGL();
    
    // Update graphics buffers
    void UpdateBuffers();
    
    // Clean up graphics objects
    void CleanupGL();
    
    // Helper methods for OBJ loading
    void parseVertexIndices(const std::string& v1, const std::string& v2, const std::string& v3,
                           std::vector<unsigned int>& vertexIndices, 
                           std::vector<unsigned int>& uvIndices, 
                           std::vector<unsigned int>& normalIndices,
                           size_t vertexCount, size_t uvCount, size_t normalCount);
    void parseVertex(const std::string& vertexStr, 
                    std::vector<unsigned int>& vertexIndices, 
                    std::vector<unsigned int>& uvIndices, 
                    std::vector<unsigned int>& normalIndices,
                    size_t vertexCount, size_t uvCount, size_t normalCount);
    void loadMTL(const std::string& mtlFilename, const std::string& objPath, 
                std::map<std::string, Vector3>& materials);
    void processModelData(const std::vector<Vector3>& temp_vertices,
                         const std::vector<Vector3>& temp_normals,
                         const std::vector<Vector2>& temp_uvs,
                         const std::vector<unsigned int>& vertexIndices,
                         const std::vector<unsigned int>& normalIndices,
                         const std::vector<unsigned int>& uvIndices,
                         const std::map<std::string, Vector3>& materials,
                         const std::string& currentMaterial);
};

#endif // MODEL_H
