#include <iostream>
// Use system GLUT instead of local GLUT
#ifdef PLATFORM_WINDOWS
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif
#include "FrameCapture.h"

// Simple OpenGL rendering function
void renderScene() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set background color to blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    
    // Draw a white triangle
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
        if (FrameCapture::CaptureViewportToFile("screenshots/editor/test_capture.png")) {
            std::cout << "Screenshot saved to screenshots/editor/test_capture.png" << std::endl;
        } else {
            std::cerr << "Failed to capture screenshot" << std::endl;
        }
    } else if (key == 'q' || key == 'Q' || key == 27) { // 27 is ESC key
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
    
    // Register callbacks
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(keyboard);
    
    // Enter GLUT event processing cycle
    glutMainLoop();
    
    return 0;
}
