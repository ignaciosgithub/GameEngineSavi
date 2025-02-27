#include "AudioSystem.h"
#include "AudioSource.h"
#include "AudioClip.h"
#include <iostream>
#include <algorithm>

// Initialize static instance pointer
AudioSystem* AudioSystem::instance = nullptr;

AudioSystem::AudioSystem() : masterVolume(1.0f), initialized(false) {
}

AudioSystem::~AudioSystem() {
    Shutdown();
}

AudioSystem& AudioSystem::GetInstance() {
    if (!instance) {
        instance = new AudioSystem();
    }
    return *instance;
}

bool AudioSystem::Initialize() {
    if (initialized) {
        return true;
    }
    
#if AUDIO_ENABLED
    // Initialize SDL audio subsystem
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "Failed to initialize SDL audio: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        return false;
    }
    
    // Set number of channels (default is 8, we'll use 16)
    Mix_AllocateChannels(16);
#endif
    
    initialized = true;
    return true;
}

void AudioSystem::Shutdown() {
    if (!initialized) {
        return;
    }
    
    // Stop all active sources
    for (auto source : activeSources) {
        if (source) {
            source->Stop();
        }
    }
    activeSources.clear();
    
    // Free all cached clips
    for (auto& pair : clipCache) {
        if (pair.second) {
            pair.second->Unload();
        }
    }
    clipCache.clear();
    
#if AUDIO_ENABLED
    // Close SDL_mixer
    Mix_CloseAudio();
    
    // Quit SDL audio subsystem
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
#endif
    
    initialized = false;
}

void AudioSystem::Update() {
    if (!initialized) {
        return;
    }
    
    // Update all active sources
    for (auto it = activeSources.begin(); it != activeSources.end();) {
        if (*it && (*it)->IsPlaying()) {
            (*it)->Update(0.0f); // Pass 0.0f as deltaTime since we're not tracking time in the audio system
            ++it;
        } else {
            // Remove inactive sources
            it = activeSources.erase(it);
        }
    }
    
    // Adjust volume for multiple sources
    AdjustVolumeForMultipleSources();
}

std::shared_ptr<AudioClip> AudioSystem::LoadClip(const std::string& path) {
    if (!initialized) {
        if (!Initialize()) {
            std::cerr << "Failed to initialize audio system" << std::endl;
            return nullptr;
        }
    }
    
    // Check if clip is already cached
    auto it = clipCache.find(path);
    if (it != clipCache.end()) {
        return it->second;
    }
    
    // Create new clip
    auto clip = std::make_shared<AudioClip>(path);
    
#if AUDIO_ENABLED
    if (!clip->Load()) {
        std::cerr << "Failed to load audio clip: " << path << std::endl;
        return nullptr;
    }
#endif
    
    // Cache clip
    clipCache[path] = clip;
    
    return clip;
}

void AudioSystem::RegisterSource(AudioSource* source) {
    if (!source || !initialized) {
        return;
    }
    
    // Check if source is already registered
    if (IsSourceRegistered(source)) {
        return;
    }
    
    // Add source to active sources
    activeSources.push_back(source);
    
    // Adjust volume for multiple sources
    AdjustVolumeForMultipleSources();
}

void AudioSystem::UnregisterSource(AudioSource* source) {
    if (!source || !initialized) {
        return;
    }
    
    // Remove source from active sources
    auto it = std::find(activeSources.begin(), activeSources.end(), source);
    if (it != activeSources.end()) {
        activeSources.erase(it);
    }
    
    // Adjust volume for multiple sources
    AdjustVolumeForMultipleSources();
}

void AudioSystem::SetMasterVolume(float volume) {
    // Clamp volume between 0 and 1
    masterVolume = (volume < 0.0f) ? 0.0f : ((volume > 1.0f) ? 1.0f : volume);
    
    // Update all active sources
    AdjustVolumeForMultipleSources();
}

bool AudioSystem::IsSourceRegistered(AudioSource* source) const {
    if (!source) {
        return false;
    }
    
    return std::find(activeSources.begin(), activeSources.end(), source) != activeSources.end();
}

void AudioSystem::AdjustVolumeForMultipleSources() {
    if (activeSources.empty()) {
        return;
    }
    
    // Calculate volume scaling factor based on number of active sources
    // This ensures that the total volume doesn't increase with more sources
    float scaleFactor = 1.0f;
    
    // If we have multiple sources, scale down the volume
    if (activeSources.size() > 1) {
        // Use a logarithmic scaling to avoid too much reduction with many sources
        // This formula ensures that the perceived total volume remains relatively constant
        scaleFactor = 1.0f / std::sqrt(static_cast<float>(activeSources.size()));
    }
    
#if AUDIO_ENABLED
    // Apply master volume and scaling factor to all active sources
    for (auto source : activeSources) {
        if (source && source->GetChannel() != -1) {
            float sourceVolume = source->GetVolume();
            float adjustedVolume = sourceVolume * masterVolume * scaleFactor;
            
            // Convert from 0.0-1.0 to 0-128 (SDL_mixer volume range)
            int sdlVolume = static_cast<int>(adjustedVolume * 128.0f);
            Mix_Volume(source->GetChannel(), sdlVolume);
        }
    }
#endif
}
