/**
 * main35engine.cpp
 * Main entry point for the GameEngineSavi engine
 * 
 * This file contains platform-specific code for both Windows and Linux/Mac
 */

#include "platform.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <deque>
#include <list>
#include <array>
#include <tuple>
#include <utility>
#include <random>
#include <limits>
#include <type_traits>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <ctime>
#include <cstdint>
#include <cerrno>
#include <cfloat>

#include "EngineCondition.h"
#include "TimeManager.h"
#include "Camera.h"
#include "Scene.h"

// Helper function to convert degrees to radians
float toRadians(float degrees) {
    return degrees * 3.14159f / 180.0f;
}

// Platform-specific type definitions and includes
#ifdef PLATFORM_WINDOWS
// Windows-specific type definitions
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
#define CALLBACK __stdcall
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
    int    pt_x;
    int    pt_y;
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

// Windows function declarations
#ifndef LoadIcon
HICON LoadIcon(HINSTANCE hInstance, const char* lpIconName);
#endif
#ifndef LoadCursor
HCURSOR LoadCursor(HINSTANCE hInstance, const char* lpCursorName);
#endif
#ifndef IDI_APPLICATION
#define IDI_APPLICATION ((const char*)32512)
#endif
#ifndef IDC_ARROW
#define IDC_ARROW ((const char*)32512)
#endif
#ifndef GetStockObject
HGDIOBJ GetStockObject(int i);
#endif
#ifndef BLACK_BRUSH
#define BLACK_BRUSH 4
#endif
#ifndef RegisterClass
ATOM RegisterClass(const WNDCLASS* lpWndClass);
#endif
#ifndef CreateWindow
HWND CreateWindow(const char* lpClassName, const char* lpWindowName, UINT dwStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, void* hMenu, HINSTANCE hInstance, void* lpParam);
#endif
#ifndef WS_CAPTION
#define WS_CAPTION 0x00C00000L
#endif
#ifndef WS_POPUPWINDOW
#define WS_POPUPWINDOW 0x80880000L
#endif
#ifndef WS_VISIBLE
#define WS_VISIBLE 0x10000000L
#endif
#ifndef DestroyWindow
BOOL DestroyWindow(HWND hWnd);
#endif
#ifndef SwapBuffers
BOOL SwapBuffers(HDC hdc);
#endif
#ifndef PeekMessage
BOOL PeekMessage(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
#endif
#ifndef PM_REMOVE
#define PM_REMOVE 0x0001
#endif
#ifndef WM_QUIT
#define WM_QUIT 0x0012
#endif
#ifndef TranslateMessage
BOOL TranslateMessage(const MSG* lpMsg);
#endif
#ifndef DispatchMessage
LRESULT DispatchMessage(const MSG* lpMsg);
#endif
#ifndef PostQuitMessage
void PostQuitMessage(int nExitCode);
#endif
#ifndef DefWindowProc
LRESULT DefWindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#endif
#ifndef WM_CREATE
#define WM_CREATE 0x0001
#endif
#ifndef WM_CLOSE
#define WM_CLOSE 0x0010
#endif
#ifndef WM_DESTROY
#define WM_DESTROY 0x0002
#endif
#ifndef WM_KEYDOWN
#define WM_KEYDOWN 0x0100
#endif
#ifndef VK_ESCAPE
#define VK_ESCAPE 0x1B
#endif
#ifndef WM_LBUTTONDOWN
#define WM_LBUTTONDOWN 0x0201
#endif
#ifndef WM_MOUSEMOVE
#define WM_MOUSEMOVE 0x0200
#endif
#ifndef LOWORD
#define LOWORD(l) ((UINT)(((UINT_PTR)(l)) & 0xffff))
#endif
#ifndef HIWORD
#define HIWORD(l) ((UINT)((((UINT_PTR)(l)) >> 16) & 0xffff))
#endif
#ifndef GetDC
HDC GetDC(HWND hWnd);
#endif
#ifndef ReleaseDC
int ReleaseDC(HWND hWnd, HDC hDC);
#endif
#ifndef ZeroMemory
#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))
#endif
#ifndef PFD_DRAW_TO_WINDOW
#define PFD_DRAW_TO_WINDOW 0x00000004
#endif
#ifndef PFD_SUPPORT_OPENGL
#define PFD_SUPPORT_OPENGL 0x00000020
#endif
#ifndef PFD_DOUBLEBUFFER
#define PFD_DOUBLEBUFFER 0x00000001
#endif
#ifndef PFD_TYPE_RGBA
#define PFD_TYPE_RGBA 0
#endif
#ifndef PFD_MAIN_PLANE
#define PFD_MAIN_PLANE 0
#endif
#ifndef ChoosePixelFormat
int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd);
#endif
#ifndef SetPixelFormat
BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd);
#endif
#ifndef wglCreateContext
HGLRC wglCreateContext(HDC hdc);
#endif
#ifndef wglMakeCurrent
BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc);
#endif
#ifndef wglDeleteContext
BOOL wglDeleteContext(HGLRC hglrc);
#endif
#ifndef CS_OWNDC
#define CS_OWNDC 0x0020
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef HGDIOBJ
typedef void* HGDIOBJ;
#endif
#ifndef WNDPROC
typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
#endif
#ifndef ATOM
typedef unsigned short ATOM;
#endif
#ifndef UINT_PTR
typedef unsigned long long UINT_PTR;
#endif
#ifndef __stdcall
#define __stdcall
#endif

// Forward declarations for Windows-specific functions
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
#endif // End of PLATFORM_WINDOWS block for Windows types

/**************************
 * Main Entry Point
 **************************/

// Platform-specific main function declarations
#ifdef PLATFORM_WINDOWS
int WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int iCmdShow)
#else
int main(int argc, char** argv)
#endif
{
    // Set the initial engine condition based on command line arguments
    // This would normally be set by the editor or build system
    #ifdef DEBUG_BUILD
    // Debug build - default to editor mode
    EngineCondition::isInEditor = true;
    #else
    // Release build - default to game mode
    EngineCondition::isInEditor = false;
    #endif

    // Platform-specific initialization
    #ifdef PLATFORM_WINDOWS
    // Windows-specific variables
    HWND hWnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    #else
    // Linux/Mac variables
    bool running = true;
    #endif

    // Initialize time manager
    TimeManager timeManager;
    
    // Create camera
    Camera camera;
    camera.SetPosition(Vector3(0.0f, 0.0f, 5.0f));
    camera.SetRotation(Vector3(0.0f, 0.0f, 0.0f));
    
    // Create scene
    Scene scene;
    scene.Initialize();
    
    // Platform-specific window creation and main loop
    #ifdef PLATFORM_WINDOWS
    // Register the window class
    WNDCLASS wc;
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = (HINSTANCE)hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GameEngineSavi";
    
    if (!RegisterClass(&wc)) {
        std::cerr << "Failed to register window class" << std::endl;
        return 1;
    }
    
    // Create the window
    hWnd = CreateWindow(
        "GameEngineSavi", "GameEngineSavi",
        WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
        0, 0, 800, 600,
        NULL, NULL, (HINSTANCE)hInstance, NULL
    );
    
    if (!hWnd) {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }
    
    // Enable OpenGL for the window
    EnableOpenGL(hWnd, &hDC, &hRC);
    
    // Main message loop
    while (!bQuit) {
        // Check for messages
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            // Check if it's a quit message
            if (msg.message == WM_QUIT) {
                bQuit = TRUE;
            } else {
                // Translate and dispatch the message
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            // Update time
            timeManager.Update();
            float deltaTime = timeManager.GetDeltaTime();
            
            // Update camera
            camera.Update(deltaTime);
            
            // Update scene
            scene.Update(deltaTime);
            
            // Render scene
            scene.Render();
            
            // Swap buffers
            SwapBuffers(hDC);
            
            // Small delay to prevent maxing out CPU
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    
    // Shutdown
    DisableOpenGL(hWnd, hDC, hRC);
    DestroyWindow(hWnd);

    return msg.wParam;
    #else // PLATFORM_LINUX or other non-Windows platforms
    // Linux/Mac implementation would go here
    std::cout << "Engine running in " << 
        (EngineCondition::IsInEditor() ? "editor mode" : "unknown mode") 
        << std::endl;
    
    // Main loop
    while (running) {
        // Update time
        timeManager.Update();
        float deltaTime = timeManager.GetDeltaTime();
        
        // Update camera
        camera.Update(deltaTime);
        
        // Update scene
        scene.Update(deltaTime);
        
        // Render scene
        scene.Render();
        
        // Small delay to prevent maxing out CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        
        // For testing purposes, exit after a few seconds
        static int frameCount = 0;
        if (++frameCount > 100) {
            running = false;
        }
    }
    
    return 0;
    #endif
}

#ifdef PLATFORM_WINDOWS
/********************
 * Window Procedure
 ********************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        return 0;
        
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
        
    case WM_DESTROY:
        return 0;
        
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            return 0;
        }
        return 0;
        
    case WM_LBUTTONDOWN:
        // Handle mouse click
        return 0;
        
    case WM_MOUSEMOVE:
        // Handle mouse movement
        return 0;
        
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

/******************
 * Enable OpenGL
 ******************/
void EnableOpenGL(HWND hWnd, HDC* hDC, HGLRC* hRC) {
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;
    
    // Get the device context
    *hDC = GetDC(hWnd);
    
    // Set the pixel format for the device context
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    iFormat = ChoosePixelFormat(*hDC, &pfd);
    SetPixelFormat(*hDC, iFormat, &pfd);
    
    // Create and enable the OpenGL rendering context
    *hRC = wglCreateContext(*hDC);
    wglMakeCurrent(*hDC, *hRC);
}

/******************
 * Disable OpenGL
 ******************/
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}
#endif // End of PLATFORM_WINDOWS block
