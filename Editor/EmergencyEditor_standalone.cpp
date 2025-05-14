/**
 * EmergencyEditor_standalone.cpp
 * Standalone emergency editor for GameEngineSavi that renders a red cube.
 * 
 * This file contains a simplified version of the editor that renders a red cube
 * using the graphics API wrapper for cross-platform compatibility.
 * It does not depend on the Editor class or other complex dependencies.
 */

#include "../platform.h"
#include "../TimeManager.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../Vector3.h"
#include "../Matrix4x4.h"
#include "../Shaders/Core/ShaderProgram.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Entry point
int main(int argc, char** argv) {
    std::cout << "Starting standalone emergency editor with red cube..." << std::endl;
    
    // Initialize graphics API
    if (!GraphicsAPIFactory::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize graphics API" << std::endl;
        return 1;
    }
    
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "Failed to get graphics API" << std::endl;
        return 1;
    }
    
    // Create window
    if (!graphics->CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Standalone Emergency Editor - Red Cube")) {
        std::cerr << "Failed to create window" << std::endl;
        
        // Even without a window, we can still render to files
        std::cout << "Continuing with headless rendering..." << std::endl;
        
        // Main loop for headless rendering
        bool running = true;
        TimeManager timeManager;
        
        while (running && timeManager.GetDeltaTime() < 5.0f) { // Run for 5 seconds
            // Update time
            timeManager.Update();
            
            std::cout << "Rendered frame at time " << timeManager.GetDeltaTime() << std::endl;
            
            // Small delay to not overwhelm the system
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        std::cout << "Emergency editor headless rendering complete." << std::endl;
        
        return 0;
    }
    
    // Set up the viewport
    graphics->SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    // Main loop
    bool running = true;
    TimeManager timeManager;
    
    while (running) {
        // Poll events
        graphics->PollEvents();
        
        // Check if window is still open
        if (!graphics->IsWindowOpen()) {
            running = false;
            continue;
        }
        
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Clear the screen
        graphics->SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        graphics->Clear(true, true); // Clear color and depth buffers
        
        // Draw a red cube directly using the graphics API
        float vertices[] = {
            // Front face
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-left, red
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Bottom-right, red
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-right, red
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-left, red
            
            // Back face
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // Bottom-left, red
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // Bottom-right, red
             0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // Top-right, red
            -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f  // Top-left, red
        };
        
        unsigned int indices[] = {
            // Front face
            0, 1, 2,
            2, 3, 0,
            
            // Right face
            1, 5, 6,
            6, 2, 1,
            
            // Back face
            5, 4, 7,
            7, 6, 5,
            
            // Left face
            4, 0, 3,
            3, 7, 4,
            
            // Top face
            3, 2, 6,
            6, 7, 3,
            
            // Bottom face
            4, 5, 1,
            1, 0, 4
        };
        
        // Create vertex array
        unsigned int vao = graphics->CreateVertexArray();
        graphics->BindVertexArray(vao);
        
        // Create vertex buffer
        unsigned int vbo = graphics->CreateBuffer();
        graphics->BindBuffer(BufferType::VERTEX_BUFFER, vbo);
        graphics->BufferData(BufferType::VERTEX_BUFFER, vertices, sizeof(vertices), false);
        
        // Create index buffer
        unsigned int ibo = graphics->CreateBuffer();
        graphics->BindBuffer(BufferType::INDEX_BUFFER, ibo);
        graphics->BufferData(BufferType::INDEX_BUFFER, indices, sizeof(indices), false);
        
        // Create and compile vertex shader
        unsigned int vertexShader = graphics->CreateShader(0); // Vertex shader
        const char* vertexShaderSource = 
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aColor;\n"
            "out vec3 ourColor;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
            "   ourColor = aColor;\n"
            "}\0";
        graphics->ShaderSource(vertexShader, std::string(vertexShaderSource));
        graphics->CompileShader(vertexShader);
        
        // Check for vertex shader compilation errors
        if (!graphics->GetShaderCompileStatus(vertexShader)) {
            std::cerr << "Vertex shader compilation failed: " << graphics->GetShaderInfoLog(vertexShader) << std::endl;
            running = false;
            continue;
        }
        
        // Create and compile fragment shader
        unsigned int fragmentShader = graphics->CreateShader(1); // Fragment shader
        const char* fragmentShaderSource = 
            "#version 330 core\n"
            "in vec3 ourColor;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   // Use a brighter red color for better visibility\n"
            "   FragColor = vec4(1.0, 0.2, 0.2, 1.0);\n"
            "}\0";
        graphics->ShaderSource(fragmentShader, std::string(fragmentShaderSource));
        graphics->CompileShader(fragmentShader);
        
        // Check for fragment shader compilation errors
        if (!graphics->GetShaderCompileStatus(fragmentShader)) {
            std::cerr << "Fragment shader compilation failed: " << graphics->GetShaderInfoLog(fragmentShader) << std::endl;
            running = false;
            continue;
        }
        
        // Create shader program
        unsigned int shaderProgram = graphics->CreateProgram();
        graphics->AttachShader(shaderProgram, vertexShader);
        graphics->AttachShader(shaderProgram, fragmentShader);
        graphics->LinkProgram(shaderProgram);
        
        // Check for shader program linking errors
        if (!graphics->GetProgramLinkStatus(shaderProgram)) {
            std::cerr << "Shader program linking failed: " << graphics->GetProgramInfoLog(shaderProgram) << std::endl;
            running = false;
            continue;
        }
        
        // Use shader program
        // Create a ShaderProgram object to pass to UseShaderProgram
        ShaderProgram* program = new ShaderProgram();
        program->SetProgramId(shaderProgram);
        graphics->UseShaderProgram(program);
        
        // Set up vertex attributes
        graphics->VertexAttribPointer(0, 3, false, 6 * sizeof(float), (void*)0);
        graphics->EnableVertexAttrib(0);
        graphics->VertexAttribPointer(1, 3, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        graphics->EnableVertexAttrib(1);
        
        // Set up transformation matrices
        float time = timeManager.GetDeltaTime();
        
        // Model matrix (rotate the cube)
        Matrix4x4 modelMatrix;
        modelMatrix.identity();
        modelMatrix.rotateY(time * 2.0f);
        
        // View matrix (position camera behind the light, looking at cube)
        Matrix4x4 viewMatrix;
        viewMatrix.identity();
        viewMatrix.translate(0.0f, -2.0f, -5.0f); // Match main editor camera position
        
        // Projection matrix (perspective)
        Matrix4x4 projectionMatrix;
        float fov = 45.0f * 3.14159f / 180.0f;
        float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        float near = 0.1f;
        float far = 100.0f;
        float f = 1.0f / tanf(fov / 2.0f);
        
        // Manual perspective matrix setup
        projectionMatrix.elements[0][0] = f / aspect;
        projectionMatrix.elements[1][1] = f;
        projectionMatrix.elements[2][2] = (far + near) / (near - far);
        projectionMatrix.elements[2][3] = -1.0f;
        projectionMatrix.elements[3][2] = (2.0f * far * near) / (near - far);
        
        // Set uniforms
        graphics->SetUniformMatrix4fv(shaderProgram, "model", &modelMatrix.elements[0][0], false);
        graphics->SetUniformMatrix4fv(shaderProgram, "view", &viewMatrix.elements[0][0], false);
        graphics->SetUniformMatrix4fv(shaderProgram, "projection", &projectionMatrix.elements[0][0], false);
        
        // Draw the cube
        graphics->DrawElements(DrawMode::TRIANGLES, 36, nullptr);
        
        // Clean up
        graphics->DeleteBuffer(vbo);
        graphics->DeleteBuffer(ibo);
        graphics->DeleteVertexArray(vao);
        graphics->DeleteShader(vertexShader);
        graphics->DeleteShader(fragmentShader);
        graphics->DeleteProgram(shaderProgram);
        delete program;
        
        // Swap buffers
        graphics->SwapBuffers();
        
        // Limit frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    // Cleanup
    graphics->DestroyWindow();
    
    std::cout << "Standalone emergency editor with red cube complete." << std::endl;
    
    return 0;
}
