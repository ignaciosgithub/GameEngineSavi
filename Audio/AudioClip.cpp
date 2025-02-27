#include "AudioClip.h"
#include <iostream>
#include "AudioPlatform.h"

AudioClip::AudioClip(const std::string& path) : chunk(nullptr), path(path), loaded(false) {
}

AudioClip::~AudioClip() {
    Unload();
}

bool AudioClip::Load() {
    // If already loaded, return true
    if (loaded) {
        return true;
    }
    
#if AUDIO_ENABLED
    // Load the audio file
    chunk = Mix_LoadWAV(path.c_str());
    if (!chunk) {
        std::cerr << "Failed to load audio clip: " << path << std::endl;
        std::cerr << "SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
#endif
    
    loaded = true;
    return true;
}

void AudioClip::Unload() {
#if AUDIO_ENABLED
    if (chunk) {
        Mix_FreeChunk(chunk);
        chunk = nullptr;
    }
#endif
    loaded = false;
}
