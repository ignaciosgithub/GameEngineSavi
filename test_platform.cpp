#include "platform.h"
#include <iostream>

int main() {
    std::cout << "Testing platform.h compilation" << std::endl;
    
#ifdef PLATFORM_WINDOWS
    std::cout << "Compiled for Windows platform" << std::endl;
#elif defined(PLATFORM_LINUX)
    std::cout << "Compiled for Linux platform" << std::endl;
#else
    std::cout << "Unknown platform" << std::endl;
#endif

    return 0;
}
