/**
 * test_rotate.cpp
 * Simple test program to verify the Model::Rotate method
 * Generates text files with rotation data for visualization
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

// Define GL_GLEXT_PROTOTYPES to ensure function prototypes are declared
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

// Platform-specific OpenGL includes
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#else
#include <GL/gl.h>
#endif

// Include OpenGL extension headers
#include "ThirdParty/OpenGL/include/GL/glext.h"

// Include GLUT for rendering
// Use system GLUT instead of local GLUT
#ifdef PLATFORM_WINDOWS
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Model.h"
#include "Vector3.h"
#include "Matrix4x4.h"

// Global variables
int frameCount = 0;
const int MAX_FRAMES = 10;
Model* globalCube = nullptr;

// Function prototypes
void display();
void reshape(int width, int height);
void saveFrameData(int frameNum, const Vector3& rotation);

// Simple test program to verify the Rotate method with OpenGL rendering
int main(int argc, char** argv) {
    std::cout << "Starting Rotate method test with OpenGL rendering..." << std::endl;
    
    // Create frames directory if it doesn't exist
    system("mkdir -p frames");
    
    // Create a model
    Model cube;
    globalCube = &cube;
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Model Rotation Test");
    
    // Initialize OpenGL
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up light
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPosition[] = { 0.0f, 0.0f, 5.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // Create a cube model
    cube.CreateCube();
    
    // Set up display and reshape callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // Start GLUT main loop
    glutMainLoop();
    
    return 0;
}

// Display callback
void display() {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
    
    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 5.0f,  // Eye position
              0.0f, 0.0f, 0.0f,  // Look at position
              0.0f, 1.0f, 0.0f); // Up vector
    
    // Set rotation angles for current frame
    float angleX = 15.0f * std::sin(frameCount * 0.2f);
    float angleY = 36.0f * frameCount; // 36 degrees per frame (360 / 10 frames)
    float angleZ = 10.0f * std::cos(frameCount * 0.2f);
    
    // Use the Rotate method
    globalCube->Rotate(angleX, angleY, angleZ);
    
    // Get the rotation values from the model
    Vector3 rotation = globalCube->GetRotation();
    
    // Draw the cube
    glPushMatrix();
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    
    // Draw a colored cube
    glBegin(GL_QUADS);
    
    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    
    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    
    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    
    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    
    // Right face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    
    // Left face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    
    glEnd();
    glPopMatrix();
    
    // Swap buffers
    glutSwapBuffers();
    
    // Save frame data
    saveFrameData(frameCount, rotation);
    
    // Increment frame count
    frameCount++;
    
    // Exit if we've generated all frames
    if (frameCount >= MAX_FRAMES) {
        std::cout << "All frames generated. Exiting..." << std::endl;
        exit(0);
    } else {
        // Request redisplay for next frame
        glutPostRedisplay();
    }
}

// Reshape callback
void reshape(int width, int height) {
    // Set viewport
    glViewport(0, 0, width, height);
}

// Save frame data to a text file
void saveFrameData(int frameNum, const Vector3& rotation) {
    // Output rotation data to a text file
    std::string filename = "frames/frame" + std::to_string(frameNum) + ".txt";
    std::ofstream file(filename);
    
    if (file.is_open()) {
        // Write frame info
        file << "Frame " << frameNum << " - Rotation: ";
        file << "X=" << rotation.x << ", Y=" << rotation.y << ", Z=" << rotation.z << std::endl;
        
        // Get rotation matrix
        Matrix4x4 rotationMatrix = Matrix4x4::createRotation(rotation.x, rotation.y, rotation.z);
        
        // Write rotation matrix
        file << "Rotation Matrix:" << std::endl;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                file << rotationMatrix.m[row][col] << " ";
            }
            file << std::endl;
        }
        
        file.close();
        std::cout << "Frame " << frameNum << " data written to: " << filename << std::endl;
    } else {
        std::cerr << "Failed to open file: " << filename << std::endl;
    }
}
