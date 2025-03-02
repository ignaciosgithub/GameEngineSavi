#include "Model.h"
#include "Face.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>

// Define OpenGL functions if they're not already defined
#ifndef glBindVertexArray
#define glBindVertexArray(x) (void)(x)
#endif

#ifndef glGenVertexArrays
#define glGenVertexArrays(n, arrays) (void)(n); (void)(arrays)
#endif

#ifndef glGenBuffers
#define glGenBuffers(n, buffers) (void)(n); (void)(buffers)
#endif

#ifndef glBindBuffer
#define glBindBuffer(target, buffer) (void)(target); (void)(buffer)
#endif

#ifndef glBufferData
#define glBufferData(target, size, data, usage) (void)(target); (void)(size); (void)(data); (void)(usage)
#endif

#ifndef glVertexAttribPointer
#define glVertexAttribPointer(index, size, type, normalized, stride, pointer) (void)(index); (void)(size); (void)(type); (void)(normalized); (void)(stride); (void)(pointer)
#endif

#ifndef glEnableVertexAttribArray
#define glEnableVertexAttribArray(index) (void)(index)
#endif

#ifndef glDrawElements
#define glDrawElements(mode, count, type, indices) (void)(mode); (void)(count); (void)(type); (void)(indices)
#endif

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
        albedoTexture->load(path);
    } else if (type == "normal") {
        if (!normalTexture) {
            normalTexture = new Texture();
        }
        normalTexture->load(path);
    } else if (type == "opacity") {
        if (!opacityTexture) {
            opacityTexture = new Texture();
        }
        opacityTexture->load(path);
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

void Model::SetShaderProgram(ShaderProgram* program) {
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
    // Convert Matrix4x4 to float array for shader uniforms
    float modelMatrixArray[16];
    // Manual conversion since toArray is not available
    modelMatrixArray[0] = modelMatrix.elements[0][0]; modelMatrixArray[1] = modelMatrix.elements[0][1];
    modelMatrixArray[2] = modelMatrix.elements[0][2]; modelMatrixArray[3] = modelMatrix.elements[0][3];
    modelMatrixArray[4] = modelMatrix.elements[1][0]; modelMatrixArray[5] = modelMatrix.elements[1][1];
    modelMatrixArray[6] = modelMatrix.elements[1][2]; modelMatrixArray[7] = modelMatrix.elements[1][3];
    modelMatrixArray[8] = modelMatrix.elements[2][0]; modelMatrixArray[9] = modelMatrix.elements[2][1];
    modelMatrixArray[10] = modelMatrix.elements[2][2]; modelMatrixArray[11] = modelMatrix.elements[2][3];
    modelMatrixArray[12] = modelMatrix.elements[3][0]; modelMatrixArray[13] = modelMatrix.elements[3][1];
    modelMatrixArray[14] = modelMatrix.elements[3][2]; modelMatrixArray[15] = modelMatrix.elements[3][3];
    
    float viewMatrixArray[16];
    // Manual conversion since toArray is not available
    viewMatrixArray[0] = viewMatrix.elements[0][0]; viewMatrixArray[1] = viewMatrix.elements[0][1];
    viewMatrixArray[2] = viewMatrix.elements[0][2]; viewMatrixArray[3] = viewMatrix.elements[0][3];
    viewMatrixArray[4] = viewMatrix.elements[1][0]; viewMatrixArray[5] = viewMatrix.elements[1][1];
    viewMatrixArray[6] = viewMatrix.elements[1][2]; viewMatrixArray[7] = viewMatrix.elements[1][3];
    viewMatrixArray[8] = viewMatrix.elements[2][0]; viewMatrixArray[9] = viewMatrix.elements[2][1];
    viewMatrixArray[10] = viewMatrix.elements[2][2]; viewMatrixArray[11] = viewMatrix.elements[2][3];
    viewMatrixArray[12] = viewMatrix.elements[3][0]; viewMatrixArray[13] = viewMatrix.elements[3][1];
    viewMatrixArray[14] = viewMatrix.elements[3][2]; viewMatrixArray[15] = viewMatrix.elements[3][3];
    
    float projectionMatrixArray[16];
    // Manual conversion since toArray is not available
    projectionMatrixArray[0] = projectionMatrix.elements[0][0]; projectionMatrixArray[1] = projectionMatrix.elements[0][1];
    projectionMatrixArray[2] = projectionMatrix.elements[0][2]; projectionMatrixArray[3] = projectionMatrix.elements[0][3];
    projectionMatrixArray[4] = projectionMatrix.elements[1][0]; projectionMatrixArray[5] = projectionMatrix.elements[1][1];
    projectionMatrixArray[6] = projectionMatrix.elements[1][2]; projectionMatrixArray[7] = projectionMatrix.elements[1][3];
    projectionMatrixArray[8] = projectionMatrix.elements[2][0]; projectionMatrixArray[9] = projectionMatrix.elements[2][1];
    projectionMatrixArray[10] = projectionMatrix.elements[2][2]; projectionMatrixArray[11] = projectionMatrix.elements[2][3];
    projectionMatrixArray[12] = projectionMatrix.elements[3][0]; projectionMatrixArray[13] = projectionMatrix.elements[3][1];
    projectionMatrixArray[14] = projectionMatrix.elements[3][2]; projectionMatrixArray[15] = projectionMatrix.elements[3][3];
    
    shaderProgram->SetUniform("model", modelMatrixArray);
    shaderProgram->SetUniform("view", viewMatrixArray);
    shaderProgram->SetUniform("projection", projectionMatrixArray);
    
    // Set texture uniforms
    if (albedoTexture) {
        shaderProgram->SetUniform("albedoTexture", 0);
        shaderProgram->SetUniform("hasAlbedoTexture", true);
    } else {
        shaderProgram->SetUniform("hasAlbedoTexture", false);
    }
    
    if (normalTexture) {
        shaderProgram->SetUniform("normalTexture", 1);
        shaderProgram->SetUniform("hasNormalTexture", true);
    } else {
        shaderProgram->SetUniform("hasNormalTexture", false);
    }
    
    if (opacityTexture) {
        shaderProgram->SetUniform("opacityTexture", 2);
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
