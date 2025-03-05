#include <iostream>
#include <GL/glut.h>
#include "FrameCapture.h"
#include "Graphics/Core/GraphicsAPIFactory.h"

// Simple OpenGL rendering function
void renderScene() {
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    
    // Clear the screen
    if (graphics) {
        graphics->Clear(true, true);
        
        // Set background color to blue
        graphics->SetClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    } else {
        // Fallback to direct OpenGL calls if graphics API is not available
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    }
    
    // Draw a white triangle
    // Note: The graphics API doesn't have direct methods for drawing primitives yet
    // so we'll use the direct OpenGL calls for now
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glVertex3f(0.5f, -0.5f, 0.0f);
        glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();
    
    // Draw a red cube
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Swap buffers
    glutSwapBuffers();
}

// Capture screenshot when 's' key is pressed
void keyboard(unsigned char key, int x, int y) {
    if (key == 's' || key == 'S') {
        std::cout << "Capturing screenshot..." << std::endl;
        
        // Create screenshots directory if it doesn't exist
        system("mkdir -p screenshots/editor");
        
        if (FrameCapture::CaptureViewportToFile("screenshots/editor/test_capture.png")) {
            std::cout << "Screenshot saved to screenshots/editor/test_capture.png" << std::endl;
        } else {
            std::cout << "Failed to capture screenshot" << std::endl;
        }
    } else if (key == 'q' || key == 'Q' || key == 27) { // 27 is ESC key
        // Clean up graphics API before exiting
        auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
        if (graphics) {
            graphics->Shutdown();
        }
        exit(0);
    }
}

int main(int argc, char** argv) {
    std::cout << "=== Frame Capture Test ===" << std::endl;
    std::cout << "Press 's' to capture a screenshot" << std::endl;
    std::cout << "Press 'q' or ESC to quit" << std::endl;
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Frame Capture Test");
    
    // Initialize graphics API
    GraphicsAPIFactory::GetInstance().Initialize();
    
    // Register callbacks
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(keyboard);
    
    // Enter GLUT event processing cycle
    glutMainLoop();
    
    return 0;
}
