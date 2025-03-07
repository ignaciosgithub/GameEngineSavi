#version 330 core

// Input data from vertex shader
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

// Output data
out vec4 FragColor;

// Material properties
uniform sampler2D albedoTexture;
uniform sampler2D normalTexture;
uniform sampler2D opacityTexture;
uniform bool useNormalMap = false;
uniform bool useOpacityMap = false;
uniform float shininess = 32.0;

// Light properties
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

#define MAX_POINT_LIGHTS 8
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights = 0;

#define MAX_DIRECTIONAL_LIGHTS 4
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int numDirectionalLights = 0;

// Camera position
uniform vec3 viewPos;

// Calculate lighting for a point light
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo) {
    // Calculate light direction
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Calculate distance to light
    float distance = length(light.position - fragPos);
    
    // Calculate attenuation
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
    
    // Calculate diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;
    
    // Calculate specular lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * light.color * light.intensity;
    
    // Apply attenuation
    diffuse *= attenuation;
    specular *= attenuation;
    
    return diffuse + specular;
}

// Calculate lighting for a directional light
vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 albedo) {
    // Calculate light direction (opposite of the light's direction)
    vec3 lightDir = normalize(-light.direction);
    
    // Calculate diffuse lighting
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.intensity;
    
    // Calculate specular lighting
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * light.color * light.intensity;
    
    return diffuse + specular;
}

void main() {
    // Sample albedo texture
    vec4 albedoColor = texture(albedoTexture, TexCoord);
    
    // Apply opacity if using opacity map
    if (useOpacityMap) {
        float opacity = texture(opacityTexture, TexCoord).r;
        if (opacity < 0.1) {
            discard; // Discard fragment if opacity is too low
        }
        albedoColor.a = opacity;
    }
    
    // Calculate normal
    vec3 normal;
    if (useNormalMap) {
        // Sample normal map and convert from [0,1] to [-1,1] range
        normal = texture(normalTexture, TexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    } else {
        normal = normalize(Normal);
    }
    
    // Calculate view direction
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // Calculate lighting
    vec3 lighting = vec3(0.1); // Ambient light
    
    // Add contribution from each directional light
    for (int i = 0; i < numDirectionalLights && i < MAX_DIRECTIONAL_LIGHTS; i++) {
        lighting += calculateDirectionalLight(directionalLights[i], normal, viewDir, albedoColor.rgb);
    }
    
    // Add contribution from each point light
    for (int i = 0; i < numPointLights && i < MAX_POINT_LIGHTS; i++) {
        lighting += calculatePointLight(pointLights[i], normal, FragPos, viewDir, albedoColor.rgb);
    }
    
    // Apply lighting to albedo
    vec3 result = lighting * albedoColor.rgb;
    
    // Output final color
    FragColor = vec4(result, albedoColor.a);
}
