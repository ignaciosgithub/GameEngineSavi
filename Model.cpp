#include "Model.h"
#include "platform.h"
#include "Graphics/Core/GraphicsAPIFactory.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

// Constructor
Model::Model() : vao(0), vbo(0), ebo(0), tbo(0), nbo(0), size(1, 1, 1) {
    // Initialize model data
}

// Destructor
Model::~Model() {
    // Clean up graphics resources
    CleanupGL();
}

// Load model from file
bool Model::LoadFromFile(const std::string& filename) {
    std::cout << "Loading model from file: " << filename << std::endl;
    
    // Check file extension
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    
    if (extension == "obj") {
        loadOBJ(filename);
        return true;
    } else {
        std::cerr << "Unsupported file format: " << extension << std::endl;
        return false;
    }
}

// Load OBJ file
void Model::loadOBJ(std::string path) {
    std::vector<Vector3> temp_vertices;
    std::vector<Vector3> temp_normals;
    std::vector<Vector2> temp_uvs;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    
    // Material data
    std::string mtlFilename;
    std::map<std::string, Vector3> materials; // material name -> color
    std::string currentMaterial;
    
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;
            
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {
            // Vertex position
            Vector3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (prefix == "vt") {
            // Texture coordinate
            Vector2 uv;
            iss >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        } else if (prefix == "vn") {
            // Vertex normal
            Vector3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (prefix == "f") {
            // Face - support different formats (v, v/vt, v//vn, v/vt/vn)
            std::string vertex1, vertex2, vertex3, vertex4;
            iss >> vertex1 >> vertex2 >> vertex3 >> vertex4;
            
            // Parse vertex indices for the first three vertices (triangle)
            parseVertexIndices(vertex1, vertex2, vertex3, 
                             vertexIndices, uvIndices, normalIndices, 
                             temp_vertices.size(), temp_uvs.size(), temp_normals.size());
            
            // If we have a fourth vertex, create another triangle (triangulate quad)
            if (!vertex4.empty()) {
                parseVertexIndices(vertex1, vertex3, vertex4, 
                                 vertexIndices, uvIndices, normalIndices, 
                                 temp_vertices.size(), temp_uvs.size(), temp_normals.size());
            }
        } else if (prefix == "mtllib") {
            // Material library
            iss >> mtlFilename;
            loadMTL(mtlFilename, path, materials);
        } else if (prefix == "usemtl") {
            // Use material
            iss >> currentMaterial;
        }
    }
    
    file.close();
    
    // Process the data
    processModelData(temp_vertices, temp_normals, temp_uvs, 
                   vertexIndices, normalIndices, uvIndices, 
                   materials, currentMaterial);
    
    // Initialize graphics buffers
    InitializeBuffers();
}

// Helper method to parse vertex indices from face definitions
void Model::parseVertexIndices(const std::string& v1, const std::string& v2, const std::string& v3,
                             std::vector<unsigned int>& vertexIndices, 
                             std::vector<unsigned int>& uvIndices, 
                             std::vector<unsigned int>& normalIndices,
                             size_t vertexCount, size_t uvCount, size_t normalCount) {
    // Parse each vertex string (v, v/vt, v//vn, or v/vt/vn)
    parseVertex(v1, vertexIndices, uvIndices, normalIndices, vertexCount, uvCount, normalCount);
    parseVertex(v2, vertexIndices, uvIndices, normalIndices, vertexCount, uvCount, normalCount);
    parseVertex(v3, vertexIndices, uvIndices, normalIndices, vertexCount, uvCount, normalCount);
}

// Helper method to parse a single vertex string
void Model::parseVertex(const std::string& vertexStr, 
                      std::vector<unsigned int>& vertexIndices, 
                      std::vector<unsigned int>& uvIndices, 
                      std::vector<unsigned int>& normalIndices,
                      size_t vertexCount, size_t uvCount, size_t normalCount) {
    // Count slashes to determine format
    size_t slashCount = std::count(vertexStr.begin(), vertexStr.end(), '/');
    
    if (slashCount == 0) {
        // Format: v
        int vertexIndex = 0;
        sscanf(vertexStr.c_str(), "%d", &vertexIndex);
        
        // OBJ indices start at 1, convert to 0-based
        if (vertexIndex > 0) vertexIndex -= 1;
        else if (vertexIndex < 0) vertexIndex = vertexCount + vertexIndex; // Negative indices
        
        vertexIndices.push_back(vertexIndex);
    } else if (slashCount == 1) {
        // Format: v/vt
        int vertexIndex = 0, uvIndex = 0;
        sscanf(vertexStr.c_str(), "%d/%d", &vertexIndex, &uvIndex);
        
        // Convert to 0-based
        if (vertexIndex > 0) vertexIndex -= 1;
        else if (vertexIndex < 0) vertexIndex = vertexCount + vertexIndex;
        
        if (uvIndex > 0) uvIndex -= 1;
        else if (uvIndex < 0) uvIndex = uvCount + uvIndex;
        
        vertexIndices.push_back(vertexIndex);
        uvIndices.push_back(uvIndex);
    } else if (slashCount == 2) {
        // Check if format is v//vn or v/vt/vn
        if (vertexStr.find("//") != std::string::npos) {
            // Format: v//vn
            int vertexIndex = 0, normalIndex = 0;
            sscanf(vertexStr.c_str(), "%d//%d", &vertexIndex, &normalIndex);
            
            // Convert to 0-based
            if (vertexIndex > 0) vertexIndex -= 1;
            else if (vertexIndex < 0) vertexIndex = vertexCount + vertexIndex;
            
            if (normalIndex > 0) normalIndex -= 1;
            else if (normalIndex < 0) normalIndex = normalCount + normalIndex;
            
            vertexIndices.push_back(vertexIndex);
            normalIndices.push_back(normalIndex);
        } else {
            // Format: v/vt/vn
            int vertexIndex = 0, uvIndex = 0, normalIndex = 0;
            sscanf(vertexStr.c_str(), "%d/%d/%d", &vertexIndex, &uvIndex, &normalIndex);
            
            // Convert to 0-based
            if (vertexIndex > 0) vertexIndex -= 1;
            else if (vertexIndex < 0) vertexIndex = vertexCount + vertexIndex;
            
            if (uvIndex > 0) uvIndex -= 1;
            else if (uvIndex < 0) uvIndex = uvCount + uvIndex;
            
            if (normalIndex > 0) normalIndex -= 1;
            else if (normalIndex < 0) normalIndex = normalCount + normalIndex;
            
            vertexIndices.push_back(vertexIndex);
            uvIndices.push_back(uvIndex);
            normalIndices.push_back(normalIndex);
        }
    }
}

// Load MTL file
void Model::loadMTL(const std::string& mtlFilename, const std::string& objPath, 
                  std::map<std::string, Vector3>& materials) {
    // Get directory of OBJ file
    std::string directory = objPath.substr(0, objPath.find_last_of("/\\") + 1);
    std::string mtlPath = directory + mtlFilename;
    
    std::ifstream file(mtlPath);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open material file " << mtlPath << std::endl;
        return;
    }
    
    std::string currentMaterial;
    Vector3 diffuseColor(1.0f, 1.0f, 1.0f); // Default white
    
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;
            
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "newmtl") {
            // New material
            iss >> currentMaterial;
        } else if (prefix == "Kd") {
            // Diffuse color
            iss >> diffuseColor.x >> diffuseColor.y >> diffuseColor.z;
            materials[currentMaterial] = diffuseColor;
        } else if (prefix == "map_Kd") {
            // Diffuse texture map
            std::string texturePath;
            iss >> texturePath;
            
            // Get full path to texture
            std::string fullTexturePath = directory + texturePath;
            
            // Load texture
            loadTexture(fullTexturePath, "albedo");
        }
    }
    
    file.close();
}

// Process model data
void Model::processModelData(const std::vector<Vector3>& temp_vertices,
                           const std::vector<Vector3>& temp_normals,
                           const std::vector<Vector2>& temp_uvs,
                           const std::vector<unsigned int>& vertexIndices,
                           const std::vector<unsigned int>& normalIndices,
                           const std::vector<unsigned int>& uvIndices,
                           const std::map<std::string, Vector3>& materials,
                           const std::string& currentMaterial) {
    // Clear existing data
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
    
    // Process the data
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        if (i < vertexIndices.size()) {
            unsigned int vertexIndex = vertexIndices[i];
            if (vertexIndex < temp_vertices.size()) {
                Vector3 vertex = temp_vertices[vertexIndex];
                vertices.push_back(vertex.x);
                vertices.push_back(vertex.y);
                vertices.push_back(vertex.z);
            } else {
                std::cerr << "Warning: Vertex index out of range: " << vertexIndex << std::endl;
            }
        }
        
        if (!temp_normals.empty() && i < normalIndices.size()) {
            unsigned int normalIndex = normalIndices[i];
            if (normalIndex < temp_normals.size()) {
                Vector3 normal = temp_normals[normalIndex];
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            } else {
                std::cerr << "Warning: Normal index out of range: " << normalIndex << std::endl;
            }
        }
        
        if (!temp_uvs.empty() && i < uvIndices.size()) {
            unsigned int uvIndex = uvIndices[i];
            if (uvIndex < temp_uvs.size()) {
                Vector2 uv = temp_uvs[uvIndex];
                texCoords.push_back(uv.x);
                texCoords.push_back(uv.y);
            } else {
                std::cerr << "Warning: UV index out of range: " << uvIndex << std::endl;
            }
        }
    }
    
    // Set material color if available
    if (!currentMaterial.empty() && materials.find(currentMaterial) != materials.end()) {
        Vector3 color = materials.at(currentMaterial);
        // Store material color for later use in shader
        // This would typically be set as a uniform in the shader
    }
}

// Load texture
void Model::loadTexture(const std::string& path, const std::string& type) {
    // Simple implementation for testing
    std::cout << "Loading texture: " << path << " of type: " << type << std::endl;
    // In a real implementation, this would load the texture using stb_image or similar
}

// Initialize buffers
void Model::InitializeBuffers() {
    InitializeGL();
}

// Initialize graphics objects
void Model::InitializeGL() {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "Failed to get graphics API instance" << std::endl;
        return;
    }
    
    // Generate VAO
    vao = graphics->CreateVertexArray();
    graphics->BindVertexArray(vao);
    
    // Generate VBO for vertices
    vbo = graphics->CreateBuffer();
    graphics->BindBuffer(BufferType::VERTEX_BUFFER, vbo);
    graphics->BufferData(BufferType::VERTEX_BUFFER, vertices.data(), vertices.size() * sizeof(float));
    
    // Set vertex attributes
    graphics->EnableVertexAttrib(0);
    graphics->VertexAttribPointer(0, 3, false, 0, nullptr);
    
    // If we have normals, create a buffer for them
    if (!normals.empty()) {
        nbo = graphics->CreateBuffer();
        graphics->BindBuffer(BufferType::VERTEX_BUFFER, nbo);
        graphics->BufferData(BufferType::VERTEX_BUFFER, normals.data(), normals.size() * sizeof(float));
        
        graphics->EnableVertexAttrib(1);
        graphics->VertexAttribPointer(1, 3, false, 0, nullptr);
    }
    
    // If we have texture coordinates, create a buffer for them
    if (!texCoords.empty()) {
        tbo = graphics->CreateBuffer();
        graphics->BindBuffer(BufferType::TEXTURE_COORD_BUFFER, tbo);
        graphics->BufferData(BufferType::TEXTURE_COORD_BUFFER, texCoords.data(), texCoords.size() * sizeof(float));
        
        graphics->EnableVertexAttrib(2);
        graphics->VertexAttribPointer(2, 2, false, 0, nullptr);
    }
    
    // If we have indices, create an element buffer
    if (!indices.empty()) {
        ebo = graphics->CreateBuffer();
        graphics->BindBuffer(BufferType::INDEX_BUFFER, ebo);
        graphics->BufferData(BufferType::INDEX_BUFFER, indices.data(), indices.size() * sizeof(unsigned int));
    }
    
    // Unbind VAO
    graphics->BindVertexArray(0);
}

// Update graphics buffers
void Model::UpdateBuffers() {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "Failed to get graphics API instance" << std::endl;
        return;
    }
    
    // Update vertex buffer
    graphics->BindBuffer(BufferType::VERTEX_BUFFER, vbo);
    graphics->BufferData(BufferType::VERTEX_BUFFER, vertices.data(), vertices.size() * sizeof(float));
    
    // Update normal buffer if we have normals
    if (!normals.empty() && nbo != 0) {
        graphics->BindBuffer(BufferType::VERTEX_BUFFER, nbo);
        graphics->BufferData(BufferType::VERTEX_BUFFER, normals.data(), normals.size() * sizeof(float));
    }
    
    // Update texture coordinate buffer if we have texture coordinates
    if (!texCoords.empty() && tbo != 0) {
        graphics->BindBuffer(BufferType::TEXTURE_COORD_BUFFER, tbo);
        graphics->BufferData(BufferType::TEXTURE_COORD_BUFFER, texCoords.data(), texCoords.size() * sizeof(float));
    }
    
    // Update index buffer if we have indices
    if (!indices.empty() && ebo != 0) {
        graphics->BindBuffer(BufferType::INDEX_BUFFER, ebo);
        graphics->BufferData(BufferType::INDEX_BUFFER, indices.data(), indices.size() * sizeof(unsigned int));
    }
}

// Clean up graphics objects
void Model::CleanupGL() {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return;
    }
    
    if (vao != 0) {
        graphics->DeleteVertexArray(vao);
        vao = 0;
    }
    
    if (vbo != 0) {
        graphics->DeleteBuffer(vbo);
        vbo = 0;
    }
    
    if (ebo != 0) {
        graphics->DeleteBuffer(ebo);
        ebo = 0;
    }
    
    if (tbo != 0) {
        graphics->DeleteBuffer(tbo);
        tbo = 0;
    }
    
    if (nbo != 0) {
        graphics->DeleteBuffer(nbo);
        nbo = 0;
    }
}

// Render the model with both point lights and directional lights
void Model::Render(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& directionalLights) {
    std::cout << "Model::Render - Starting render with " << pointLights.size() << " point lights and " 
              << directionalLights.size() << " directional lights" << std::endl;
    
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics || vao == 0) {
        std::cout << "Model::Render - ERROR: Graphics API or VAO is null (graphics=" 
                  << (graphics ? "valid" : "null") << ", vao=" << vao << ")" << std::endl;
        return;
    }
    
    // Use shader program if available
    if (shaderProgram) {
        std::cout << "Model::Render - Using shader program with handle: " << shaderProgram->GetHandle() << std::endl;
        graphics->UseShaderProgram(shaderProgram);
    } else {
        std::cout << "Model::Render - ERROR: No shader program set for model" << std::endl;
        return; // Return early if no shader program is set to prevent segmentation fault
    }
    
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
    viewMatrix.identity();
    projectionMatrix.identity();
    
    // Call the version with explicit matrices
    Render(pointLights, directionalLights, viewMatrix, projectionMatrix);
    std::cerr << "Warning: Using default view/projection matrices for model rendering" << std::endl;
}

// Render the model with both point lights and directional lights and specific view/projection matrices
void Model::Render(const std::vector<PointLight>& pointLights, const std::vector<DirectionalLight>& directionalLights, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) {
    std::cout << "Model::Render - Starting render with " << pointLights.size() << " point lights and " 
              << directionalLights.size() << " directional lights and custom matrices" << std::endl;
    
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics || vao == 0) {
        std::cout << "Model::Render - ERROR: Graphics API or VAO is null (graphics=" 
                  << (graphics ? "valid" : "null") << ", vao=" << vao << ")" << std::endl;
        return;
    }
    
    // Use shader program if available
    if (shaderProgram) {
        std::cout << "Model::Render - Using shader program with handle: " << shaderProgram->GetHandle() << std::endl;
        graphics->UseShaderProgram(shaderProgram);
    } else {
        std::cout << "Model::Render - ERROR: No shader program set for model" << std::endl;
        return; // Return early if no shader program is set to prevent segmentation fault
    }
    
    Matrix4x4 modelMatrix;
    modelMatrix.identity(); // Start with identity matrix
    modelMatrix.translate(position.x, position.y, position.z);
    
    Matrix4x4 rotationMatrix = Matrix4x4::createRotation(rotation.x, rotation.y, rotation.z);
    modelMatrix = modelMatrix * rotationMatrix;
    
    shaderProgram->SetUniform("model", modelMatrix);
    shaderProgram->SetUniform("view", viewMatrix);
    shaderProgram->SetUniform("projection", projectionMatrix);
    
    // Set point light uniforms
    int pointLightCount = std::min((int)pointLights.size(), 8); // Limit to 8 point lights
    shaderProgram->SetUniform("numPointLights", pointLightCount);
    
    for (int i = 0; i < pointLightCount; i++) {
        std::string prefix = "pointLights[" + std::to_string(i) + "].";
        shaderProgram->SetUniform(prefix + "position", pointLights[i].GetPosition());
        shaderProgram->SetUniform(prefix + "color", pointLights[i].GetColor());
        shaderProgram->SetUniform(prefix + "intensity", pointLights[i].GetIntensity());
        shaderProgram->SetUniform(prefix + "range", pointLights[i].GetRange());
    }
    
    // Set directional light uniforms
    int dirLightCount = std::min((int)directionalLights.size(), 4); // Limit to 4 directional lights
    shaderProgram->SetUniform("numDirectionalLights", dirLightCount);
    
    for (int i = 0; i < dirLightCount; i++) {
        std::string prefix = "directionalLights[" + std::to_string(i) + "].";
        shaderProgram->SetUniform(prefix + "direction", directionalLights[i].GetDirection());
        shaderProgram->SetUniform(prefix + "color", directionalLights[i].GetColor());
        shaderProgram->SetUniform(prefix + "intensity", directionalLights[i].GetIntensity());
    }
    
    graphics->BindVertexArray(vao);
    
    if (!indices.empty()) {
        graphics->DrawElements(DrawMode::TRIANGLES, indices.size(), nullptr);
    } else {
        graphics->DrawArrays(DrawMode::TRIANGLES, 0, vertices.size() / 3);
    }
    
    graphics->BindVertexArray(0);
}

// Render the model with only point lights
void Model::Render(const std::vector<PointLight>& lights) {
    // Call the new method with an empty directional lights vector
    std::vector<DirectionalLight> emptyDirectionalLights;
    Render(lights, emptyDirectionalLights);
}

// Set shader program
void Model::SetShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
}
