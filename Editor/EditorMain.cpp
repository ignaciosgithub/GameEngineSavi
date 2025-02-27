#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../EngineTime.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

#ifdef PLATFORM_WINDOWS
// Windows-specific includes are handled in platform.h
#else
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/keysym.h>
// Include X11/Xlib.h after our EngineTime class has been renamed to avoid conflicts
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
extern "C" {
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
    
    // Set up OpenGL viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
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
            
            // Render editor
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
            editor->HandleInput(LOWORD(lParam), HIWORD(lParam), false);
        }
        return 0;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}
} // extern "C"
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
    
    // Set up OpenGL viewport
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
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
                    editor->HandleInput(event.xmotion.x, event.xmotion.y, false);
                }
                break;
            }
        }
        
        // Update editor
        editor->Update(1.0f / 60.0f);
        
        // Render editor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
