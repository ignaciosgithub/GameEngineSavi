#include "Model.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

void Model::loadOBJ(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << path << std::endl;
        return;
    }

    std::vector<Vector3> temp_vertices;
    std::vector<Vector3> temp_normals;
    std::vector<Vector2> temp_texcoords;
    std::vector<Face> temp_faces;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            temp_vertices.push_back(Vector3(x, y, z));
        } else if (prefix == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            temp_normals.push_back(Vector3(x, y, z));
        } else if (prefix == "vt") {
            float u, v;
            iss >> u >> v;
            temp_texcoords.push_back(Vector2(u, v));
        } else if (prefix == "f") {
            Face face;
            std::string vertex;
            while (iss >> vertex) {
                std::istringstream viss(vertex);
                std::string index_str;
                int indices[3] = {0, 0, 0}; // v/vt/vn
                int i = 0;
                while (std::getline(viss, index_str, '/')) {
                    if (!index_str.empty()) {
                        indices[i] = std::stoi(index_str) - 1; // OBJ indices start at 1
                    }
                    i++;
                }
                face.vertex_indices.push_back(indices[0]);
                if (i > 1 && !index_str.empty()) face.texcoord_indices.push_back(indices[1]);
                if (i > 2) face.normal_indices.push_back(indices[2]);
            }
            temp_faces.push_back(face);
        }
    }

    file.close();

    // Process the data into a format suitable for OpenGL
    for (const auto& face : temp_faces) {
        for (size_t i = 0; i < face.vertex_indices.size(); i++) {
            int vertex_index = face.vertex_indices[i];
            
            // Add vertex coordinates
            vertices.push_back(temp_vertices[vertex_index].x);
            vertices.push_back(temp_vertices[vertex_index].y);
            vertices.push_back(temp_vertices[vertex_index].z);
            
            // Add texture coordinates if available
            if (!face.texcoord_indices.empty() && i < face.texcoord_indices.size()) {
                int texcoord_index = face.texcoord_indices[i];
                if (texcoord_index >= 0 && texcoord_index < temp_texcoords.size()) {
                    texCoords.push_back(temp_texcoords[texcoord_index].x);
                    texCoords.push_back(temp_texcoords[texcoord_index].y);
                } else {
                    texCoords.push_back(0.0f);
                    texCoords.push_back(0.0f);
                }
            } else {
                texCoords.push_back(0.0f);
                texCoords.push_back(0.0f);
            }
            
            // Add normal coordinates if available
            if (!face.normal_indices.empty() && i < face.normal_indices.size()) {
                int normal_index = face.normal_indices[i];
                if (normal_index >= 0 && normal_index < temp_normals.size()) {
                    normals.push_back(temp_normals[normal_index].x);
                    normals.push_back(temp_normals[normal_index].y);
                    normals.push_back(temp_normals[normal_index].z);
                } else {
                    // Calculate normal if not provided
                    Vector3 normal = calculateNormal(vertices.size() - 3);
                    normals.push_back(normal.x);
                    normals.push_back(normal.y);
                    normals.push_back(normal.z);
                }
            } else {
                // Calculate normal if not provided
                Vector3 normal = calculateNormal(vertices.size() - 3);
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            }
            
            // Add index
            indices.push_back(indices.size());
        }
    }

    // Initialize OpenGL buffers
    InitializeBuffers();
    
    std::cout << "Loaded model: " << path << " with " << vertices.size() / 3 << " vertices" << std::endl;
}

void Model::loadTexture(const std::string& path, const std::string& type) {
    // Create texture if it doesn't exist
    if (type == "albedo") {
        if (!albedoTexture) {
            albedoTexture = new Texture();
        }
        albedoTexture->loadFromFile(path);
    } else if (type == "normal") {
        if (!normalTexture) {
            normalTexture = new Texture();
        }
        normalTexture->loadFromFile(path);
    } else if (type == "opacity") {
        if (!opacityTexture) {
            opacityTexture = new Texture();
        }
        opacityTexture->loadFromFile(path);
    }
}

void Model::setTiling(float x, float y) {
    // Set tiling for all textures
    if (albedoTexture) {
        albedoTexture->setTiling(x, y);
    }
    if (normalTexture) {
        normalTexture->setTiling(x, y);
    }
    if (opacityTexture) {
        opacityTexture->setTiling(x, y);
    }
}

void Model::translate(const Vector3& vec) {
    position = position + vec;
}

void Model::UpdateVertices(const std::vector<GLfloat>& newVertices) {
    if (newVertices.size() != vertices.size()) {
        std::cerr << "Error: New vertices size does not match current vertices size" << std::endl;
        return;
    }
    
    // Update vertices
    vertices = newVertices;
    
    // Update buffers
    UpdateBuffers();
    
    // Call callback if set
    if (onVerticesUpdated) {
        onVerticesUpdated(this);
    }
}

void Model::SetShaderProgram(Shaders::ShaderProgram* program) {
    shaderProgram = program;
}

void Model::UpdateUniforms(const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix) {
    if (!shaderProgram) {
        return;
    }
    
    // Create model matrix
    Matrix4x4 modelMatrix;
    modelMatrix.identity();
    
    // Apply transformations
    // TODO: Implement proper model matrix with rotation and scale
    
    // Set uniforms
    shaderProgram->SetUniform("model", modelMatrix);
    shaderProgram->SetUniform("view", viewMatrix);
    shaderProgram->SetUniform("projection", projectionMatrix);
    
    // Set texture uniforms
    if (albedoTexture) {
        shaderProgram->SetUniform("albedoTexture", 0, albedoTexture->getTextureID());
        shaderProgram->SetUniform("hasAlbedoTexture", true);
    } else {
        shaderProgram->SetUniform("hasAlbedoTexture", false);
    }
    
    if (normalTexture) {
        shaderProgram->SetUniform("normalTexture", 1, normalTexture->getTextureID());
        shaderProgram->SetUniform("hasNormalTexture", true);
    } else {
        shaderProgram->SetUniform("hasNormalTexture", false);
    }
    
    if (opacityTexture) {
        shaderProgram->SetUniform("opacityTexture", 2, opacityTexture->getTextureID());
        shaderProgram->SetUniform("hasOpacityTexture", true);
    } else {
        shaderProgram->SetUniform("hasOpacityTexture", false);
    }
}

void Model::Render(const std::vector<PointLight>& pointLights) {
    // Use shader program if available
    if (shaderProgram) {
        shaderProgram->Use();
    }
    
    // Bind VAO
    glBindVertexArray(vao);
    
    // Draw elements
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // Unbind VAO
    glBindVertexArray(0);
}

void Model::draw(const std::vector<PointLight>& pointLights) {
    Render(pointLights);
}

void Model::rotate() {
    // Rotate model
    rotation.y += 0.01f;
    if (rotation.y > 360.0f) {
        rotation.y -= 360.0f;
    }
}

void Model::drawcomp(const std::vector<PointLight>& pointLights) {
    // Compatibility method for older code
    Render(pointLights);
}

void Model::InitializeBuffers() {
    // Generate VAO
    if (vao == 0) {
        glGenVertexArrays(1, &vao);
    }
    
    // Generate VBO
    if (vbo == 0) {
        glGenBuffers(1, &vbo);
    }
    
    // Generate EBO
    if (ebo == 0) {
        glGenBuffers(1, &ebo);
    }
    
    // Generate TBO
    if (tbo == 0) {
        glGenBuffers(1, &tbo);
    }
    
    // Generate NBO
    if (nbo == 0) {
        glGenBuffers(1, &nbo);
    }
    
    // Bind VAO
    glBindVertexArray(vao);
    
    // Bind and set VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    
    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Bind and set TBO
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(GLfloat), texCoords.data(), GL_STATIC_DRAW);
    
    // Set texture coordinate attribute pointers
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    
    // Bind and set NBO
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    
    // Set normal attribute pointers
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(2);
    
    // Bind and set EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
    
    // Unbind VAO
    glBindVertexArray(0);
}

void Model::UpdateBuffers() {
    // Bind VAO
    glBindVertexArray(vao);
    
    // Update VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    
    // Update NBO
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);
    
    // Unbind VAO
    glBindVertexArray(0);
}
