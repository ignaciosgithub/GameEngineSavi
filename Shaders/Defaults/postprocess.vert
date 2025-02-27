#version 330 core

// Input vertex data
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// Output data to fragment shader
out vec2 TexCoord;

void main() {
    // Pass position directly to clip space
    gl_Position = vec4(aPos, 1.0);
    
    // Pass texture coordinates to fragment shader
    TexCoord = aTexCoord;
}
