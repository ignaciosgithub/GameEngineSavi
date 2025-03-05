// Define GLEW_STATIC before including glew.h
#define GLEW_STATIC

// Include GLEW first
#include <GL/glew.h>

#include <iostream>
#include <string>
#include <vector>
#include <GL/gl.h>
#include <GL/freeglut.h>

#include "EditorMain.h"
#include "../Scene.h"
#include "../GameObject.h"
#include "../Debugger.h"

int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Game Engine Editor");
    
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Error initializing GLEW: " << glewGetErrorString(err) << std::endl;
        return 1;
    }
    std::cout << "GLEW initialized successfully" << std::endl;
    
    // Initialize editor
    InitializeEditor();
    
    // Set up callbacks
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(KeyboardInput);
    glutMouseFunc(MouseInput);
    glutMotionFunc(MouseMotion);
    
    // Enter main loop
    glutMainLoop();
    
    return 0;
}

void InitializeEditor() {
    // Initialize the editor
    std::cout << "Initializing editor..." << std::endl;
    
    // Set up OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Create a default scene
    Scene* scene = new Scene();
    scene->CreateDefaultObjects();
    
    std::cout << "Editor initialized successfully" << std::endl;
}

void RenderScene() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up camera
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 5.0f, 10.0f,  // Eye position
              0.0f, 0.0f, 0.0f,   // Look at position
              0.0f, 1.0f, 0.0f);  // Up vector
    
    // Swap buffers
    glutSwapBuffers();
}

void Resize(int width, int height) {
    // Adjust viewport
    glViewport(0, 0, width, height);
    
    // Update projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
    
    // Switch back to modelview matrix
    glMatrixMode(GL_MODELVIEW);
}

void KeyboardInput(unsigned char key, int x, int y) {
    // Handle keyboard input
    switch (key) {
        case 27: // ESC key
            exit(0);
            break;
    }
    
    // Redraw the scene
    glutPostRedisplay();
}

void MouseInput(int button, int state, int x, int y) {
    // Handle mouse input
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Left mouse button pressed
        } else {
            // Left mouse button released
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            // Right mouse button pressed
        } else {
            // Right mouse button released
        }
    }
    
    // Redraw the scene
    glutPostRedisplay();
}

void MouseMotion(int x, int y) {
    // Handle mouse motion
    
    // Redraw the scene
    glutPostRedisplay();
}
