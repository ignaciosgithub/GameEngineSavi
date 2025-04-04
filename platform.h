#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
    #ifndef PLATFORM_WINDOWS
    #define PLATFORM_WINDOWS
    #endif
    
    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
    #define NOMINMAX
    #endif
    
    // Windows-specific includes
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <windowsx.h>
    #include "ThirdParty/OpenGL/include/GL/platform_gl.h"
    #include <psapi.h>  // For GetProcessMemoryInfo
    
    // Windows-specific library dependencies
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")
    #pragma comment(lib, "user32.lib")
    #pragma comment(lib, "gdi32.lib")
    
    // Windows-specific socket type definitions
    typedef SOCKET SocketHandle;
    #define INVALID_SOCKET_HANDLE INVALID_SOCKET
    #define SOCKET_ERROR_CODE SOCKET_ERROR
    
    // Windows-specific OpenGL types and macros
    #ifndef CALLBACK
    #define CALLBACK __stdcall
    #endif
    
    #ifndef WINAPI
    #define WINAPI __stdcall
    #endif
    
    #ifndef APIENTRY
    #define APIENTRY WINAPI
    #endif
    
    // Windows-specific message handling
    #ifndef GET_X_LPARAM
    #define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
    #endif
    
    #ifndef GET_Y_LPARAM
    #define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
    #endif
    
#elif defined(__linux__) || defined(__unix__)
    #ifndef PLATFORM_LINUX
    #define PLATFORM_LINUX
    #endif
    
    // Linux-specific includes
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>  // For sysconf
    #include <fcntl.h>
    #include <stdio.h>   // For FILE operations
    
    // Linux-specific socket type definitions
    typedef int SocketHandle;
    #define INVALID_SOCKET_HANDLE -1
    #define SOCKET_ERROR_CODE -1
    
#else
    #error "Unsupported platform"
#endif

// Common includes for all platforms
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <thread>

#endif // PLATFORM_H
