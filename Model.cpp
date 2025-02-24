#include <gl/gl.h>
#include <GL/glu.h>
#include <windows.h>
#include <gl/gl.h>

#include<vector>
#include"Vector3.h"
#include"Triangle.h"
#include"Matrix4x4.h"
#include"Raycast.h"
#include"PointLight.h"
#include"Camera.h"
#include"Face.h"
#include"Pyramid.h"
#include"Texture.h"

#include <fstream>
#include<string>
#include <sstream>
#include<math.h>
#include <algorithm>

inline float toRadians(float degrees) {
    return degrees * static_cast<float>(3.14f) / 180.0f;
}

void Model::loadOBJ(std::string path) {
    std::ifstream file(path);
    std::string line;
    
    std::vector<Vector3> tempVertices;
    std::vector<Vector3> tempTexCoords;
    
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") {
            std::istringstream s(line.substr(2));
            Vector3 v;
            s >> v.x; s >> v.y; s >> v.z;
            tempVertices.push_back(v);
        }
        else if (line.substr(0, 3) == "vt ") {
            std::istringstream s(line.substr(3));
            float u, v;
            s >> u; s >> v;
            tempTexCoords.push_back(Vector3(u, v, 0));
        }
        else if (line.substr(0, 2) == "f ") {
            std::istringstream s(line.substr(2));
            GLuint a, b, c; // to store vertex indices
            GLuint at, bt, ct; // to store texture coordinate indices
            
            // Check if the face has texture coordinates
            std::string faceData = line.substr(2);
            bool hasTexCoords = faceData.find('/') != std::string::npos;
            
            if (hasTexCoords) {
                // Format with texture coordinates: f v1/vt1 v2/vt2 v3/vt3
                char slash;
                s >> a >> slash >> at >> b >> slash >> bt >> c >> slash >> ct;
                
                // OBJ indices start at 1, adjust to 0-based
                a--; b--; c--;
                at--; bt--; ct--;
                
                // Store vertex indices
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(c);
                
                // Store texture coordinates for later use
                if (!tempTexCoords.empty()) {
                    // Make sure indices are valid
                    at = std::min(at, (GLuint)tempTexCoords.size() - 1);
                    bt = std::min(bt, (GLuint)tempTexCoords.size() - 1);
                    ct = std::min(ct, (GLuint)tempTexCoords.size() - 1);
                    
                    texCoords.push_back(tempTexCoords[at].x);
                    texCoords.push_back(tempTexCoords[at].y);
                    texCoords.push_back(tempTexCoords[bt].x);
                    texCoords.push_back(tempTexCoords[bt].y);
                    texCoords.push_back(tempTexCoords[ct].x);
                    texCoords.push_back(tempTexCoords[ct].y);
                }
            } else {
                // Simple format without texture coordinates: f v1 v2 v3
                s >> a >> b >> c;
                
                // OBJ indices start at 1, adjust to 0-based
                a--; b--; c--;
                
                // Store vertex indices
                indices.push_back(a);
                indices.push_back(b);
                indices.push_back(c);
                
                // Add default texture coordinates if none provided
                texCoords.push_back(0.0f); texCoords.push_back(0.0f);
                texCoords.push_back(1.0f); texCoords.push_back(0.0f);
                texCoords.push_back(0.5f); texCoords.push_back(1.0f);
            }
        }
    }
    
    // Create the actual vertex array from the indices
    for(GLuint i=0; i<indices.size(); i+=3) {
        Vector3 v1 = tempVertices[indices[i]];
        Vector3 v2 = tempVertices[indices[i+1]];
        Vector3 v3 = tempVertices[indices[i+2]];
        
        vertices.push_back(v1.x); vertices.push_back(v1.y); vertices.push_back(v1.z);
        vertices.push_back(v2.x); vertices.push_back(v2.y); vertices.push_back(v2.z);
        vertices.push_back(v3.x); vertices.push_back(v3.y); vertices.push_back(v3.z);
        
        // Initialize color map entries
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
        colourmap.push_back(Vector3(0.0f, 0.0f, 0.0f));
    }
    
    // If no texture coordinates were loaded, generate default ones
    if (texCoords.empty() && !vertices.empty()) {
        for (size_t i = 0; i < vertices.size(); i += 9) {
            // Simple planar mapping based on x,z coordinates
            texCoords.push_back(vertices[i] * 0.5f + 0.5f);
            texCoords.push_back(vertices[i+2] * 0.5f + 0.5f);
            
            texCoords.push_back(vertices[i+3] * 0.5f + 0.5f);
            texCoords.push_back(vertices[i+5] * 0.5f + 0.5f);
            
            texCoords.push_back(vertices[i+6] * 0.5f + 0.5f);
            texCoords.push_back(vertices[i+8] * 0.5f + 0.5f);
        }
    }
}

void Model::loadTexture(const std::string& path, const std::string& type) {
    if (type == "albedo") {
        if (albedoTexture) delete albedoTexture;
        albedoTexture = new Texture();
        albedoTexture->load(path);
    }
    else if (type == "normal") {
        if (normalTexture) delete normalTexture;
        normalTexture = new Texture();
        normalTexture->load(path);
    }
    else if (type == "opacity") {
        if (opacityTexture) delete opacityTexture;
        opacityTexture = new Texture();
        opacityTexture->load(path);
    }
}

void Model::setTiling(float x, float y) {
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
    for(size_t i=0; i<vertices.size(); i+=3) {
        vertices[i]   += vec.x;
        vertices[i+1] += vec.y;
        vertices[i+2] += vec.z;
    }
}

Vector3 Model::calculateNormal(size_t vertexIndex) {
    // Calculate face normal for the triangle
    if (vertexIndex + 8 >= vertices.size()) {
        return Vector3(0, 1, 0); // Default normal if index is out of bounds
    }
    
    Vector3 v1(vertices[vertexIndex], vertices[vertexIndex+1], vertices[vertexIndex+2]);
    Vector3 v2(vertices[vertexIndex+3], vertices[vertexIndex+4], vertices[vertexIndex+5]);
    Vector3 v3(vertices[vertexIndex+6], vertices[vertexIndex+7], vertices[vertexIndex+8]);
    
    // Calculate two edges of the triangle
    Vector3 edge1;
    edge1.x = v2.x - v1.x;
    edge1.y = v2.y - v1.y;
    edge1.z = v2.z - v1.z;
    
    Vector3 edge2;
    edge2.x = v3.x - v1.x;
    edge2.y = v3.y - v1.y;
    edge2.z = v3.z - v1.z;
    
    // Cross product to get normal
    Vector3 normal;
    normal.x = edge1.y * edge2.z - edge1.z * edge2.y;
    normal.y = edge1.z * edge2.x - edge1.x * edge2.z;
    normal.z = edge1.x * edge2.y - edge1.y * edge2.x;
    
    // Normalize
    float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
    if (length > 0) {
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
    }
    
    return normal;
}

void Model::Render(const std::vector<PointLight>& pointLights) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);
    
    // Enable texturing and blending for opacity
    if (albedoTexture || opacityTexture) {
        glEnable(GL_TEXTURE_2D);
    }
    
    if (opacityTexture) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    // Bind albedo texture if available
    if (albedoTexture) {
        glBindTexture(GL_TEXTURE_2D, albedoTexture->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    
    // Enable texture coordinates if we have textures
    if ((albedoTexture || normalTexture || opacityTexture) && !texCoords.empty()) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0]);
    }
    
    glBegin(GL_TRIANGLES);
    for(size_t i = 0; i < vertices.size(); i+=3) {
        Vector3 vertex = Vector3(vertices[i], vertices[i+1], vertices[i+2]);
        
        // Calculate normal for lighting
        Vector3 normal = calculateNormal(i);
        
        // Apply normal mapping if available
        if (normalTexture && i/3*2+1 < texCoords.size()) {
            // Sample normal map (simplified for now)
            float u = texCoords[i/3*2] * normalTexture->tiling_x;
            float v = texCoords[i/3*2+1] * normalTexture->tiling_y;
            
            // In a real implementation, we would sample the normal map texture here
            // For now, we'll just use the geometric normal
        }
        
        // Calculate lighting
        Vector3 colorSum(0, 0, 0);
        for (const auto& light : pointLights) {
            float distanceToLight = light.position.sqdist(vertex);
            float intensity = light.SQIntensity(distanceToLight);
            
            // Apply normal mapping to lighting calculation
            Vector3 lightDir;
            lightDir.x = light.position.x - vertex.x;
            lightDir.y = light.position.y - vertex.y;
            lightDir.z = light.position.z - vertex.z;
            
            // Normalize light direction
            float lightDirLength = sqrt(lightDir.x*lightDir.x + lightDir.y*lightDir.y + lightDir.z*lightDir.z);
            if (lightDirLength > 0) {
                lightDir.x /= lightDirLength;
                lightDir.y /= lightDirLength;
                lightDir.z /= lightDirLength;
            }
            
            // Calculate dot product for diffuse lighting
            float nDotL = normal.x * lightDir.x + normal.y * lightDir.y + normal.z * lightDir.z;
            if (nDotL < 0) nDotL = 0;
            
            intensity *= nDotL;
            
            Vector3 colorContribution;
            colorContribution.x = light.color.x * intensity;
            colorContribution.z = light.color.z * intensity;
            colorContribution.y = light.color.y * intensity;
            
            colorSum.x += colorContribution.x;
            colorSum.y += colorContribution.y;
            colorSum.z += colorContribution.z;
        }
        
        pcolorsum = colorSum;
        colorSum = colorSum.clamp(0.0f, 1.0f);
        
        // Set color
        glColor3f(colorSum.x, colorSum.y, colorSum.z);
        
        // Set texture coordinates if we have textures
        if ((albedoTexture || normalTexture || opacityTexture) && i/3*2+1 < texCoords.size()) {
            float u = texCoords[i/3*2];
            float v = texCoords[i/3*2+1];
            
            // Apply tiling
            if (albedoTexture) {
                u *= albedoTexture->tiling_x;
                v *= albedoTexture->tiling_y;
            }
            
            glTexCoord2f(u, v);
        }
        
        // Draw vertex
        glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
    }
    glEnd();
    
    // Disable states
    glDisableClientState(GL_VERTEX_ARRAY);
    
    if ((albedoTexture || normalTexture || opacityTexture) && !texCoords.empty()) {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    
    if (albedoTexture || opacityTexture) {
        glDisable(GL_TEXTURE_2D);
    }
    
    if (opacityTexture) {
        glDisable(GL_BLEND);
    }
    
    ++frameCounter;
}

void Model::draw(const std::vector<PointLight>& pointLights) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);

    glBegin(GL_TRIANGLES);
    for(size_t i = 0; i < vertices.size(); i+=3) {
        Vector3 vertex = Vector3(vertices[i], vertices[i+1], vertices[i+2]);

        Vector3 colorSum(0, 0, 0);
        for (const auto& light : pointLights) {
            // Calculate distance to the light source
            float distanceToLight;
            distanceToLight = light.position.distance(vertex);

            // Calculate color contribution from this light
            float intensity = light.Intensity(distanceToLight);
            Vector3 colorContribution;
            colorContribution.x = light.color.x * intensity;
            colorContribution.z = light.color.z * intensity;
            colorContribution.y = light.color.y * intensity;
            // Sum the color contributions
            colorSum.x += colorContribution.x;
            colorSum.y += colorContribution.y;
            colorSum.z += colorContribution.z;
        }

        // Ensure that the maximum color component value is 1.0
        colorSum = colorSum.clamp(0.0f, 1.0f);
       
        glColor3f(colorSum.x, colorSum.y, colorSum.z);
        glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
    }
    glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Model::rotate() {
    float radX = toRadians(rotation.x);
    float radY = toRadians(rotation.y);
    float radZ = toRadians(rotation.z);

    float cosX = cos(radX);
    float sinX = sin(radX);
    float cosY = cos(radY);
    float sinY = sin(radY);
    float cosZ = cos(radZ);
    float sinZ = sin(radZ);

    for (size_t i = 0; i < vertices.size(); i += 3) {
        // Retrieve original vertex positions
        float x = vertices[i], y = vertices[i + 1], z = vertices[i + 2];
        
        // Rotate around X axis
        float x1 = x;
        float y1 = y * cosX - z * sinX;
        float z1 = y * sinX + z * cosX;
        
        // Rotate around Y axis
        float x2 = x1 * cosY + z1 * sinY;
        float y2 = y1;
        float z2 = z1 * cosY - x1 * sinY;
        
        // Rotate around Z axis
        float xFinal = x2 * cosZ - y2 * sinZ;
        float yFinal = x2 * sinZ + y2 * cosZ;
        float zFinal = z2;

        // Apply rotational transformation
        vertices[i] = xFinal;
        vertices[i + 1] = yFinal;
        vertices[i + 2] = zFinal;
    }
}

void Model::drawcomp(const std::vector<PointLight>& pointLights) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_FLAT);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
    glBegin(GL_TRIANGLES);
    for(size_t i = 0; i < vertices.size(); i+=3) {
        Vector3 colorSum(0, 0, 0);
        if (i % 5 == frameCounter % 5) {
            Vector3 vertex = Vector3(vertices[i], vertices[i+1], vertices[i+2]);
            Vector3 colorSum(0, 0, 0);
            for (const auto& light : pointLights) {
                float distanceToLight = light.position.sqdist(vertex);
                float intensity = light.SQIntensity(distanceToLight);

                Vector3 colorContribution;
                colorContribution.x = light.color.x * intensity;
                colorContribution.z = light.color.z * intensity;
                colorContribution.y = light.color.y * intensity;

                colorSum.x += colorContribution.x;
                colorSum.y += colorContribution.y;
                colorSum.z += colorContribution.z;
                pcolorsum = colorSum;
               
                colorSum = colorSum.clamp(0.0f, 1.0f);
                glColor3f(colorSum.x, colorSum.y, colorSum.z);
                glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
            }
        } else {
            colorSum = pcolorsum;
            colorSum = colorSum.clamp(0.0f, 1.0f);
            glColor3f(colorSum.x, colorSum.y, colorSum.z);
            glVertex3f(vertices[i], vertices[i+1], vertices[i+2]);
        }
    }
    glEnd();
    glDisableClientState(GL_VERTEX_ARRAY);
    ++frameCounter;
}
