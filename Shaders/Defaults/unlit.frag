#version 330 core

// Input data from vertex shader
in vec2 TexCoord;

// Output data
out vec4 FragColor;

// Material properties
uniform sampler2D albedoTexture;
uniform sampler2D opacityTexture;
uniform bool useOpacityMap = false;
uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
    // Sample albedo texture
    vec4 albedoColor = texture(albedoTexture, TexCoord) * color;
    
    // Apply opacity if using opacity map
    if (useOpacityMap) {
        float opacity = texture(opacityTexture, TexCoord).r;
        if (opacity < 0.1) {
            discard; // Discard fragment if opacity is too low
        }
        albedoColor.a = opacity;
    }
    
    // Output final color
    FragColor = albedoColor;
}
