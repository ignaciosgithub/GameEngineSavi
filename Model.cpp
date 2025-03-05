#include "Model.h"
#include "platform.h"
#include "Graphics/Core/GraphicsAPIFactory.h"
#include "PointLight.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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
    
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
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
            // Face
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;
            
            // Parse vertex indices
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            
            // Parse first vertex
            sscanf(vertex1.c_str(), "%d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0]);
            sscanf(vertex2.c_str(), "%d/%d/%d", &vertexIndex[1], &uvIndex[1], &normalIndex[1]);
            sscanf(vertex3.c_str(), "%d/%d/%d", &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
            
            // OBJ file indices start at 1, so subtract 1
            vertexIndices.push_back(vertexIndex[0] - 1);
            vertexIndices.push_back(vertexIndex[1] - 1);
            vertexIndices.push_back(vertexIndex[2] - 1);
            
            if (!temp_uvs.empty()) {
                uvIndices.push_back(uvIndex[0] - 1);
                uvIndices.push_back(uvIndex[1] - 1);
                uvIndices.push_back(uvIndex[2] - 1);
            }
            
            if (!temp_normals.empty()) {
                normalIndices.push_back(normalIndex[0] - 1);
                normalIndices.push_back(normalIndex[1] - 1);
                normalIndices.push_back(normalIndex[2] - 1);
            }
        }
    }
    
    file.close();
    
    // Process the data
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        
        Vector3 vertex = temp_vertices[vertexIndex];
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
        
        if (!temp_normals.empty() && i < normalIndices.size()) {
            unsigned int normalIndex = normalIndices[i];
            Vector3 normal = temp_normals[normalIndex];
            normals.push_back(normal.x);
            normals.push_back(normal.y);
            normals.push_back(normal.z);
        }
        
        if (!temp_uvs.empty() && i < uvIndices.size()) {
            unsigned int uvIndex = uvIndices[i];
            Vector2 uv = temp_uvs[uvIndex];
            texCoords.push_back(uv.x);
            texCoords.push_back(uv.y);
        }
    }
    
    // Initialize graphics buffers
    InitializeBuffers();
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

// Render the model
void Model::Render(const std::vector<PointLight>& lights) {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics || vao == 0) {
        return;
    }
    
    // Use shader program if available
    if (shaderProgram) {
        graphics->UseShaderProgram(shaderProgram);
    } else {
        std::cerr << "Warning: No shader program set for model" << std::endl;
    }
    
    graphics->BindVertexArray(vao);
    
    if (!indices.empty()) {
        graphics->DrawElements(DrawMode::TRIANGLES, indices.size(), nullptr);
    } else {
        graphics->DrawArrays(DrawMode::TRIANGLES, 0, vertices.size() / 3);
    }
    
    graphics->BindVertexArray(0);
}

// Set shader program
void Model::SetShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
}
