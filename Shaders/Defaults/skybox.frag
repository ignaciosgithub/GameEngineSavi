#version 330 core

// Input data from vertex shader
in vec3 TexCoords;

// Output data
out vec4 FragColor;

// Skybox texture
uniform samplerCube skybox;

void main() {
    // Sample skybox texture
    FragColor = texture(skybox, TexCoords);
}
