#ifndef EDITORMAIN_H
#define EDITORMAIN_H

// Function declarations
void InitializeEditor();
void RenderScene();
void Resize(int width, int height);
void KeyboardInput(unsigned char key, int x, int y);
void MouseInput(int button, int state, int x, int y);
void MouseMotion(int x, int y);

#endif // EDITORMAIN_H
