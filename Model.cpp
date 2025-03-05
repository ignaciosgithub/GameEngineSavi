#include "Model.h"
#include "platform.h"

// Include OpenGL headers in the correct order
#include "ThirdParty/OpenGL/include/GL/platform_gl.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Constructor
Model::Model() : vao(0), vbo(0), ebo(0), vertexCount(0), indexCount(0) {
    // Initialize model data
}

// Destructor
Model::~Model() {
    // Clean up OpenGL resources
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    
    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
}

// Load model from file
bool Model::LoadFromFile(const std::string& filename) {
    std::cout << "Loading model from file: " << filename << std::endl;
    
    // Check file extension
    std::string extension = filename.substr(filename.find_last_of(".") + 1);
    
    if (extension == "obj") {
        return LoadOBJ(filename);
    } else {
        std::cerr << "Unsupported file format: " << extension << std::endl;
        return false;
    }
}

// Load OBJ file
bool Model::LoadOBJ(const std::string& filename) {
    std::vector<Vector3> temp_vertices;
    std::vector<Vector3> temp_normals;
    std::vector<Vector2> temp_uvs;
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
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
        vertices.push_back(vertex);
        
        if (!temp_normals.empty() && i < normalIndices.size()) {
            unsigned int normalIndex = normalIndices[i];
            Vector3 normal = temp_normals[normalIndex];
            normals.push_back(normal);
        }
        
        if (!temp_uvs.empty() && i < uvIndices.size()) {
            unsigned int uvIndex = uvIndices[i];
            Vector2 uv = temp_uvs[uvIndex];
            uvs.push_back(uv);
        }
    }
    
    // Initialize OpenGL buffers
    InitBuffers();
    
    return true;
}

// Initialize OpenGL buffers
void Model::InitBuffers() {
    // Generate VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Generate VBO for vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);
    
    // Set vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    
    // If we have normals, create a buffer for them
    if (!normals.empty()) {
        GLuint normalVBO;
        glGenBuffers(1, &normalVBO);
        glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), &normals[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    }
    
    // If we have UVs, create a buffer for them
    if (!uvs.empty()) {
        GLuint uvVBO;
        glGenBuffers(1, &uvVBO);
        glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), &uvs[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);
    }
    
    // Unbind VAO
    glBindVertexArray(0);
    
    // Set vertex count
    vertexCount = vertices.size();
}

// Render the model
void Model::Render() {
    if (vao != 0) {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glBindVertexArray(0);
    }
}
