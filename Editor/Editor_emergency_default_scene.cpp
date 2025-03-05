/**
 * Editor_emergency_default_scene.cpp
 * Emergency renderer that loads a default scene and captures PNG screenshots
 * Uses GLUT for cross-platform rendering
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <GL/gl.h>
#include <GL/glu.h>
// Use system GLUT instead of local GLUT
#ifdef PLATFORM_WINDOWS
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif

#include "../Vector3.h"
#include "../Matrix4x4.h"
#include "../FrameCapture_png.h"
#include "../Model.h"
#include "../PointLight.h"

// Global variables
int windowWidth = 800;
int windowHeight = 600;
int frameCount = 0;
const int MAX_FRAMES = 10;
bool captureFrames = true;
float rotationAngle = 0.0f;

// Scene objects
Model* cube = nullptr;
std::vector<PointLight> lights;

// Initialize OpenGL
void initGL() {
    // Set clear color (dark blue)
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    // Set up light 0 with increased intensity
    GLfloat lightAmbient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat lightDiffuse[] = { 1.5f, 1.5f, 1.5f, 1.0f };  // Increased intensity
    GLfloat lightSpecular[] = { 1.5f, 1.5f, 1.5f, 1.0f }; // Increased intensity
    GLfloat lightPosition[] = { 0.0f, 2.0f, 5.0f, 1.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    // Set up material properties
    GLfloat matAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat matDiffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
    
    std::cout << "OpenGL initialized successfully" << std::endl;
}

// Initialize scene
void initScene() {
    // Create a cube model
    cube = new Model();
    
    // Set initial rotation to show 3D nature
    cube->Rotate(15.0f, 0.0f, 10.0f);
    
    // Create a point light
    PointLight light;
    light.position = Vector3(0.0f, 2.0f, 5.0f);
    light.color = Vector3(1.0f, 1.0f, 1.0f);
    light.intensity = 5.0f;  // Increased intensity for better visibility
    lights.push_back(light);
    
    std::cout << "Scene initialized successfully" << std::endl;
}

// Draw a colored cube using OpenGL
void drawCube() {
    // Draw cube using immediate mode
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
}

// Draw a point light
void drawPointLight(const PointLight& light) {
    glPushMatrix();
    glTranslatef(light.position.x, light.position.y, light.position.z);
    glDisable(GL_LIGHTING);
    glColor3f(light.color.x, light.color.y, light.color.z);
    glutSolidSphere(0.1f, 10, 10);
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// Render scene
void renderScene() {
    // Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
    
    // Set up modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Position camera
    gluLookAt(0.0f, 0.0f, 5.0f,  // Eye position
              0.0f, 0.0f, 0.0f,  // Look at position
              0.0f, 1.0f, 0.0f); // Up vector
    
    // Draw point lights
    for (const auto& light : lights) {
        drawPointLight(light);
    }
    
    // Draw cube with rotation
    glPushMatrix();
    
    // Apply rotation
    Vector3 rotation = cube->GetRotation();
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y + rotationAngle, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    
    // Draw cube
    drawCube();
    
    glPopMatrix();
    
    // Ensure all rendering commands are completed
    glFlush();
    glFinish();
    
    // Swap buffers
    glutSwapBuffers();
}

// Capture frame to PNG file
void captureFrame() {
    // Create filename
    std::string filename = "frames/frame" + std::to_string(frameCount) + ".png";
    
    // Capture viewport to file
    bool success = FrameCapture_PNG::CaptureViewportToFile(filename, windowWidth, windowHeight);
    
    if (success) {
        std::cout << "Frame " << frameCount << " captured to " << filename << std::endl;
    } else {
        std::cerr << "Failed to capture frame " << frameCount << std::endl;
    }
    
    // Increment frame count
    frameCount++;
    
    // Update rotation for next frame
    rotationAngle += 36.0f; // Rotate 36 degrees per frame (360 / 10 frames)
    
    // Check if we've captured all frames
    if (frameCount >= MAX_FRAMES) {
        std::cout << "All frames captured. Exiting..." << std::endl;
        
        // Clean up
        delete cube;
        
        exit(0);
    }
}

// Display callback
void display() {
    // Render the scene
    renderScene();
    
    // Capture frame if enabled
    if (captureFrames) {
        captureFrame();
        
        // Add a small delay to ensure file operations complete
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Request redisplay for next frame
    glutPostRedisplay();
}

// Reshape callback
void reshape(int width, int height) {
    // Update window dimensions
    windowWidth = width;
    windowHeight = height;
    
    // Set viewport
    glViewport(0, 0, width, height);
}

// Main function
int main(int argc, char** argv) {
    std::cout << "Starting emergency renderer with default scene..." << std::endl;
    
    // Create frames directory if it doesn't exist
    system("mkdir -p frames");
    
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Emergency Renderer - Default Scene");
    
    // Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    
    // Initialize OpenGL
    initGL();
    
    // Initialize scene
    initScene();
    
    std::cout << "Starting frame capture cycle..." << std::endl;
    
    // Start main loop
    glutMainLoop();
    
    return 0;
}
