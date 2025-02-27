#include "../Shaders/Core/Shader.h"
#include "../Shaders/Core/ShaderProgram.h"
#include "../Shaders/Core/ShaderError.h"
#include <iostream>
#include <string>

using namespace Shaders;

// Test vertex shader source
const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}
)";

// Test fragment shader source
const std::string fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D albedoTexture;
uniform bool hasAlbedoTexture;
uniform vec3 viewPos;
uniform int lightCount;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};

uniform PointLight lights[10];

out vec4 FragColor;

void main() {
    // Base color
    vec4 baseColor = hasAlbedoTexture ? texture(albedoTexture, TexCoord) : vec4(1.0, 1.0, 1.0, 1.0);
    
    // Ambient light
    vec3 ambient = vec3(0.1, 0.1, 0.1) * baseColor.rgb;
    
    // Diffuse and specular light
    vec3 result = ambient;
    
    for (int i = 0; i < lightCount; i++) {
        // Light direction
        vec3 lightDir = normalize(lights[i].position - FragPos);
        
        // Diffuse
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity * baseColor.rgb;
        
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * lights[i].color * lights[i].intensity;
        
        // Attenuation
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        
        // Apply attenuation
        diffuse *= attenuation;
        specular *= attenuation;
        
        // Add to result
        result += diffuse + specular;
    }
    
    // Final color
    FragColor = vec4(result, baseColor.a);
}
)";

// Test geometry shader source
const std::string geometryShaderSource = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];

out vec2 GTexCoord;
out vec3 GNormal;
out vec3 GFragPos;

void main() {
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        GTexCoord = TexCoord[i];
        GNormal = Normal[i];
        GFragPos = FragPos[i];
        EmitVertex();
    }
    EndPrimitive();
}
)";

// Test shader compilation
bool TestShaderCompilation() {
    bool success = true;
    
    // Test vertex shader
    Shader vertexShader(Shader::VERTEX);
    if (!vertexShader.LoadFromString(vertexShaderSource)) {
        std::cerr << "Failed to load vertex shader source: " << vertexShader.GetError() << std::endl;
        success = false;
    }
    
    if (!vertexShader.Compile()) {
        std::cerr << "Failed to compile vertex shader: " << vertexShader.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Vertex shader compiled successfully" << std::endl;
    }
    
    // Test fragment shader
    Shader fragmentShader(Shader::FRAGMENT);
    if (!fragmentShader.LoadFromString(fragmentShaderSource)) {
        std::cerr << "Failed to load fragment shader source: " << fragmentShader.GetError() << std::endl;
        success = false;
    }
    
    if (!fragmentShader.Compile()) {
        std::cerr << "Failed to compile fragment shader: " << fragmentShader.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Fragment shader compiled successfully" << std::endl;
    }
    
    // Test geometry shader
    Shader geometryShader(Shader::GEOMETRY);
    if (!geometryShader.LoadFromString(geometryShaderSource)) {
        std::cerr << "Failed to load geometry shader source: " << geometryShader.GetError() << std::endl;
        success = false;
    }
    
    if (!geometryShader.Compile()) {
        std::cerr << "Failed to compile geometry shader: " << geometryShader.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Geometry shader compiled successfully" << std::endl;
    }
    
    // Test shader program
    ShaderProgram program;
    program.AttachShader(&vertexShader);
    program.AttachShader(&fragmentShader);
    program.AttachShader(&geometryShader);
    
    if (!program.Link()) {
        std::cerr << "Failed to link shader program: " << program.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Shader program linked successfully" << std::endl;
    }
    
    return success;
}

int main() {
    std::cout << "Testing shader compilation..." << std::endl;
    
    if (TestShaderCompilation()) {
        std::cout << "All shader compilation tests passed" << std::endl;
        return 0;
    } else {
        std::cerr << "Shader compilation tests failed" << std::endl;
        return 1;
    }
}
