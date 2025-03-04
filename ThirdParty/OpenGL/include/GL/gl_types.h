#ifndef __gl_types_h_
#define __gl_types_h_

// OpenGL type definitions - these are common across all platforms
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

// Define OpenGL constants
#ifndef GL_FALSE
    #define GL_FALSE 0
#endif

#ifndef GL_TRUE
    #define GL_TRUE 1
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

// Platform-specific code for Linux only
#ifndef _WIN32
    // On non-Windows platforms, define Windows types for cross-platform compatibility
    #include <cstring> // For memset
    
    // Define TRUE and FALSE for cross-platform compatibility
    #ifndef TRUE
        #define TRUE 1
    #endif

    #ifndef FALSE
        #define FALSE 0
    #endif
    
    // Basic Windows types needed for cross-platform compatibility
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
    
    // GLX constants and types for Linux
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
    inline XVisualInfo* glXChooseVisual(void*, int, int*) { return nullptr; }
    inline GLXContext glXCreateContext(void*, XVisualInfo*, GLXContext, int) { return nullptr; }
    inline void glXMakeCurrent(void*, unsigned long, GLXContext) {}
    inline void glXSwapBuffers(void*, unsigned long) {}
    inline void glXDestroyContext(void*, GLXContext) {}
#endif

#endif /* __gl_types_h_ */
