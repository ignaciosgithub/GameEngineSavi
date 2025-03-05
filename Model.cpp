#include "Model.h"
#include "PointLight.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Platform-specific OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#else
#include <GL/gl.h>
#endif

// Include OpenGL extension headers
#include "ThirdParty/OpenGL/include/GL/glext.h"

// Define GL_GLEXT_PROTOTYPES to ensure function prototypes are declared
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

// Constructor
Model::Model() {
    // Initialize model properties
}

// Destructor
Model::~Model() {
    // Clean up model resources
}

// Load model from file
bool Model::LoadFromFile(const std::string& filename) {
    std::cout << "Loading model from file: " << filename << std::endl;
    
    // Simple implementation to load a model from file
    // In a real implementation, this would parse the file format
    // and load vertices, normals, textures, etc.
    
    // For now, just create a simple cube
    // Convert Vector3 to GLfloat for vertices
    std::vector<GLfloat> cubeVertices = {
        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        
        // Back face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f
    };
    
    // Assign vertices
    vertices = cubeVertices;
    
    // Create indices for faces
    std::vector<GLuint> cubeIndices = {
        // Front face
        0, 1, 2,
        0, 2, 3,
        
        // Back face
        4, 6, 5,
        4, 7, 6,
        
        // Left face
        4, 0, 3,
        4, 3, 7,
        
        // Right face
        1, 5, 6,
        1, 6, 2,
        
        // Top face
        3, 2, 6,
        3, 6, 7,
        
        // Bottom face
        4, 5, 1,
        4, 1, 0
    };
    
    // Assign indices
    indices = cubeIndices;
    
    return true;
}

// Render the model
void Model::Render(const std::vector<PointLight>& lights) {
    // Check if shader program is set
    if (!shaderProgram) {
        std::cout << "Warning: No shader program set for model" << std::endl;
        return;
    }
    
    // Check if we have vertices and indices
    if (vertices.empty() || indices.empty()) {
        std::cout << "Warning: Model has no vertices or indices" << std::endl;
        return;
    }
    
    // Check if OpenGL buffers are initialized
    if (vao == 0 || vbo == 0 || ebo == 0) {
        InitializeGL();
    }
    
    // Use shader program
    shaderProgram->Use();
    
    // Set model matrix
    Matrix4x4 modelMatrix;
    modelMatrix.identity();
    
    // Apply translation
    modelMatrix.translate(position.x, position.y, position.z);
    
    // Apply rotation
    modelMatrix.rotateX(rotation.x);
    modelMatrix.rotateY(rotation.y);
    modelMatrix.rotateZ(rotation.z);
    
    // Set model matrix uniform
    shaderProgram->SetUniform("model", modelMatrix);
    
    // Set number of lights
    int numLights = lights.size();
    shaderProgram->SetUniform("numLights", numLights);
    
    // Set light properties
    for (int i = 0; i < numLights && i < 8; i++) { // Limit to 8 lights for performance
        const PointLight& light = lights[i];
        
        // Set light position
        std::string posName = "pointLights[" + std::to_string(i) + "].position";
        shaderProgram->SetUniform(posName, light.GetPosition());
        
        // Set light color
        std::string colorName = "pointLights[" + std::to_string(i) + "].color";
        shaderProgram->SetUniform(colorName, light.GetColor());
        
        // Set light intensity
        std::string intensityName = "pointLights[" + std::to_string(i) + "].intensity";
        shaderProgram->SetUniform(intensityName, light.GetIntensity());
        
        // Set light range
        std::string rangeName = "pointLights[" + std::to_string(i) + "].range";
        shaderProgram->SetUniform(rangeName, light.GetRange());
    }
    
    // Bind textures if available
    if (albedoTexture) {
        glActiveTexture(GL_TEXTURE0);
        albedoTexture->bind();
        shaderProgram->SetUniform("albedoTexture", 0);
        shaderProgram->SetUniform("hasAlbedoTexture", 1);
    } else {
        shaderProgram->SetUniform("hasAlbedoTexture", 0);
    }
    
    if (normalTexture) {
        glActiveTexture(GL_TEXTURE1);
        normalTexture->bind();
        shaderProgram->SetUniform("normalTexture", 1);
        shaderProgram->SetUniform("hasNormalTexture", 1);
    } else {
        shaderProgram->SetUniform("hasNormalTexture", 0);
    }
    
    if (opacityTexture) {
        glActiveTexture(GL_TEXTURE2);
        opacityTexture->bind();
        shaderProgram->SetUniform("opacityTexture", 2);
        shaderProgram->SetUniform("hasOpacityTexture", 1);
    } else {
        shaderProgram->SetUniform("hasOpacityTexture", 0);
    }
    
    // Bind VAO
    glBindVertexArray(vao);
    
    // Draw the model
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind VAO
    glBindVertexArray(0);
    
    // Unbind textures
    if (albedoTexture) {
        glActiveTexture(GL_TEXTURE0);
        albedoTexture->unbind();
    }
    
    if (normalTexture) {
        glActiveTexture(GL_TEXTURE1);
        normalTexture->unbind();
    }
    
    if (opacityTexture) {
        glActiveTexture(GL_TEXTURE2);
        opacityTexture->unbind();
    }
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during rendering: " << error << std::endl;
    }
}

// Draw model (compatibility method)
void Model::draw(const std::vector<PointLight>& pointLights) {
    // Call the Render method for compatibility
    Render(pointLights);
}

// Draw model (compatibility method)
void Model::drawcomp(const std::vector<PointLight>& pointLights) {
    // Call the Render method for compatibility
    Render(pointLights);
}

// Set the shader program for this model
void Model::SetShaderProgram(ShaderProgram* program) {
    shaderProgram = program;
    std::cout << "Shader program set for model" << std::endl;
}

// Initialize OpenGL objects
void Model::InitializeGL() {
    // Clean up any existing OpenGL objects
    CleanupGL();
    
    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // Generate and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    // Upload vertex data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    
    // Generate and bind EBO
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    
    // Upload index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    
    // Set up vertex attributes (position)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Generate and bind texture coordinate buffer if we have texture coordinates
    if (!texCoords.empty()) {
        glGenBuffers(1, &tbo);
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        
        // Set up texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(1);
    }
    
    // Generate and bind normal buffer if we have normals
    if (!normals.empty()) {
        glGenBuffers(1, &nbo);
        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
        
        // Set up normal attribute
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(2);
    }
    
    // Unbind VAO
    glBindVertexArray(0);
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during initialization: " << error << std::endl;
    } else {
        std::cout << "OpenGL objects initialized successfully" << std::endl;
    }
}

// Update OpenGL buffers
void Model::UpdateBuffers() {
    // Check if buffers exist, if not initialize them
    if (vbo == 0 || ebo == 0) {
        InitializeGL();
        return;
    }
    
    // Bind VAO
    glBindVertexArray(vao);
    
    // Update VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    
    // Update EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    
    // Update texture coordinates if available
    if (!texCoords.empty() && tbo != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
    }
    
    // Update normals if available
    if (!normals.empty() && nbo != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, nbo);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    }
    
    // Unbind VAO
    glBindVertexArray(0);
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during buffer update: " << error << std::endl;
    } else {
        std::cout << "OpenGL buffers updated successfully" << std::endl;
    }
}

// Clean up OpenGL objects
void Model::CleanupGL() {
    // Delete VAO
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    
    // Delete VBO
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    
    // Delete EBO
    if (ebo != 0) {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
    
    // Delete TBO
    if (tbo != 0) {
        glDeleteBuffers(1, &tbo);
        tbo = 0;
    }
    
    // Delete NBO
    if (nbo != 0) {
        glDeleteBuffers(1, &nbo);
        nbo = 0;
    }
    
    std::cout << "OpenGL objects cleaned up" << std::endl;
}

// Initialize buffers
void Model::InitializeBuffers() {
    // Call InitializeGL to set up OpenGL objects
    InitializeGL();
}

// Update vertices
void Model::UpdateVertices(const std::vector<GLfloat>& newVertices) {
    vertices = newVertices;
    
    // Update OpenGL buffers
    UpdateBuffers();
    
    // Call callback if set
    if (onVerticesUpdated) {
        onVerticesUpdated(this);
    }
}

// Update uniforms
void Model::UpdateUniforms(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) {
    if (!shaderProgram) {
        std::cout << "Warning: No shader program set for model" << std::endl;
        return;
    }
    
    // Use shader program
    shaderProgram->Use();
    
    // Create model matrix
    Matrix4x4 modelMatrix;
    modelMatrix.identity();
    
    // Apply translation
    modelMatrix.translate(position.x, position.y, position.z);
    
    // Apply rotation
    modelMatrix.rotateX(rotation.x);
    modelMatrix.rotateY(rotation.y);
    modelMatrix.rotateZ(rotation.z);
    
    // Set model matrix uniform
    shaderProgram->SetUniform("model", modelMatrix);
    
    // Set view matrix uniform
    shaderProgram->SetUniform("view", viewMatrix);
    
    // Set projection matrix uniform
    shaderProgram->SetUniform("projection", projectionMatrix);
    
    // Set additional uniforms for lighting
    shaderProgram->SetUniform("viewPos", Vector3(0.0f, 0.0f, 5.0f)); // Camera position
    
    // Set material properties
    shaderProgram->SetUniform("material.shininess", 32.0f);
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error during uniform update: " << error << std::endl;
    }
}

// Create a cube
void Model::CreateCube() {
    // Reuse the cube creation logic from LoadFromFile
    std::vector<GLfloat> cubeVertices = {
        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        
        // Back face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f
    };
    
    // Assign vertices
    vertices = cubeVertices;
    
    // Create indices for faces
    std::vector<GLuint> cubeIndices = {
        // Front face
        0, 1, 2,
        0, 2, 3,
        
        // Back face
        4, 6, 5,
        4, 7, 6,
        
        // Left face
        4, 0, 3,
        4, 3, 7,
        
        // Right face
        1, 5, 6,
        1, 6, 2,
        
        // Top face
        3, 2, 6,
        3, 6, 7,
        
        // Bottom face
        4, 5, 1,
        4, 1, 0
    };
    
    // Assign indices
    indices = cubeIndices;
    
    // Initialize OpenGL buffers
    InitializeBuffers();
    
    std::cout << "Created cube model" << std::endl;
}

// Create a plane
void Model::CreatePlane() {
    // Create a simple plane
    std::vector<GLfloat> planeVertices = {
        // Vertices
        -0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, 0.5f
    };
    
    // Assign vertices
    vertices = planeVertices;
    
    // Create indices
    std::vector<GLuint> planeIndices = {
        0, 1, 2,
        0, 2, 3
    };
    
    // Assign indices
    indices = planeIndices;
    
    // Initialize OpenGL buffers
    InitializeBuffers();
    
    std::cout << "Created plane model" << std::endl;
}

// Create a sphere
void Model::CreateSphere(float radius, int segments) {
    // Create a simple sphere
    std::vector<GLfloat> sphereVertices;
    std::vector<GLuint> sphereIndices;
    
    // Add top vertex
    sphereVertices.push_back(0.0f);
    sphereVertices.push_back(radius);
    sphereVertices.push_back(0.0f);
    
    // Add middle vertices
    for (int i = 1; i < segments; i++) {
        float phi = 3.14159f * (float)i / (float)segments;
        float sinPhi = sin(phi);
        float cosPhi = cos(phi);
        
        for (int j = 0; j < segments; j++) {
            float theta = 2.0f * 3.14159f * (float)j / (float)segments;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);
            
            float x = radius * sinPhi * cosTheta;
            float y = radius * cosPhi;
            float z = radius * sinPhi * sinTheta;
            
            sphereVertices.push_back(x);
            sphereVertices.push_back(y);
            sphereVertices.push_back(z);
        }
    }
    
    // Add bottom vertex
    sphereVertices.push_back(0.0f);
    sphereVertices.push_back(-radius);
    sphereVertices.push_back(0.0f);
    
    // Create indices
    // Top cap
    for (int i = 0; i < segments; i++) {
        sphereIndices.push_back(0);
        sphereIndices.push_back(i + 1);
        sphereIndices.push_back(((i + 1) % segments) + 1);
    }
    
    // Middle segments
    for (int i = 0; i < segments - 2; i++) {
        for (int j = 0; j < segments; j++) {
            int current = i * segments + j + 1;
            int next = i * segments + ((j + 1) % segments) + 1;
            int currentBelow = (i + 1) * segments + j + 1;
            int nextBelow = (i + 1) * segments + ((j + 1) % segments) + 1;
            
            sphereIndices.push_back(current);
            sphereIndices.push_back(next);
            sphereIndices.push_back(nextBelow);
            
            sphereIndices.push_back(current);
            sphereIndices.push_back(nextBelow);
            sphereIndices.push_back(currentBelow);
        }
    }
    
    // Bottom cap
    int bottomVertex = sphereVertices.size() / 3 - 1;
    for (int i = 0; i < segments; i++) {
        int current = (segments - 2) * segments + i + 1;
        int next = (segments - 2) * segments + ((i + 1) % segments) + 1;
        
        sphereIndices.push_back(bottomVertex);
        sphereIndices.push_back(next);
        sphereIndices.push_back(current);
    }
    
    // Assign vertices and indices
    vertices = sphereVertices;
    indices = sphereIndices;
    
    // Initialize OpenGL buffers
    InitializeBuffers();
    
    std::cout << "Created sphere model with radius " << radius << " and " << segments << " segments" << std::endl;
}

// Load model from OBJ file
void Model::loadOBJ(std::string path) {
    std::cout << "Loading OBJ file: " << path << std::endl;
    
    // Clear existing data
    vertices.clear();
    indices.clear();
    texCoords.clear();
    normals.clear();
    
    // Open the file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open OBJ file: " << path << std::endl;
        // Fallback to a cube if file can't be opened
        CreateCube();
        return;
    }
    
    // Temporary storage for vertices, texture coordinates, and normals
    std::vector<Vector3> tempVertices;
    std::vector<Vector2> tempTexCoords;
    std::vector<Vector3> tempNormals;
    
    // Parse the file
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;
        
        if (token == "v") {
            // Vertex
            float x, y, z;
            iss >> x >> y >> z;
            tempVertices.push_back(Vector3(x, y, z));
        } else if (token == "vt") {
            // Texture coordinate
            float u, v;
            iss >> u >> v;
            tempTexCoords.push_back(Vector2(u, v));
        } else if (token == "vn") {
            // Normal
            float x, y, z;
            iss >> x >> y >> z;
            tempNormals.push_back(Vector3(x, y, z));
        } else if (token == "f") {
            // Face
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;
            
            // Process each vertex of the face
            std::vector<std::string> vertices = {vertex1, vertex2, vertex3};
            std::vector<int> vertexIndices;
            std::vector<int> texCoordIndices;
            std::vector<int> normalIndices;
            
            for (const auto& vertex : vertices) {
                // Parse vertex data (format: v/vt/vn or v//vn or v/vt)
                std::istringstream viss(vertex);
                std::string indexStr;
                
                // Vertex index
                std::getline(viss, indexStr, '/');
                if (!indexStr.empty()) {
                    vertexIndices.push_back(std::stoi(indexStr) - 1); // OBJ indices start at 1
                }
                
                // Texture coordinate index
                std::getline(viss, indexStr, '/');
                if (!indexStr.empty()) {
                    texCoordIndices.push_back(std::stoi(indexStr) - 1);
                }
                
                // Normal index
                std::getline(viss, indexStr, '/');
                if (!indexStr.empty()) {
                    normalIndices.push_back(std::stoi(indexStr) - 1);
                }
            }
            
            // Add indices
            for (size_t i = 0; i < vertexIndices.size(); i++) {
                indices.push_back(vertexIndices[i]);
            }
        }
    }
    
    // Convert to flat arrays for OpenGL
    for (const auto& vertex : tempVertices) {
        vertices.push_back(vertex.x);
        vertices.push_back(vertex.y);
        vertices.push_back(vertex.z);
    }
    
    for (const auto& texCoord : tempTexCoords) {
        texCoords.push_back(texCoord.x);
        texCoords.push_back(texCoord.y);
    }
    
    for (const auto& normal : tempNormals) {
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }
    
    // Close the file
    file.close();
    
    // Check if we loaded any data
    if (vertices.empty() || indices.empty()) {
        std::cerr << "Warning: No vertices or indices loaded from OBJ file. Creating fallback cube." << std::endl;
        CreateCube();
        return;
    }
    
    // Initialize OpenGL buffers
    InitializeBuffers();
    
    std::cout << "Successfully loaded OBJ file: " << path << std::endl;
    std::cout << "Vertices: " << vertices.size() / 3 << ", Indices: " << indices.size() << std::endl;
}

// Load texture
void Model::loadTexture(const std::string& path, const std::string& type) {
    std::cout << "Loading texture: " << path << " of type " << type << std::endl;
    
    // Check if the path is a previous frame PNG
    bool isPreviousFrame = path.find("frame") != std::string::npos && path.find(".png") != std::string::npos;
    
    // Create texture based on type
    if (type == "albedo") {
        if (albedoTexture) {
            delete albedoTexture;
        }
        
        albedoTexture = new Texture();
        if (albedoTexture->load(path)) {
            std::cout << "Loaded albedo texture: " << path << std::endl;
        } else {
            std::cerr << "Failed to load albedo texture: " << path << std::endl;
            
            // Try to load a fallback texture from previous frames
            if (!isPreviousFrame) {
                std::string fallbackPath = "frames/frame0.png";
                std::cout << "Attempting to load fallback texture: " << fallbackPath << std::endl;
                
                if (albedoTexture->load(fallbackPath)) {
                    std::cout << "Loaded fallback albedo texture: " << fallbackPath << std::endl;
                } else {
                    std::cerr << "Failed to load fallback albedo texture" << std::endl;
                    delete albedoTexture;
                    albedoTexture = nullptr;
                }
            } else {
                delete albedoTexture;
                albedoTexture = nullptr;
            }
        }
    } else if (type == "normal") {
        if (normalTexture) {
            delete normalTexture;
        }
        
        normalTexture = new Texture();
        if (normalTexture->load(path)) {
            std::cout << "Loaded normal texture: " << path << std::endl;
        } else {
            std::cerr << "Failed to load normal texture: " << path << std::endl;
            
            // Try to load a fallback texture from previous frames
            if (!isPreviousFrame) {
                std::string fallbackPath = "frames/frame0.png";
                std::cout << "Attempting to load fallback texture: " << fallbackPath << std::endl;
                
                if (normalTexture->load(fallbackPath)) {
                    std::cout << "Loaded fallback normal texture: " << fallbackPath << std::endl;
                } else {
                    std::cerr << "Failed to load fallback normal texture" << std::endl;
                    delete normalTexture;
                    normalTexture = nullptr;
                }
            } else {
                delete normalTexture;
                normalTexture = nullptr;
            }
        }
    } else if (type == "opacity") {
        if (opacityTexture) {
            delete opacityTexture;
        }
        
        opacityTexture = new Texture();
        if (opacityTexture->load(path)) {
            std::cout << "Loaded opacity texture: " << path << std::endl;
        } else {
            std::cerr << "Failed to load opacity texture: " << path << std::endl;
            
            // Try to load a fallback texture from previous frames
            if (!isPreviousFrame) {
                std::string fallbackPath = "frames/frame0.png";
                std::cout << "Attempting to load fallback texture: " << fallbackPath << std::endl;
                
                if (opacityTexture->load(fallbackPath)) {
                    std::cout << "Loaded fallback opacity texture: " << fallbackPath << std::endl;
                } else {
                    std::cerr << "Failed to load fallback opacity texture" << std::endl;
                    delete opacityTexture;
                    opacityTexture = nullptr;
                }
            } else {
                delete opacityTexture;
                opacityTexture = nullptr;
            }
        }
    } else {
        std::cerr << "Unknown texture type: " << type << std::endl;
    }
}

// Set texture tiling
void Model::setTiling(float x, float y) {
    // Generate texture coordinates if they don't exist
    if (texCoords.empty() && !vertices.empty()) {
        // Create default texture coordinates (0-1 range)
        texCoords.resize(vertices.size() / 3 * 2);
        for (size_t i = 0, j = 0; i < vertices.size(); i += 3, j += 2) {
            // Default mapping: x,z to u,v (assuming y is up)
            texCoords[j] = (vertices[i] + 0.5f); // u (x + 0.5 to map from -0.5,0.5 to 0,1)
            texCoords[j + 1] = (vertices[i + 2] + 0.5f); // v (z + 0.5 to map from -0.5,0.5 to 0,1)
        }
    }
    
    // Apply tiling by scaling texture coordinates
    for (size_t i = 0; i < texCoords.size(); i += 2) {
        // Scale u by x
        texCoords[i] *= x;
        // Scale v by y
        texCoords[i + 1] *= y;
    }
    
    // Update texture coordinate buffer if it exists
    if (tbo != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, tbo);
        glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    
    std::cout << "Set texture tiling to (" << x << ", " << y << ")" << std::endl;
}

// Translate model
void Model::translate(const Vector3& vec) {
    // Update the model's position
    position = position + vec;
    
    // Log the translation
    std::cout << "Translated model to (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
    
    // No need to update buffers since translation is applied in the model matrix
    // during rendering, not in the vertex data itself
}

// Rotate model
void Model::rotate() {
    // Rotate the model around the Y axis
    rotation.y += 1.0f;
    if (rotation.y > 360.0f) {
        rotation.y -= 360.0f;
    }
    
    // Also add a slight rotation on X and Z for more interesting movement
    rotation.x += 0.2f;
    if (rotation.x > 360.0f) {
        rotation.x -= 360.0f;
    }
    
    rotation.z += 0.1f;
    if (rotation.z > 360.0f) {
        rotation.z -= 360.0f;
    }
    
    std::cout << "Rotated model to (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")" << std::endl;
}

// Set texture path
void Model::SetTexturePath(const std::string& path) {
    texturePath = path;
}

// Get shader program
ShaderProgram* Model::GetShaderProgram() const {
    return shaderProgram;
}

// Get texture path
const std::string& Model::GetTexturePath() const {
    return texturePath;
}
