#version 330 core

// Input vertex data
layout (location = 0) in vec3 aPos;

// Uniforms for transformation matrices
uniform mat4 view;
uniform mat4 projection;

// Output data to fragment shader
out vec3 TexCoords;

void main() {
    // Remove translation from view matrix
    mat4 viewNoTranslation = mat4(mat3(view));
    
    // Calculate position in clip space
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    
    // Set z to w to ensure skybox is always at the far plane
    gl_Position = pos.xyww;
    
    // Use position as texture coordinates
    TexCoords = aPos;
}
