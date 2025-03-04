#pragma once

#include <cstring> // For memset

// Define TRUE and FALSE for cross-platform compatibility
#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

// Define OpenGL constants
#ifndef GL_FALSE
    #define GL_FALSE 0
#endif

#ifndef GL_TRUE
    #define GL_TRUE 1
#endif

// OpenGL type definitions
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;

// Platform-specific code
#ifdef PLATFORM_WINDOWS
    #include <windows.h>
#else
    // Define Windows types for cross-platform compatibility
    typedef unsigned long DWORD;
    typedef DWORD DWORD_PTR;
    typedef unsigned short WORD;
    typedef unsigned char BYTE;
    typedef DWORD COLORREF;
    typedef void* HGLRC;
    typedef void* HDC;
    typedef void* HINSTANCE;
    typedef void* HWND;
    typedef void* LPVOID;
    typedef const char* LPCSTR;
    typedef char* LPSTR;
    typedef LPCSTR LPCTSTR;
    typedef LPSTR LPTSTR;
    typedef long LONG;
    typedef unsigned long ULONG;
    typedef int BOOL;
    typedef DWORD* LPDWORD;
    typedef void* LPARAM;
    typedef unsigned int UINT;
    typedef unsigned short USHORT;
    typedef unsigned int WPARAM;
    typedef long LRESULT;
    
    // Define Windows message constants for cross-platform compatibility
    #define WM_CLOSE 0x0010
    #define WM_DESTROY 0x0002
    #define WM_LBUTTONDOWN 0x0201
    #define WM_LBUTTONUP 0x0202
    #define WM_MOUSEMOVE 0x0200
    #define WM_KEYDOWN 0x0100
    #define VK_F12 0x7B
    
    // Define Windows function macros for cross-platform compatibility
    #ifndef CALLBACK
        #define CALLBACK 
    #endif
    #ifndef WINAPI
        #define WINAPI 
    #endif
    #ifndef APIENTRY
        #define APIENTRY WINAPI
    #endif
    
    // Define Windows helper macros for cross-platform compatibility
    #ifndef LOWORD
        #define LOWORD(l) ((WORD)((DWORD)(l) & 0xffff))
    #endif
    #ifndef HIWORD
        #define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xffff))
    #endif
    
    // Define additional Windows types and functions for cross-platform compatibility
    typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);
    
    // Define Windows class styles
    #define CS_HREDRAW 0x0002
    #define CS_VREDRAW 0x0001
    #define CS_OWNDC 0x0020
    
    // Define Windows functions as empty stubs for cross-platform compatibility
    inline void PostQuitMessage(int) {}
    inline LRESULT DefWindowProc(HWND, UINT, WPARAM, LPARAM) { return 0; }
    inline void* GetStockObject(int) { return nullptr; }
    inline void* LoadIcon(void*, const char*) { return nullptr; }
    inline void* LoadCursor(void*, const char*) { return nullptr; }
    inline void ZeroMemory(void* dest, size_t size) { memset(dest, 0, size); }
    inline int ChoosePixelFormat(HDC, void*) { return 0; }
    inline BOOL SetPixelFormat(HDC, int, void*) { return TRUE; }
    inline HGLRC wglCreateContext(HDC) { return nullptr; }
    inline BOOL wglMakeCurrent(HDC, HGLRC) { return TRUE; }
    inline BOOL wglDeleteContext(HGLRC) { return TRUE; }
    inline BOOL ReleaseDC(HWND, HDC) { return TRUE; }
    inline BOOL SwapBuffers(HDC) { return TRUE; }
    inline BOOL TranslateMessage(void*) { return TRUE; }
    inline LRESULT DispatchMessage(void*) { return 0; }
    inline BOOL PeekMessage(void*, HWND, UINT, UINT, UINT) { return FALSE; }
    inline HWND CreateWindow(const char*, const char*, DWORD, int, int, int, int, HWND, void*, HINSTANCE, void*) { return nullptr; }
    inline BOOL ShowWindow(HWND, int) { return TRUE; }
    inline BOOL UpdateWindow(HWND) { return TRUE; }
    inline HDC GetDC(HWND) { return nullptr; }
    inline BOOL RegisterClassEx(void*) { return TRUE; }
    
    // Define Windows constants for cross-platform compatibility
    #define IDI_APPLICATION ((const char*)32512)
    #define IDC_ARROW ((const char*)32512)
    #define BLACK_BRUSH 4
    #define CW_USEDEFAULT 0x80000000
    #define WS_OVERLAPPEDWINDOW 0x00CF0000
    #define PM_REMOVE 0x0001
    
    // Define Windows structs for cross-platform compatibility
    typedef struct tagWNDCLASSEX {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HINSTANCE hInstance;
        void* hIcon;
        void* hCursor;
        void* hbrBackground;
        const char* lpszMenuName;
        const char* lpszClassName;
        void* hIconSm;
    } WNDCLASSEX;
    
    typedef struct tagMSG {
        HWND hwnd;
        UINT message;
        WPARAM wParam;
        LPARAM lParam;
        DWORD time;
        LONG pt_x;
        LONG pt_y;
    } MSG;
    
    typedef struct tagPIXELFORMATDESCRIPTOR {
        WORD nSize;
        WORD nVersion;
        DWORD dwFlags;
        BYTE iPixelType;
        BYTE cColorBits;
        BYTE cRedBits;
        BYTE cRedShift;
        BYTE cGreenBits;
        BYTE cGreenShift;
        BYTE cBlueBits;
        BYTE cBlueShift;
        BYTE cAlphaBits;
        BYTE cAlphaShift;
        BYTE cAccumBits;
        BYTE cAccumRedBits;
        BYTE cAccumGreenBits;
        BYTE cAccumBlueBits;
        BYTE cAccumAlphaBits;
        BYTE cDepthBits;
        BYTE cStencilBits;
        BYTE cAuxBuffers;
        BYTE iLayerType;
        BYTE bReserved;
        DWORD dwLayerMask;
        DWORD dwVisibleMask;
        DWORD dwDamageMask;
    } PIXELFORMATDESCRIPTOR;
    
    // Define Windows flags for pixel format
    #define PFD_DRAW_TO_WINDOW 0x00000004
    #define PFD_SUPPORT_OPENGL 0x00000020
    #define PFD_DOUBLEBUFFER 0x00000001
    #define PFD_TYPE_RGBA 0
    #define PFD_MAIN_PLANE 0
#endif

// Define GLX constants and types for Linux
#ifndef PLATFORM_WINDOWS
    // GLX constants
    #define GLX_RGBA 4
    #define GLX_DEPTH_SIZE 12
    #define GLX_DOUBLEBUFFER 5
    #define None 0L

    // GLX types
    typedef struct __GLXcontextRec *GLXContext;
    typedef struct __GLXFBConfigRec *GLXFBConfig;
    
    // XVisualInfo structure
    typedef struct {
        void* visual;
        int visualid;
        int screen;
        int depth;
        int c_class;
        unsigned long red_mask;
        unsigned long green_mask;
        unsigned long blue_mask;
        int colormap_size;
        int bits_per_rgb;
    } XVisualInfo;
    
    // GLX function stubs
    inline XVisualInfo* glXChooseVisual(void* display, int screen, int* attribList) { return nullptr; }
    inline GLXContext glXCreateContext(void* display, XVisualInfo* vis, GLXContext shareList, int direct) { return nullptr; }
    inline void glXMakeCurrent(void* display, unsigned long drawable, GLXContext ctx) {}
    inline void glXSwapBuffers(void* display, unsigned long drawable) {}
    inline void glXDestroyContext(void* display, GLXContext ctx) {}
#endif

// Define OpenGL shader constants
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84

// Define OpenGL texture constants
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE_2D 0x0DE1

// Define OpenGL viewport constants
#define GL_VIEWPORT 0x0BA2

// Define OpenGL pixel format constants
#define GL_RGBA 0x1908
#define GL_UNSIGNED_BYTE 0x1401

// Define OpenGL error constants
#define GL_NO_ERROR 0
