#version 330 core

// Input and output primitive types
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

// Input from vertex shader
in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];

// Output to fragment shader
out vec2 TexCoord_out;
out vec3 Normal_out;
out vec3 FragPos_out;

void main() {
    // For each vertex in the input triangle
    for (int i = 0; i < 3; i++) {
        // Pass through the vertex data
        gl_Position = gl_in[i].gl_Position;
        TexCoord_out = TexCoord[i];
        Normal_out = Normal[i];
        FragPos_out = FragPos[i];
        
        // Emit the vertex
        EmitVertex();
    }
    
    // End the primitive
    EndPrimitive();
}
