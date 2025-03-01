#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "AudioPlatform.h"
#include "../Vector3.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class AudioClip;
class AudioSource;

/**
 * Audio system implementation using SDL_mixer.
 * Note: SDL_mixer does not support spatial audio features like
 * Doppler effect or 3D positional audio. Position and velocity
 * values are stored but do not affect audio playback.
 */

class AudioSystem {
private:
    static AudioSystem* instance;
    std::unordered_map<std::string, std::shared_ptr<AudioClip>> clipCache;
    std::vector<AudioSource*> activeSources;
    float masterVolume;
    bool initialized;
    Vector3 listenerPosition;
    Vector3 listenerVelocity;
    
    AudioSystem();
    ~AudioSystem();

public:
    static AudioSystem& GetInstance();
    
    bool Initialize();
    void Shutdown();
    void Update();
    
    std::shared_ptr<AudioClip> LoadClip(const std::string& path);
    void RegisterSource(AudioSource* source);
    void UnregisterSource(AudioSource* source);
    
    void SetMasterVolume(float volume);
    float GetMasterVolume() const { return masterVolume; }
    
    // Check if a source is already registered
    bool IsSourceRegistered(AudioSource* source) const;
    
    // Get the number of active sources
    size_t GetActiveSourceCount() const { return activeSources.size(); }
    
    // Spatial audio support
    void SetListenerPosition(const Vector3& position);
    void SetListenerVelocity(const Vector3& velocity);
    Vector3 GetListenerPosition() const;
    Vector3 GetListenerVelocity() const;
    
    // Adjust volume for multiple sources to prevent volume increase
    void AdjustVolumeForMultipleSources();
};

#endif // AUDIO_SYSTEM_H
