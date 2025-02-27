#version 330 core

// Input vertex data
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// Uniforms for transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output data to fragment shader
out vec2 TexCoord;

void main() {
    // Calculate position in clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Pass texture coordinates to fragment shader
    TexCoord = aTexCoord;
}
