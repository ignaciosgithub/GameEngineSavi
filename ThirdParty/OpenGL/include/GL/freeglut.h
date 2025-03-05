#ifndef __freeglut_h__
#define __freeglut_h__

/* freeglut compatibility header */
#include <GL/gl.h>
#include <GL/glu.h>

/* GLUT API macro definitions */
#define GLUT_RGB            0
#define GLUT_RGBA           GLUT_RGB
#define GLUT_SINGLE         0
#define GLUT_DOUBLE         2
#define GLUT_DEPTH          16
#define GLUT_STENCIL        32

/* Mouse button definitions */
#define GLUT_LEFT_BUTTON    0
#define GLUT_MIDDLE_BUTTON  1
#define GLUT_RIGHT_BUTTON   2

/* Mouse button state definitions */
#define GLUT_DOWN           0
#define GLUT_UP             1

/* GLUT API function declarations */
void glutInit(int *argc, char **argv);
void glutInitDisplayMode(unsigned int mode);
void glutInitWindowSize(int width, int height);
int glutCreateWindow(const char *title);
void glutDisplayFunc(void (*func)(void));
void glutReshapeFunc(void (*func)(int width, int height));
void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y));
void glutMouseFunc(void (*func)(int button, int state, int x, int y));
void glutMotionFunc(void (*func)(int x, int y));
void glutMainLoop(void);
void glutSwapBuffers(void);
void glutPostRedisplay(void);

/* Function to simulate gluPerspective since it's deprecated */
void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

/* Function to simulate gluLookAt since it's deprecated */
void gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
               GLdouble centerX, GLdouble centerY, GLdouble centerZ,
               GLdouble upX, GLdouble upY, GLdouble upZ);

#endif /* __freeglut_h__ */
