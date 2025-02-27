#ifndef AUDIO_PLATFORM_H
#define AUDIO_PLATFORM_H

#include "../platform.h"

// Platform-specific includes for audio
#ifdef PLATFORM_WINDOWS
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_mixer.h>
#else
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_mixer.h>
#endif

// Platform-specific defines for audio
#ifdef PLATFORM_WINDOWS
    #define AUDIO_PATH_SEPARATOR "\\"
#else
    #define AUDIO_PATH_SEPARATOR "/"
#endif

// Define a macro to check if we're in a headless environment
#ifndef HEADLESS_ENVIRONMENT
    #define HEADLESS_ENVIRONMENT 0
#endif

// Define a macro for dummy audio in headless environments
#if HEADLESS_ENVIRONMENT
    #define AUDIO_ENABLED 0
#else
    #define AUDIO_ENABLED 1
#endif

#endif // AUDIO_PLATFORM_H
