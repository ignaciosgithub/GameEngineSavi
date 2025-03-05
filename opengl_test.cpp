// Use system GLUT instead of local GLUT
#ifdef PLATFORM_WINDOWS
#include <GL/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
        glVertex2f(-0.5, -0.5);
        glVertex2f(0.5, -0.5);
        glVertex2f(0.5, 0.5);
        glVertex2f(-0.5, 0.5);
    glEnd();
    glutSwapBuffers();
    
    // Print pixel color at center to verify rendering
    float pixel[3];
    glReadPixels(glutGet(GLUT_WINDOW_WIDTH)/2, glutGet(GLUT_WINDOW_HEIGHT)/2, 
                1, 1, GL_RGB, GL_FLOAT, pixel);
    std::cout << "Center pixel color: " << pixel[0] << ", " 
              << pixel[1] << ", " << pixel[2] << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "Starting OpenGL test program..." << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Test");
    
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
