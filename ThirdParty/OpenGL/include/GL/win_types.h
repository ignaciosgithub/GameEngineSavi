#ifndef WIN_TYPES_H
#define WIN_TYPES_H

#ifdef PLATFORM_WINDOWS
// Windows-specific types and macros
#include <windows.h>
#else
// Define Windows types for Linux compilation
typedef void* HWND;
typedef void* HDC;
typedef void* HGLRC;
typedef void* HINSTANCE;
typedef void* HINSTANCE;
typedef char* LPSTR;
typedef unsigned int UINT;
typedef unsigned long WPARAM;
typedef long LPARAM;
typedef int BOOL;
typedef int LRESULT;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef void* LPVOID;
typedef void* HANDLE;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBRUSH;
typedef void* HMENU;
typedef struct tagWNDCLASS {
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
typedef struct tagMSG {
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    int    pt_x;
    int    pt_y;
} MSG;
typedef struct tagPIXELFORMATDESCRIPTOR {
    WORD  nSize;
    WORD  nVersion;
    DWORD dwFlags;
    BYTE  iPixelType;
    BYTE  cColorBits;
    BYTE  cRedBits;
    BYTE  cRedShift;
    BYTE  cGreenBits;
    BYTE  cGreenShift;
    BYTE  cBlueBits;
    BYTE  cBlueShift;
    BYTE  cAlphaBits;
    BYTE  cAlphaShift;
    BYTE  cAccumBits;
    BYTE  cAccumRedBits;
    BYTE  cAccumGreenBits;
    BYTE  cAccumBlueBits;
    BYTE  cAccumAlphaBits;
    BYTE  cDepthBits;
    BYTE  cStencilBits;
    BYTE  cAuxBuffers;
    BYTE  iLayerType;
    BYTE  bReserved;
    DWORD dwLayerMask;
    DWORD dwVisibleMask;
    DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR;

// Windows-specific macros
#define CALLBACK
#define WINAPI
#define WS_CAPTION        0x00C00000L
#define WS_POPUPWINDOW    0x00800000L
#define WS_VISIBLE        0x10000000L
#define CS_OWNDC          0x0020
#define PM_REMOVE         0x0001
#define WM_QUIT           0x0012
#define WM_CREATE         0x0001
#define WM_CLOSE          0x0010
#define WM_DESTROY        0x0002
#define WM_KEYDOWN        0x0100
#define WM_LBUTTONDOWN    0x0201
#define WM_MOUSEMOVE      0x0200
#define VK_ESCAPE         0x1B
#define IDI_APPLICATION   ((void*)32512)
#define IDC_ARROW         ((void*)32512)
#define BLACK_BRUSH       4
#define PFD_DRAW_TO_WINDOW 0x00000004
#define PFD_SUPPORT_OPENGL 0x00000020
#define PFD_DOUBLEBUFFER  0x00000001
#define PFD_TYPE_RGBA     0
#define PFD_MAIN_PLANE    0

// Windows-specific functions (stubs for Linux)
inline HWND CreateWindow(const char* className, const char* windowName, DWORD style,
                        int x, int y, int width, int height,
                        HWND parent, HMENU menu, HINSTANCE instance, LPVOID param) { return nullptr; }
inline HICON LoadIcon(HINSTANCE instance, const char* iconName) { return nullptr; }
inline HCURSOR LoadCursor(HINSTANCE instance, const char* cursorName) { return nullptr; }
inline HBRUSH GetStockObject(int object) { return nullptr; }
inline int RegisterClass(const WNDCLASS* wc) { return 0; }
inline BOOL PeekMessage(MSG* msg, HWND hwnd, UINT msgFilterMin, UINT msgFilterMax, UINT removeMsg) { return 0; }
inline BOOL TranslateMessage(const MSG* msg) { return 0; }
inline LRESULT DispatchMessage(const MSG* msg) { return 0; }
inline BOOL SwapBuffers(HDC hdc) { return 0; }
inline void DestroyWindow(HWND hwnd) {}
inline HDC GetDC(HWND hwnd) { return nullptr; }
inline int ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR* ppfd) { return 0; }
inline BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR* ppfd) { return 0; }
inline HGLRC wglCreateContext(HDC hdc) { return nullptr; }
inline BOOL wglMakeCurrent(HDC hdc, HGLRC hglrc) { return 0; }
inline BOOL wglDeleteContext(HGLRC hglrc) { return 0; }
inline int ReleaseDC(HWND hwnd, HDC hdc) { return 0; }
inline LRESULT DefWindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) { return 0; }
inline void PostQuitMessage(int exitCode) {}
inline WORD LOWORD(DWORD value) { return (WORD)(value & 0xFFFF); }
inline WORD HIWORD(DWORD value) { return (WORD)((value >> 16) & 0xFFFF); }
inline void ZeroMemory(void* dest, size_t length) { memset(dest, 0, length); }
#endif

#endif // WIN_TYPES_H
