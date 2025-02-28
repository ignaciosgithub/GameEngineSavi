#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../EngineTime.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#endif

// Window dimensions
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

// Global editor instance
std::unique_ptr<Editor> editor;

#ifdef PLATFORM_WINDOWS
// Forward declarations for Windows
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Windows entry point implementation
extern "C" 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Register window class
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "EditorWindowClass";
    RegisterClassEx(&wc);
    
    // Create window
    HWND hWnd = CreateWindow(
        "EditorWindowClass",
        "GameEngineSavi Editor",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    
    // Show window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    // Set up OpenGL
    HDC hDC = GetDC(hWnd);
    
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    int format = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, format, &pfd);
    
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    
    // Set up OpenGL for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Set up viewport and perspective projection
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Main loop
    MSG msg;
    bool running = true;
    
    while (running) {
        // Process messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            // Update editor
            editor->Update(1.0f / 60.0f);
            
            // Clear buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Reset matrices for 3D rendering
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);
            
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            
            // Render editor
            editor->Render();
            SwapBuffers(hDC);
        }
    }
    
    // Clean up OpenGL
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    
    return static_cast<int>(msg.wParam);
}

// Windows procedure implementation
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        
    case WM_DESTROY:
        return 0;
        
    case WM_LBUTTONDOWN:
        if (editor) {
            editor->HandleInput(LOWORD(lParam), HIWORD(lParam), true);
        }
        return 0;
        
    case WM_LBUTTONUP:
        if (editor) {
            editor->HandleInput(LOWORD(lParam), HIWORD(lParam), false);
        }
        return 0;
        
    case WM_MOUSEMOVE:
        if (editor) {
            static int lastX = -1, lastY = -1;
            if (lastX != -1 && lastY != -1) {
                // Calculate mouse movement delta
                int deltaX = LOWORD(lParam) - lastX;
                int deltaY = HIWORD(lParam) - lastY;
                
                // Handle mouse look for camera rotation
                Camera* camera = editor->GetEditorCamera();
                if (camera && (deltaX != 0 || deltaY != 0)) {
                    // Adjust rotation sensitivity
                    float sensitivity = 0.005f;
                    
                    // Update camera look direction based on mouse movement
                    Vector3 dir = camera->lookDirection;
                    Vector3 up(0, 1, 0);
                    Vector3 right = Vector3::Cross(up, dir);
                    right.normalize();
                    
                    // Rotate around Y axis (left/right)
                    float angleY = deltaX * sensitivity;
                    float cosY = cos(angleY);
                    float sinY = sin(angleY);
                    dir = dir * cosY + right * sinY;
                    
                    // Rotate around X axis (up/down)
                    float angleX = deltaY * sensitivity;
                    Vector3 newUp = Vector3::Cross(dir, right);
                    newUp.normalize();
                    dir = dir * cos(angleX) + newUp * sin(angleX);
                    
                    dir.normalize();
                    camera->lookDirection = dir;
                    
                    std::cout << "Camera direction: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
                }
            }
            
            // Store current mouse position for next frame
            lastX = LOWORD(lParam);
            lastY = HIWORD(lParam);
            
            // Pass to regular input handling
            editor->HandleInput(LOWORD(lParam), HIWORD(lParam), false);
        }
        return 0;
        
    case WM_KEYDOWN:
        if (editor) {
            // Handle WASD keys for camera movement
            Camera* camera = editor->GetEditorCamera();
            if (camera) {
                Vector3 pos = camera->GetPosition();
                Vector3 dir = camera->lookDirection;
                Vector3 right = Vector3::Cross(Vector3(0, 1, 0), dir);
                right.normalize();
                
                float moveSpeed = 0.5f;
                
                // W - Move forward
                if (wParam == 'W') {
                    pos = pos + dir * moveSpeed;
                }
                // S - Move backward
                else if (wParam == 'S') {
                    pos = pos - dir * moveSpeed;
                }
                // A - Move left
                else if (wParam == 'A') {
                    pos = pos - right * moveSpeed;
                }
                // D - Move right
                else if (wParam == 'D') {
                    pos = pos + right * moveSpeed;
                }
                
                camera->SetPosition(pos);
                std::cout << "Camera position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
            }
        }
        return 0;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
#else
// Linux entry point
int main(int argc, char** argv) {
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Open X display
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::cerr << "Cannot open X display" << std::endl;
        return 1;
    }
    
    // Get default screen
    int screen = DefaultScreen(display);
    
    // Get root window
    Window root = RootWindow(display, screen);
    
    // Set up OpenGL visual
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    XVisualInfo* vi = glXChooseVisual(display, screen, att);
    if (vi == NULL) {
        std::cerr << "No appropriate visual found" << std::endl;
        return 1;
    }
    
    // Create colormap
    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    
    // Set up window attributes
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    
    // Create window
    Window window = XCreateWindow(
        display, root,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
        vi->depth, InputOutput, vi->visual,
        CWColormap | CWEventMask, &swa
    );
    
    // Set window title
    XStoreName(display, window, "GameEngineSavi Editor");
    
    // Map window
    XMapWindow(display, window);
    
    // Create OpenGL context
    GLXContext context = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, window, context);
    
    // Set up OpenGL for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    
    // Set up viewport and perspective projection
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Main loop
    bool running = true;
    
    while (running) {
        // Process X events
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);
            
            switch (event.type) {
            case Expose:
                // Redraw
                break;
                
            case KeyPress:
                // Handle key press
                if (XLookupKeysym(&event.xkey, 0) == XK_Escape) {
                    running = false;
                }
                // Handle WASD keys for camera movement
                else if (editor) {
                    Camera* camera = editor->GetEditorCamera();
                    if (camera) {
                        Vector3 pos = camera->GetPosition();
                        Vector3 dir = camera->lookDirection;
                        Vector3 right = Vector3::Cross(Vector3(0, 1, 0), dir);
                        right.normalize();
                        
                        float moveSpeed = 0.5f;
                        
                        // W - Move forward
                        if (XLookupKeysym(&event.xkey, 0) == XK_w || XLookupKeysym(&event.xkey, 0) == XK_W) {
                            pos = pos + dir * moveSpeed;
                        }
                        // S - Move backward
                        else if (XLookupKeysym(&event.xkey, 0) == XK_s || XLookupKeysym(&event.xkey, 0) == XK_S) {
                            pos = pos - dir * moveSpeed;
                        }
                        // A - Move left
                        else if (XLookupKeysym(&event.xkey, 0) == XK_a || XLookupKeysym(&event.xkey, 0) == XK_A) {
                            pos = pos - right * moveSpeed;
                        }
                        // D - Move right
                        else if (XLookupKeysym(&event.xkey, 0) == XK_d || XLookupKeysym(&event.xkey, 0) == XK_D) {
                            pos = pos + right * moveSpeed;
                        }
                        
                        camera->SetPosition(pos);
                        std::cout << "Camera position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
                    }
                }
                break;
                
            case ButtonPress:
                if (editor) {
                    editor->HandleInput(event.xbutton.x, event.xbutton.y, true);
                }
                break;
                
            case ButtonRelease:
                if (editor) {
                    editor->HandleInput(event.xbutton.x, event.xbutton.y, false);
                }
                break;
                
            case MotionNotify:
                if (editor) {
                    static int lastX = -1, lastY = -1;
                    if (lastX != -1 && lastY != -1) {
                        // Calculate mouse movement delta
                        int deltaX = event.xmotion.x - lastX;
                        int deltaY = event.xmotion.y - lastY;
                        
                        // Handle mouse look for camera rotation
                        Camera* camera = editor->GetEditorCamera();
                        if (camera && (deltaX != 0 || deltaY != 0)) {
                            // Adjust rotation sensitivity
                            float sensitivity = 0.005f;
                            
                            // Update camera look direction based on mouse movement
                            Vector3 dir = camera->lookDirection;
                            Vector3 up(0, 1, 0);
                            Vector3 right = Vector3::Cross(up, dir);
                            right.normalize();
                            
                            // Rotate around Y axis (left/right)
                            float angleY = deltaX * sensitivity;
                            float cosY = cos(angleY);
                            float sinY = sin(angleY);
                            dir = dir * cosY + right * sinY;
                            
                            // Rotate around X axis (up/down)
                            float angleX = deltaY * sensitivity;
                            Vector3 newUp = Vector3::Cross(dir, right);
                            newUp.normalize();
                            dir = dir * cos(angleX) + newUp * sin(angleX);
                            
                            dir.normalize();
                            camera->lookDirection = dir;
                            
                            std::cout << "Camera direction: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;
                        }
                    }
                    
                    // Store current mouse position for next frame
                    lastX = event.xmotion.x;
                    lastY = event.xmotion.y;
                    
                    // Pass to regular input handling
                    editor->HandleInput(event.xmotion.x, event.xmotion.y, false);
                }
                break;
            }
        }
        
        // Update editor
        editor->Update(1.0f / 60.0f);
        
        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Reset matrices for 3D rendering
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Render editor
        editor->Render();
        glXSwapBuffers(display, window);
    }
    
    // Clean up OpenGL
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    
    // Clean up X
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    
    return 0;
}
#endif
