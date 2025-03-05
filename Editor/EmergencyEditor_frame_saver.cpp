/**
 * EmergencyEditor_frame_saver.cpp
 * Emergency editor for GameEngineSavi that saves frames to a folder.
 * 
 * This file contains a modified version of the editor that renders each frame
 * to a PNG file instead of directly to the screen.
 */

#include "Editor.h"
#include "../EngineCondition.h"
#include "../platform.h"
#include "../TimeManager.h"
#include "../ThirdParty/OpenGL/include/GL/gl_types.h"
#include "../FrameCapture_png.h"
#include "../Camera.h"
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib> // For system()

// Platform-specific includes
#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include "../ThirdParty/OpenGL/include/GL/platform_gl.h"
#else
    #include "../ThirdParty/OpenGL/include/GL/platform_gl.h"
    #include <X11/keysym.h>
    #include <X11/Xlib.h>
#endif

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Global editor instance
std::unique_ptr<Editor> editor;

// Frame counter
int frameCount = 0;

// Function to generate frame filename
std::string GetFrameFilename() {
    std::stringstream ss;
    ss << "frames/frame" << frameCount << ".png";
    return ss.str();
}

// Common initialization function
void InitializeEditor() {
    // Create editor
    editor = std::unique_ptr<Editor>(new Editor(WINDOW_WIDTH, WINDOW_HEIGHT));
    
    // Initialize editor
    editor->Initialize();
    
    // Create frames directory
    system("mkdir -p frames");
    
    // Set engine condition to editor mode
    EngineCondition::SetInEditor(true);
}

#ifdef PLATFORM_WINDOWS
// Windows entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::cout << "Starting emergency editor frame saver on Windows..." << std::endl;
    
    // Initialize editor
    InitializeEditor();
    
    // Register window class
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "EmergencyEditorFrameSaverClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return 1;
    }
    
    // Create window
    HWND hWnd = CreateWindow(
        "EmergencyEditorFrameSaverClass", "Emergency Editor Frame Saver",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );
    
    if (!hWnd) {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }
    
    // Get device context
    HDC hDC = GetDC(hWnd);
    
    // Set pixel format
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    int format = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, format, &pfd);
    
    // Create OpenGL context
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    
    // Main loop
    bool running = true;
    TimeManager timeManager;
    
    while (running && frameCount < 10) { // Limit to 10 frames for testing
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Update editor
        if (editor) {
            editor->Update(deltaTime);
            editor->Render();
            
            // Capture frame to file using PNG format
            std::string filename = GetFrameFilename();
            FrameCapture_PNG::CaptureViewportToFile(filename);
            std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
            
            // Increment frame counter
            frameCount++;
            
            // Small delay to not overwhelm the system
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Process messages
        MSG msg;
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    // Clean up
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);
    
    std::cout << "Emergency editor frame saving complete. " << frameCount << " frames rendered." << std::endl;
    
    return 0;
}
#else
// Linux entry point
int main(int argc, char** argv) {
    std::cout << "Starting emergency editor frame saver on Linux..." << std::endl;
    
    // Initialize editor
    InitializeEditor();
    
    // Open X display
    Display* display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Failed to open X display" << std::endl;
        
        // Even without a display, we can still render to files
        std::cout << "Continuing with headless rendering..." << std::endl;
        
        // Main loop for headless rendering
        bool running = true;
        TimeManager timeManager;
        
        while (running && frameCount < 10) { // Limit to 10 frames for testing
            // Update time
            timeManager.Update();
            float deltaTime = timeManager.GetDeltaTime();
            
            // Update editor
            if (editor) {
                editor->Update(deltaTime);
                editor->Render();
                
                // Capture frame to file using PNG format
                std::string filename = GetFrameFilename();
                FrameCapture_PNG::CaptureViewportToFile(filename);
                std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
                
                // Increment frame counter
                frameCount++;
                
                // Small delay to not overwhelm the system
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        
        std::cout << "Emergency editor frame saving complete. " << frameCount << " frames rendered." << std::endl;
        
        return 0;
    }
    
    // Get root window
    Window root = DefaultRootWindow(display);
    
    // Get visual info
    XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), NULL);
    if (!vi) {
        std::cerr << "Failed to get visual info" << std::endl;
        XCloseDisplay(display);
        return 1;
    }
    
    // Create colormap
    Visual* visual = (Visual*)vi->visual;
    Colormap cmap = XCreateColormap(display, root, visual, AllocNone);
    
    // Set up window attributes
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    
    // Create window
    Window window = XCreateWindow(
        display, root,
        0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0,
        vi->depth, InputOutput, visual,
        CWColormap | CWEventMask, &swa
    );
    
    // Set window title
    XStoreName(display, window, "Emergency Editor Frame Saver");
    
    // Map window
    XMapWindow(display, window);
    
    // Create OpenGL context
    GLXContext context = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, window, context);
    
    // Force software rendering
    putenv((char*)"LIBGL_ALWAYS_SOFTWARE=1");
    std::cout << "Forcing software rendering" << std::endl;
    
    // Set up OpenGL for 3D rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    // Main loop
    bool running = true;
    TimeManager timeManager;
    
    while (running && frameCount < 10) { // Limit to 10 frames for testing
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Update editor
        if (editor) {
            editor->Update(deltaTime);
            editor->Render();
            
            // Capture frame to file using PNG format
            std::string filename = GetFrameFilename();
            FrameCapture_PNG::CaptureViewportToFile(filename);
            std::cout << "Rendered frame " << frameCount << " to " << filename << std::endl;
            
            // Increment frame counter
            frameCount++;
            
            // Small delay to not overwhelm the system
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        // Process events
        while (XPending(display)) {
            XEvent event;
            XNextEvent(display, &event);
            
            switch (event.type) {
            case Expose:
                // Redraw
                if (editor) {
                    editor->Render();
                }
                break;
                
            case KeyPress:
                // Handle key press
                if (XLookupKeysym(&event.xkey, 0) == XK_Escape) {
                    running = false;
                }
                break;
            }
        }
        
        // Swap buffers
        glXSwapBuffers(display, window);
    }
    
    // Clean up
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, context);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    
    std::cout << "Emergency editor frame saving complete. " << frameCount << " frames rendered." << std::endl;
    
    return 0;
}
#endif
