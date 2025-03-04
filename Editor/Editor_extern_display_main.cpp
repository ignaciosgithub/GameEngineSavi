#include "Editor_extern_display.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../TimeManager.h"
#include "../ThirdParty/OpenGL/include/GL/gl_types.h"
#include "../FrameCapture_png.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <GL/glut.h>

// Window dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Global editor instance
std::unique_ptr<Editor_extern_display> editor;

// Frame capture settings
bool captureFrames = true;
int frameCount = 0;
const int MAX_FRAMES = 10;

// Function prototypes
void display();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle();
void captureFrame();

// Initialize editor
void initializeEditor() {
    // Create editor
    editor = std::unique_ptr<Editor_extern_display>(new Editor_extern_display(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
}

// Display callback
void display() {
    // Render the editor
    editor->Render();
    
    // Capture frame if enabled
    if (captureFrames && frameCount < MAX_FRAMES) {
        captureFrame();
    }
    
    // Swap buffers
    glutSwapBuffers();
}

// Reshape callback
void reshape(int width, int height) {
    // Resize editor
    editor->Resize(width, height);
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y) {
    // Exit on ESC
    if (key == 27) {
        exit(0);
    }
    
    // Handle WASD keys for camera movement
    Camera* camera = editor->GetCamera();
    if (camera) {
        Vector3 pos = camera->GetPosition();
        Vector3 dir = camera->lookDirection;
        Vector3 right = Vector3(0, 1, 0).cross(dir);
        right.normalize();
        
        float moveSpeed = 0.5f;
        
        // W - Move forward
        if (key == 'w' || key == 'W') {
            pos = pos + dir * moveSpeed;
        }
        // S - Move backward
        else if (key == 's' || key == 'S') {
            pos = pos - dir * moveSpeed;
        }
        // A - Move left
        else if (key == 'a' || key == 'A') {
            pos = pos - right * moveSpeed;
        }
        // D - Move right
        else if (key == 'd' || key == 'D') {
            pos = pos + right * moveSpeed;
        }
        
        camera->SetPosition(pos);
        std::cout << "Camera position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    }
}

// Idle callback
void idle() {
    // Update editor
    editor->Update(1.0f / 60.0f);
    
    // Redisplay
    glutPostRedisplay();
}

// Capture frame to PNG file
void captureFrame() {
    // Create filename
    std::string filename = "frames/frame" + std::to_string(frameCount) + ".png";
    
    // Capture viewport to file
    bool success = FrameCapture_PNG::CaptureViewportToFile(filename, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    if (success) {
        std::cout << "Frame " << frameCount << " captured to " << filename << std::endl;
    } else {
        std::cerr << "Failed to capture frame " << frameCount << std::endl;
    }
    
    // Increment frame count
    frameCount++;
    
    // Add a small delay to ensure file operations complete
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Main function
int main(int argc, char** argv) {
    std::cout << "Starting external display editor..." << std::endl;
    
    // Create frames directory if it doesn't exist
    system("mkdir -p frames");
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("GameEngineSavi External Display");
    
    // Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    
    // Set up OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Initialize editor
    initializeEditor();
    
    std::cout << "Starting main loop..." << std::endl;
    
    // Start main loop
    glutMainLoop();
    
    return 0;
}
