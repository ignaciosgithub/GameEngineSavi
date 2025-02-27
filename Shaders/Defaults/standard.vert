#version 330 core

// Input vertex data
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

// Uniforms for transformation matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output data to fragment shader
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
    // Calculate position in clip space
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // Pass texture coordinates to fragment shader
    TexCoord = aTexCoord;
    
    // Calculate normal in world space
    // Note: This is a simplified approach, for more accuracy we should use the normal matrix
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Calculate fragment position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
}
