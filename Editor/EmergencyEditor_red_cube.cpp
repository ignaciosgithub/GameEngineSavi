/**
 * EmergencyEditor_red_cube.cpp
 * Emergency editor for GameEngineSavi that renders a red cube.
 * 
 * This file contains a simplified version of the editor that renders a red cube
 * using the graphics API wrapper for cross-platform compatibility.
 */

#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../TimeManager.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../Camera.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Global editor instance
std::unique_ptr<Editor> editor;

// Common initialization function
void InitializeEditor() {
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Set engine condition to editor mode
    EngineCondition::isInEditor = true;
}

// Entry point
int main(int argc, char** argv) {
    std::cout << "Starting emergency editor with red cube..." << std::endl;
    
    // Initialize editor
    InitializeEditor();
    
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
    if (!graphics->CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Emergency Editor - Red Cube")) {
        std::cerr << "Failed to create window" << std::endl;
        
        // Even without a window, we can still render to files
        std::cout << "Continuing with headless rendering..." << std::endl;
        
        // Main loop for headless rendering
        bool running = true;
        TimeManager timeManager;
        
        while (running && timeManager.GetTotalTime() < 5.0f) { // Run for 5 seconds
            // Update time
            timeManager.Update();
            float deltaTime = timeManager.GetDeltaTime();
            
            // Update editor
            if (editor) {
                editor->Update(deltaTime);
                editor->Render();
                
                std::cout << "Rendered frame at time " << timeManager.GetTotalTime() << std::endl;
            }
            
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
        graphics->ClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        graphics->Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Update editor
        if (editor) {
            editor->Update(deltaTime);
            editor->Render();
        }
        
        // Draw a red cube directly using the graphics API
        // This is a fallback in case the editor's rendering fails
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
        
        // Create vertex buffer
        unsigned int vbo = graphics->CreateBuffer();
        graphics->BindBuffer(GL_ARRAY_BUFFER, vbo);
        graphics->BufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        // Create index buffer
        unsigned int ibo = graphics->CreateBuffer();
        graphics->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        graphics->BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // Create and compile vertex shader
        unsigned int vertexShader = graphics->CreateShader(GL_VERTEX_SHADER);
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
        graphics->ShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        graphics->CompileShader(vertexShader);
        
        // Create and compile fragment shader
        unsigned int fragmentShader = graphics->CreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentShaderSource = 
            "#version 330 core\n"
            "in vec3 ourColor;\n"
            "out vec4 FragColor;\n"
            "void main()\n"
            "{\n"
            "   FragColor = vec4(ourColor, 1.0);\n"
            "}\0";
        graphics->ShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        graphics->CompileShader(fragmentShader);
        
        // Create shader program
        unsigned int shaderProgram = graphics->CreateProgram();
        graphics->AttachShader(shaderProgram, vertexShader);
        graphics->AttachShader(shaderProgram, fragmentShader);
        graphics->LinkProgram(shaderProgram);
        
        // Use shader program
        graphics->UseShaderProgram(shaderProgram);
        
        // Set up vertex attributes
        graphics->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        graphics->EnableVertexAttribArray(0);
        graphics->VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        graphics->EnableVertexAttribArray(1);
        
        // Set up transformation matrices
        float time = timeManager.GetTotalTime();
        
        // Model matrix (rotate the cube)
        float model[16] = {
            cosf(time), 0.0f, sinf(time), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sinf(time), 0.0f, cosf(time), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        // View matrix (move the camera back a bit)
        float view[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, -3.0f, 1.0f
        };
        
        // Projection matrix (perspective)
        float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        float fov = 45.0f * 3.14159f / 180.0f;
        float near = 0.1f;
        float far = 100.0f;
        float f = 1.0f / tanf(fov / 2.0f);
        float projection[16] = {
            f / aspect, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f, (far + near) / (near - far), -1.0f,
            0.0f, 0.0f, (2.0f * far * near) / (near - far), 0.0f
        };
        
        // Set uniforms
        graphics->SetUniformMatrix4fv(shaderProgram, "model", model, false);
        graphics->SetUniformMatrix4fv(shaderProgram, "view", view, false);
        graphics->SetUniformMatrix4fv(shaderProgram, "projection", projection, false);
        
        // Draw the cube
        graphics->DrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        // Clean up
        graphics->DeleteBuffer(vbo);
        graphics->DeleteBuffer(ibo);
        graphics->DeleteShader(vertexShader);
        graphics->DeleteShader(fragmentShader);
        graphics->DeleteProgram(shaderProgram);
        
        // Swap buffers
        graphics->SwapBuffers();
        
        // Limit frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    // Cleanup
    graphics->DestroyWindow();
    
    std::cout << "Emergency editor with red cube complete." << std::endl;
    
    return 0;
}
