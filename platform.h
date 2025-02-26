#ifndef PLATFORM_H
#define PLATFORM_H

// Platform detection
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
    #ifndef PLATFORM_WINDOWS
    #define PLATFORM_WINDOWS
    #endif
    
    // Windows-specific includes
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    
    // Windows-specific socket type definitions
    typedef SOCKET SocketHandle;
    #define INVALID_SOCKET_HANDLE INVALID_SOCKET
    #define SOCKET_ERROR_CODE SOCKET_ERROR
    
#elif defined(__linux__) || defined(__unix__)
    #ifndef PLATFORM_LINUX
    #define PLATFORM_LINUX
    #endif
    
    // Linux-specific includes
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    
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
