#include "AudioSource.h"
#include "AudioSystem.h"
#include <iostream>
#include "AudioPlatform.h"

AudioSource::AudioSource() : clip(nullptr), channel(-1), volume(1.0f), loop(false), playing(false) {
}

AudioSource::~AudioSource() {
    Stop();
}

void AudioSource::Play() {
    if (!clip || !clip->IsLoaded()) {
        if (clip) {
            // Try to load the clip
            if (!clip->Load()) {
                std::cerr << "Failed to load audio clip: " << clip->GetPath() << std::endl;
                return;
            }
        } else {
            std::cerr << "No audio clip set for AudioSource" << std::endl;
            return;
        }
    }
    
#if AUDIO_ENABLED
    // Get a channel from the audio system
    channel = Mix_PlayChannel(-1, clip->GetChunk(), loop ? -1 : 0);
    
    if (channel == -1) {
        std::cerr << "Failed to play audio clip: " << Mix_GetError() << std::endl;
        return;
    }
    
    // Set volume
    SetVolume(volume);
#endif
    
    playing = true;
    
    // Register with audio system
    AudioSystem::GetInstance().RegisterSource(this);
}

void AudioSource::Stop() {
#if AUDIO_ENABLED
    if (channel != -1) {
        Mix_HaltChannel(channel);
        channel = -1;
    }
#endif
    playing = false;
    
    // Unregister from audio system
    AudioSystem::GetInstance().UnregisterSource(this);
}

void AudioSource::Pause() {
#if AUDIO_ENABLED
    if (channel != -1 && playing) {
        Mix_Pause(channel);
        playing = false;
    }
#endif
}

void AudioSource::Resume() {
#if AUDIO_ENABLED
    if (channel != -1 && !playing) {
        Mix_Resume(channel);
        playing = true;
    }
#endif
}

void AudioSource::SetClip(std::shared_ptr<AudioClip> clip) {
    // Stop current playback if any
    if (playing) {
        Stop();
    }
    
    this->clip = clip;
}

void AudioSource::SetVolume(float volume) {
    // Clamp volume between 0 and 1
    this->volume = (volume < 0.0f) ? 0.0f : ((volume > 1.0f) ? 1.0f : volume);
    
#if AUDIO_ENABLED
    // Set channel volume if playing
    if (channel != -1) {
        // Convert from 0.0-1.0 to 0-128 (SDL_mixer volume range)
        int sdlVolume = static_cast<int>(this->volume * 128.0f);
        Mix_Volume(channel, sdlVolume);
    }
#endif
}

void AudioSource::SetLoop(bool loop) {
    this->loop = loop;
    
    // If already playing, restart with new loop setting
    if (playing) {
        Stop();
        Play();
    }
}

void AudioSource::SetStopCondition(std::function<bool()> condition) {
    this->stopCondition = condition;
}

void AudioSource::Start() {
    // Called when the component is first initialized
    // Nothing to do here for now
}

void AudioSource::Update(float deltaTime) {
    // Check if we should stop based on condition
    if (playing && stopCondition && stopCondition()) {
        Stop();
    }
    
#if AUDIO_ENABLED
    // Check if the channel is still playing
    if (playing && channel != -1 && !Mix_Playing(channel)) {
        // Channel finished playing
        channel = -1;
        playing = false;
        
        // Unregister from audio system
        AudioSystem::GetInstance().UnregisterSource(this);
    }
#endif
}

void AudioSource::OnDestroy() {
    // Make sure to stop playback when component is destroyed
    Stop();
}
