#version 330 core

// Input data from vertex shader
in vec2 TexCoord;

// Output data
out vec4 FragColor;

// Screen texture
uniform sampler2D screenTexture;

// Effect parameters
uniform bool useGrayscale = false;
uniform bool useInversion = false;
uniform bool useBlur = false;
uniform float gamma = 2.2;

void main() {
    // Sample screen texture
    vec4 color = texture(screenTexture, TexCoord);
    
    // Apply effects
    if (useGrayscale) {
        float average = (color.r + color.g + color.b) / 3.0;
        color = vec4(average, average, average, color.a);
    }
    
    if (useInversion) {
        color = vec4(1.0 - color.rgb, color.a);
    }
    
    if (useBlur) {
        // Simple box blur
        vec2 texelSize = 1.0 / textureSize(screenTexture, 0);
        vec4 result = vec4(0.0);
        
        for (int x = -2; x <= 2; x++) {
            for (int y = -2; y <= 2; y++) {
                vec2 offset = vec2(float(x), float(y)) * texelSize;
                result += texture(screenTexture, TexCoord + offset);
            }
        }
        
        color = result / 25.0;
    }
    
    // Apply gamma correction
    color.rgb = pow(color.rgb, vec3(1.0 / gamma));
    
    // Output final color
    FragColor = color;
}
