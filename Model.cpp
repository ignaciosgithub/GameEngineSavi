#include "Model.h"
#include <iostream>

// Implementation of translate method (declared in header)
void Model::translate(const Vector3& vec) {
    position += vec;
}

// Update the model's vertices with new vertex positions
void Model::UpdateVertices(const std::vector<GLfloat>& newVertices) {
    // Ensure the new vertices vector has the same size as the current vertices vector
    if (newVertices.size() != vertices.size()) {
        std::cerr << "Cannot update vertices: size mismatch" << std::endl;
        return;
    }
    
    // Update the vertices
    vertices = newVertices;
    
    // Notify listeners that vertices have been updated
    if (onVerticesUpdated) {
        onVerticesUpdated(this);
    }
}
