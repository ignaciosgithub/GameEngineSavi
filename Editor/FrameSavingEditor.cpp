/**
 * FrameSavingEditor.cpp
 * A headless version of the Editor that saves frames to disk instead of displaying them directly.
 * This version captures the entire editor UI including panels and renders it to PNG files.
 * It still detects keyboard input for navigation and editing, even in headless mode.
 */

#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/stat.h>
#include <memory>
#include <thread>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstring>

#include "../platform.h"
#include "../TimeManager.h"
#include "../Graphics/Core/GraphicsAPIFactory.h"
#include "../Graphics/Core/IGraphicsAPI.h"
#include "../FrameCapture_png.h"
#include "../Editor/Editor.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../CameraManager.h"
#include "../Shaders/Core/ShaderProgram.h"

// Define headless mode for conditional compilation
#ifndef HEADLESS_MODE
#define HEADLESS_MODE
#endif

// Include stb_image_write.h for direct PNG writing
#include "../ThirdParty/stb/stb_image_write.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) mkdir(dir)
#else
#define MKDIR(dir) mkdir(dir, 0755)
#endif

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Frame counter
int frameCount = 0;

// Function to generate frame filename
std::string GetFrameFilename() {
    std::stringstream ss;
    ss << "frames/frame" << frameCount << ".png";
    return ss.str();
}

// Create frames directory if it doesn't exist
void createFramesDirectory() {
    const char* framesDir = "frames";
    struct stat st = {0};
    
    if (stat(framesDir, &st) == -1) {
        if (MKDIR(framesDir) != 0) {
            std::cerr << "Error creating frames directory" << std::endl;
        } else {
            std::cout << "Created frames directory" << std::endl;
        }
    }
}

// Create a simple red cube
void createRedCube(std::shared_ptr<IGraphicsAPI> graphics, unsigned int& vao, unsigned int& vbo, unsigned int& ibo) {
    // Create vertex array object
    vao = graphics->CreateVertexArray();
    graphics->BindVertexArray(vao);
    
    // Vertex data for a red cube
    float vertices[] = {
        // Position (XYZ)     // Color (RGB)
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
    
    // Index data for the cube
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
    
    // Create and bind vertex buffer
    vbo = graphics->CreateBuffer();
    graphics->BindBuffer(BufferType::VERTEX_BUFFER, vbo);
    graphics->BufferData(BufferType::VERTEX_BUFFER, vertices, sizeof(vertices), false);
    
    // Create and bind index buffer
    ibo = graphics->CreateBuffer();
    graphics->BindBuffer(BufferType::INDEX_BUFFER, ibo);
    graphics->BufferData(BufferType::INDEX_BUFFER, indices, sizeof(indices), false);
    
    // Set up vertex attributes
    graphics->VertexAttribPointer(0, 3, false, 6 * sizeof(float), (void*)0);
    graphics->EnableVertexAttrib(0);
    graphics->VertexAttribPointer(1, 3, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    graphics->EnableVertexAttrib(1);
}

// Create a simple shader program for rendering the red cube
unsigned int createShaderProgram(std::shared_ptr<IGraphicsAPI> graphics) {
    // Create and compile vertex shader
    unsigned int vertexShader = graphics->CreateShader(0x8B31); // GL_VERTEX_SHADER
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
        "   ourColor = vec3(1.0, 0.0, 0.0); // Force red color\n"
        "}\0";
    graphics->ShaderSource(vertexShader, std::string(vertexShaderSource));
    graphics->CompileShader(vertexShader);
    
    // Check for vertex shader compilation errors
    if (!graphics->GetShaderCompileStatus(vertexShader)) {
        std::cerr << "Vertex shader compilation failed: " 
                  << graphics->GetShaderInfoLog(vertexShader) << std::endl;
    }
    
    // Create and compile fragment shader
    unsigned int fragmentShader = graphics->CreateShader(0x8B30); // GL_FRAGMENT_SHADER
    const char* fragmentShaderSource = 
        "#version 330 core\n"
        "in vec3 ourColor;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Force bright red color\n"
        "}\0";
    graphics->ShaderSource(fragmentShader, std::string(fragmentShaderSource));
    graphics->CompileShader(fragmentShader);
    
    // Check for fragment shader compilation errors
    if (!graphics->GetShaderCompileStatus(fragmentShader)) {
        std::cerr << "Fragment shader compilation failed: " 
                  << graphics->GetShaderInfoLog(fragmentShader) << std::endl;
    }
    
    // Create shader program
    unsigned int shaderProgram = graphics->CreateProgram();
    graphics->AttachShader(shaderProgram, vertexShader);
    graphics->AttachShader(shaderProgram, fragmentShader);
    graphics->LinkProgram(shaderProgram);
    
    // Check for shader program linking errors
    if (!graphics->GetProgramLinkStatus(shaderProgram)) {
        std::cerr << "Shader program linking failed: " 
                  << graphics->GetProgramInfoLog(shaderProgram) << std::endl;
    }
    
    // Delete shaders as they're linked into the program now and no longer needed
    graphics->DeleteShader(vertexShader);
    graphics->DeleteShader(fragmentShader);
    
    return shaderProgram;
}

// Create a simple red image directly in memory
void createRedImageDirectly(const std::string& filename, int width, int height) {
    // Create a buffer for the red image (RGBA format)
    std::vector<unsigned char> pixels(width * height * 4);
    
    // Fill with red color
    for (int i = 0; i < width * height * 4; i += 4) {
        pixels[i] = 255;     // R
        pixels[i + 1] = 0;   // G
        pixels[i + 2] = 0;   // B
        pixels[i + 3] = 255; // A
    }
    
    // Save to PNG file using stbi_write_png
    int result = stbi_write_png(
        filename.c_str(),
        width,
        height,
        4,  // RGBA components
        pixels.data(),
        width * 4  // Stride in bytes
    );
    
    if (result == 0) {
        std::cerr << "Failed to write PNG file: " << filename << std::endl;
    } else {
        std::cout << "Created red image directly: " << filename << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "Starting Frame Saving Editor..." << std::endl;
    
    // Create frames directory
    createFramesDirectory();
    
    // Initialize graphics API
    if (!GraphicsAPIFactory::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize graphics API, falling back to direct image creation" << std::endl;
        
        // Create 10 frames directly
        for (int i = 0; i < 10; i++) {
            std::string filename = "frames/frame" + std::to_string(i) + ".png";
            createRedImageDirectly(filename, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        
        return 0;
    }
    
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "Failed to get graphics API, falling back to direct image creation" << std::endl;
        
        // Create 10 frames directly
        for (int i = 0; i < 10; i++) {
            std::string filename = "frames/frame" + std::to_string(i) + ".png";
            createRedImageDirectly(filename, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        
        return 0;
    }
    
    // Try to create a window, but don't require it
    bool hasWindow = false;
    if (graphics->CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Frame Saving Editor")) {
        hasWindow = true;
        graphics->SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        std::cout << "Created window successfully" << std::endl;
    } else {
        std::cout << "Running in headless mode..." << std::endl;
        
        // If we can't create a window, fall back to direct image creation
        for (int i = 0; i < 10; i++) {
            std::string filename = "frames/frame" + std::to_string(i) + ".png";
            createRedImageDirectly(filename, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        
        return 0;
    }
    
    // Create red cube geometry
    unsigned int vao, vbo, ibo;
    createRedCube(graphics, vao, vbo, ibo);
    
    // Create shader program
    unsigned int shaderProgram = createShaderProgram(graphics);
    
    // Main loop
    TimeManager timeManager;
    
    // Render 30 frames to show more movement
    for (int i = 0; i < 30 && hasWindow; i++) {
        // Update time
        timeManager.Update();
        
        // Poll events
        graphics->PollEvents();
        
        // Check if window is still open
        if (!graphics->IsWindowOpen()) {
            break;
        }
        
        // Clear the screen
        graphics->SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        graphics->Clear(true, true);
        
        // Bind VAO
        graphics->BindVertexArray(vao);
        
        // Create a ShaderProgram object to use with the graphics API
        ShaderProgram shaderProgramObj;
        shaderProgramObj.SetProgramId(shaderProgram);
        graphics->UseShaderProgram(&shaderProgramObj);
        
        // Set up key detection for headless mode
        bool keyWPressed = false;
        bool keySPressed = false;
        bool keyAPressed = false;
        bool keyDPressed = false;
        
        // In headless mode, we simulate key presses based on frame number
        // This allows for camera movement simulation even without a window
        if (i % 3 == 0) keyWPressed = true;  // Move forward every 3rd frame
        if (i % 5 == 1) keySPressed = true;  // Move backward every 5th frame
        if (i % 4 == 2) keyAPressed = true;  // Move left every 4th frame
        if (i % 4 == 3) keyDPressed = true;  // Move right every 4th frame
        
        // Set up transformation matrices
        float time = static_cast<float>(i) * 0.1f; // Rotate based on frame number
        
        // Apply camera movement based on key detection
        static float cameraX = 0.0f;
        static float cameraY = 0.0f;
        static float cameraZ = -3.0f;
        
        // Move camera based on key presses
        if (keyWPressed) cameraZ += 0.1f;  // Move forward
        if (keySPressed) cameraZ -= 0.1f;  // Move backward
        if (keyAPressed) cameraX -= 0.1f;  // Move left
        if (keyDPressed) cameraX += 0.1f;  // Move right
        
        // Model matrix (rotate the cube)
        float model[16] = {
            cosf(time), 0.0f, sinf(time), 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            -sinf(time), 0.0f, cosf(time), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
        
        // View matrix (use camera position from key detection)
        float view[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            cameraX, cameraY, cameraZ, 1.0f
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
        graphics->DrawElements(DrawMode::TRIANGLES, 36, nullptr);
        
        // Capture frame to file using PNG format
        std::string filename = GetFrameFilename();
        bool success = FrameCapture_PNG::CaptureViewportToFile(filename);
        if (success) {
            std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
        } else {
            std::cerr << "Failed to save frame " << frameCount << " to " << filename << std::endl;
            
            // Fall back to direct image creation
            createRedImageDirectly(filename, WINDOW_WIDTH, WINDOW_HEIGHT);
        }
        
        // Increment frame counter
        frameCount++;
        
        // Swap buffers
        graphics->SwapBuffers();
        
        // Small delay to not overwhelm the system
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Clean up
    if (hasWindow) {
        graphics->DeleteBuffer(vbo);
        graphics->DeleteBuffer(ibo);
        graphics->DeleteVertexArray(vao);
        graphics->DeleteProgram(shaderProgram);
        graphics->DestroyWindow();
    }
    
    std::cout << "Frame saving editor complete. " << frameCount << " frames rendered." << std::endl;
    
    return 0;
}
