/**************************
 * Includes
 *
 **************************/

#include "platform.h"
#include <vector>
#include <string>
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
#include "Time.h"
#include "Scene.h"
#include "EngineCondition.h"
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

#ifdef PLATFORM_WINDOWS
LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
WPARAM wParam, LPARAM lParam);
void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC);
#endif

/**************************
 * Main Entry Point
 *
 **************************/

#ifdef PLATFORM_WINDOWS
int WINAPI WinMain (HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int iCmdShow)
#else
int main(int argc, char** argv)
#endif
{
    // Set the initial engine condition based on command line arguments
    // This would normally be set by the editor or build system
    #ifdef DEBUG_BUILD
    EngineCondition::SetState(EngineCondition::State::DEBUG_BUILD);
    std::cout << "Starting engine in DEBUG mode" << std::endl;
    #else
    EngineCondition::SetState(EngineCondition::State::RELEASE_BUILD);
    std::cout << "Starting engine in RELEASE mode" << std::endl;
    #endif

    // Check for editor mode command line arguments
    // In a real implementation, this would be handled by the editor
    #ifdef PLATFORM_WINDOWS
    if (lpCmdLine && strstr(lpCmdLine, "-editor")) {
        EngineCondition::EnterEditMode();
        std::cout << "Starting in editor mode" << std::endl;
    }
    #else
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-editor") == 0) {
            EngineCondition::EnterEditMode();
            std::cout << "Starting in editor mode" << std::endl;
            break;
        }
    }
    #endif

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
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "GLSample";
    RegisterClass (&wc);

    /* create main window */
    hWnd = CreateWindow (
      "GLSample", "OpenGL Sample", 
      WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
      0, 0, 256, 256,
      NULL, NULL, hInstance, NULL);

    /* enable OpenGL for the window */
    EnableOpenGL (hWnd, &hDC, &hRC);

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage (&msg);
                DispatchMessage (&msg);
            }
        }
        else
        {
            // If we're in editor compiling mode, show a message
            if (EngineCondition::IsInEditorCompiling()) {
                std::cout << "Compiling game in editor..." << std::endl;
                // In a real implementation, this would trigger the compilation process
                // For now, we'll just switch back to edit mode after a delay
                std::this_thread::sleep_for(std::chrono::seconds(2));
                EngineCondition::EnterEditMode();
            }
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL (hWnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow (hWnd);

    return msg.wParam;
    #else
    // Linux/Mac implementation would go here
    std::cout << "Engine running in " << 
        (EngineCondition::IsInEditorPlaying() ? "editor play mode" : 
         EngineCondition::IsInEditorEditing() ? "editor edit mode" :
         EngineCondition::IsInEditorCompiling() ? "editor compile mode" :
         EngineCondition::IsDebugBuild() ? "debug build" : 
         EngineCondition::IsReleaseBuild() ? "release build" : "unknown mode") 
        << std::endl;
    
    // Simulate some engine activity
    for (int i = 0; i < 5; i++) {
        std::cout << "Engine tick " << i << std::endl;
        
        // If we're in editor compiling mode, show a message
        if (EngineCondition::IsInEditorCompiling()) {
            std::cout << "Compiling game in editor..." << std::endl;
            // In a real implementation, this would trigger the compilation process
            // For now, we'll just switch back to edit mode after a delay
            std::this_thread::sleep_for(std::chrono::seconds(2));
            EngineCondition::EnterEditMode();
        }
        
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

LRESULT CALLBACK WndProc (HWND hWnd, UINT message,
                          WPARAM wParam, LPARAM lParam)
{

    switch (message)
    {
    case WM_CREATE:
        return 0;
    case WM_CLOSE:
        PostQuitMessage (0);
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
            if (EngineCondition::IsInEditorEditing()) {
                EngineCondition::EnterPlayMode();
                std::cout << "Entering play mode" << std::endl;
            } else if (EngineCondition::IsInEditorPlaying()) {
                EngineCondition::EnterEditMode();
                std::cout << "Entering edit mode" << std::endl;
            }
            return 0;
        case 'C': // Press C to toggle compile mode in editor
            if (EngineCondition::IsInEditor()) {
                EngineCondition::EnterCompileMode();
                std::cout << "Entering compile mode" << std::endl;
            }
            return 0;
        }
        return 0;

    default:
        return DefWindowProc (hWnd, message, wParam, lParam);
    }
}


/*******************
 * Enable OpenGL
 *
 *******************/

void EnableOpenGL (HWND hWnd, HDC *hDC, HGLRC *hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC (hWnd);

    /* set the pixel format for the DC */
    ZeroMemory (&pfd, sizeof (pfd));
    pfd.nSize = sizeof (pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | 
      PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat (*hDC, &pfd);
    SetPixelFormat (*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );

}


/******************
 * Disable OpenGL
 *
 ******************/

void DisableOpenGL (HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent (NULL, NULL);
    wglDeleteContext (hRC);
    ReleaseDC (hWnd, hDC);
}
#endif
