/**************************
 * Includes
 *
 **************************/

#include "platform.h"
#include "ThirdParty/OpenGL/include/GL/gl_definitions.h"
#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/gl.h>
#include "ThirdParty/OpenGL/include/GL/win_types.h"
#endif
#include <vector>
#include <string>
#include <cstring>
#include "Vector3.h"
#include "Triangle.h"
#include "Matrix4x4.h"
#include "Raycast.h"
#include "PointLight.h"
#include "Camera.h"
#include "Face.h"
#include "Pyramid.h"
#include "Model.h"
#include "MonoBehaviourLike.h"
#include "GameObject.h"
#include "Prefab.h"
#include "TimeManager.h"
#include "Scene.h"
#include "EngineCondition.h"
#include "GUI/GUI.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <atomic>

/**************************
 * Function Declarations
 *
 **************************/
inline float toRadians(float degrees) {
    return degrees * static_cast<float>(M_PI) / 180.0f;
}

// Global GUI instance
std::unique_ptr<GUI> gui;

// Forward declarations for Windows-specific types and functions
#ifdef PLATFORM_WINDOWS
// Define Windows types if not already defined
#ifndef HWND
typedef void* HWND;
#endif
#ifndef HDC
typedef void* HDC;
#endif
#ifndef HGLRC
typedef void* HGLRC;
#endif
#ifndef UINT
typedef unsigned int UINT;
#endif
#ifndef WPARAM
typedef unsigned long long WPARAM;
#endif
#ifndef LPARAM
typedef long long LPARAM;
#endif
#ifndef LRESULT
typedef long long LRESULT;
#endif
#ifndef CALLBACK
#define CALLBACK
#endif
#ifndef HINSTANCE
typedef void* HINSTANCE;
#endif
#ifndef LPSTR
typedef char* LPSTR;
#endif
#ifndef HICON
typedef void* HICON;
#endif
#ifndef HCURSOR
typedef void* HCURSOR;
#endif
#ifndef HBRUSH
typedef void* HBRUSH;
#endif
#ifndef BOOL
typedef int BOOL;
#endif
#ifndef WNDCLASS
typedef struct {
    UINT      style;
    void*     lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    const char* lpszMenuName;
    const char* lpszClassName;
} WNDCLASS;
#endif
#ifndef MSG
typedef struct {
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    UINT   time;
    void*  pt;
} MSG;
#endif
#ifndef PIXELFORMATDESCRIPTOR
typedef struct {
    UINT  nSize;
    UINT  nVersion;
    UINT  dwFlags;
    UINT  iPixelType;
    UINT  cColorBits;
    UINT  cRedBits;
    UINT  cRedShift;
    UINT  cGreenBits;
    UINT  cGreenShift;
    UINT  cBlueBits;
    UINT  cBlueShift;
    UINT  cAlphaBits;
    UINT  cAlphaShift;
    UINT  cAccumBits;
    UINT  cAccumRedBits;
    UINT  cAccumGreenBits;
    UINT  cAccumBlueBits;
    UINT  cAccumAlphaBits;
    UINT  cDepthBits;
    UINT  cStencilBits;
    UINT  cAuxBuffers;
    UINT  iLayerType;
    UINT  bReserved;
    UINT  dwLayerMask;
    UINT  dwVisibleMask;
    UINT  dwDamageMask;
} PIXELFORMATDESCRIPTOR;
#endif

// Define Windows constants and functions if not already defined
#ifndef LoadIcon
#define LoadIcon(a, b) nullptr
#endif
#ifndef LoadCursor
#define LoadCursor(a, b) nullptr
#endif
#ifndef IDI_APPLICATION
#define IDI_APPLICATION nullptr
#endif
#ifndef IDC_ARROW
#define IDC_ARROW nullptr
#endif
#ifndef GetStockObject
#define GetStockObject(a) nullptr
#endif
#ifndef BLACK_BRUSH
#define BLACK_BRUSH 0
#endif
#ifndef RegisterClass
#define RegisterClass(a) 0
#endif
#ifndef CreateWindow
#define CreateWindow(a, b, c, d, e, f, g, h, i, j, k) nullptr
#endif
#ifndef WS_CAPTION
#define WS_CAPTION 0
#endif
#ifndef WS_POPUPWINDOW
#define WS_POPUPWINDOW 0
#endif
#ifndef WS_VISIBLE
#define WS_VISIBLE 0
#endif
#ifndef DestroyWindow
#define DestroyWindow(a)
#endif
#ifndef SwapBuffers
#define SwapBuffers(a)
#endif
#ifndef PeekMessage
#define PeekMessage(a, b, c, d, e) 0
#endif
#ifndef PM_REMOVE
#define PM_REMOVE 0
#endif
#ifndef WM_QUIT
#define WM_QUIT 0
#endif
#ifndef TranslateMessage
#define TranslateMessage(a)
#endif
#ifndef DispatchMessage
#define DispatchMessage(a)
#endif
#ifndef PostQuitMessage
#define PostQuitMessage(a)
#endif
#ifndef DefWindowProc
#define DefWindowProc(a, b, c, d) 0
#endif
#ifndef WM_CREATE
#define WM_CREATE 0
#endif
#ifndef WM_CLOSE
#define WM_CLOSE 0
#endif
#ifndef WM_DESTROY
#define WM_DESTROY 0
#endif
#ifndef WM_KEYDOWN
#define WM_KEYDOWN 0
#endif
#ifndef VK_ESCAPE
#define VK_ESCAPE 0
#endif
#ifndef WM_LBUTTONDOWN
#define WM_LBUTTONDOWN 0
#endif
#ifndef WM_MOUSEMOVE
#define WM_MOUSEMOVE 0
#endif
#ifndef LOWORD
#define LOWORD(a) 0
#endif
#ifndef HIWORD
#define HIWORD(a) 0
#endif
#ifndef GetDC
#define GetDC(a) nullptr
#endif
#ifndef ReleaseDC
#define ReleaseDC(a, b)
#endif
#ifndef ZeroMemory
#define ZeroMemory(a, b) memset(a, 0, b)
#endif
#ifndef PFD_DRAW_TO_WINDOW
#define PFD_DRAW_TO_WINDOW 0
#endif
#ifndef PFD_SUPPORT_OPENGL
#define PFD_SUPPORT_OPENGL 0
#endif
#ifndef PFD_DOUBLEBUFFER
#define PFD_DOUBLEBUFFER 0
#endif
#ifndef PFD_TYPE_RGBA
#define PFD_TYPE_RGBA 0
#endif
#ifndef PFD_MAIN_PLANE
#define PFD_MAIN_PLANE 0
#endif
#ifndef ChoosePixelFormat
#define ChoosePixelFormat(a, b) 0
#endif
#ifndef SetPixelFormat
#define SetPixelFormat(a, b, c) 0
#endif
#ifndef wglCreateContext
#define wglCreateContext(a) nullptr
#endif
#ifndef wglMakeCurrent
#define wglMakeCurrent(a, b) 0
#endif
#ifndef wglDeleteContext
#define wglDeleteContext(a)
#endif
#ifndef CS_OWNDC
#define CS_OWNDC 0
#endif
#ifndef NULL
#define NULL nullptr
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Forward declarations for Windows-specific functions
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
#endif

/**************************
 * Main Entry Point
 *
 **************************/

#ifdef PLATFORM_WINDOWS
int WinMain(void* hInstance,
            void* hPrevInstance,
            char* lpCmdLine,
            int iCmdShow)
#else
int main(int argc, char** argv)
#endif
{
    // Set the initial engine condition based on command line arguments
    // This would normally be set by the editor or build system
    #ifdef DEBUG_BUILD
    // Using available EngineCondition API
    EngineCondition::isInEditor = true;
    std::cout << "Starting engine in DEBUG mode" << std::endl;
    #else
    // Using available EngineCondition API
    EngineCondition::isInEditor = true;
    std::cout << "Starting engine in RELEASE mode" << std::endl;
    #endif

    // Check for editor mode command line arguments
    // In a real implementation, this would be handled by the editor
    #ifdef PLATFORM_WINDOWS
    if (lpCmdLine && strstr(lpCmdLine, "-editor")) {
        EngineCondition::isInEditor = true;
        std::cout << "Starting in editor mode" << std::endl;
    }
    #else
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-editor") == 0) {
            EngineCondition::isInEditor = true;
            std::cout << "Starting in editor mode" << std::endl;
            break;
        }
    }
    #endif

    // Initialize GUI
    // Use C++11 compatible way to create unique_ptr
    gui.reset(new GUI());
    
    // Create editor panel
    std::unique_ptr<Panel> editorPanel(new Panel(10, 10, 200, 580));
    
    // Create play button
    std::unique_ptr<Button> playButton(new Button(20, 20, 80, 30, "Play"));
    
    // Set button click handler
    playButton->SetOnClick([]() {
        if (EngineCondition::IsInEditor()) {
            // Using available EngineCondition API
            std::cout << "Entering play mode" << std::endl;
        }
    });
    
    // Add button to panel and panel to GUI
    editorPanel->AddElement(std::move(playButton));
    gui->AddElement(std::move(editorPanel));

    #ifdef PLATFORM_WINDOWS
    WNDCLASS wc;
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;        
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = (HINSTANCE)hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass(&wc);

    /* create main window */
    hWnd = CreateWindow(
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 256, 256,
      NULL, NULL, (HINSTANCE)hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL(hWnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // If we're in editor mode, show a message
            if (EngineCondition::IsInEditor()) {
                std::cout << "In editor mode..." << std::endl;
                // In a real implementation, this would trigger the compilation process
                // For now, we'll just wait a bit
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
            
            // Draw GUI
            gui->Draw();
            
            // Swap buffers
            SwapBuffers(hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hWnd);

    return msg.wParam;
    #else
    // Linux/Mac implementation would go here
    std::cout << "Engine running in " << 
        (EngineCondition::IsInEditor() ? "editor mode" : "unknown mode") 
        << std::endl;
    
    // Simulate some engine activity
    for (int i = 0; i < 5; i++) {
        std::cout << "Engine tick " << i << std::endl;
        
        // If we're in editor mode, show a message
        if (EngineCondition::IsInEditor()) {
            std::cout << "In editor mode..." << std::endl;
            // In a real implementation, this would trigger the compilation process
            // For now, we'll just wait a bit
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        
        // In a real implementation, we would draw the GUI here
        // gui->Draw();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
    #endif
}

#ifdef PLATFORM_WINDOWS
/********************
 * Window Procedure
 *
 ********************/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;

    case WM_DESTROY:
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        case 'P': // Press P to toggle play mode in editor
            if (EngineCondition::IsInEditor()) {
                // Using available EngineCondition API
                std::cout << "Entering play mode" << std::endl;
            } else {
                // Using available EngineCondition API
                std::cout << "Entering edit mode" << std::endl;
            }
            return 0;
        case 'C': // Press C to toggle compile mode in editor
            if (EngineCondition::IsInEditor()) {
                // Using available EngineCondition API
                std::cout << "Entering compile mode" << std::endl;
            }
            return 0;
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        gui->HandleInput(LOWORD(lParam), HIWORD(lParam), true);
        return 0;
        
    case WM_MOUSEMOVE:
        gui->HandleInput(LOWORD(lParam), HIWORD(lParam), false);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hWnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
#endif
