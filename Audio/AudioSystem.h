#ifndef AUDIO_SYSTEM_H
#define AUDIO_SYSTEM_H

#include "AudioPlatform.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class AudioClip;
class AudioSource;

class AudioSystem {
private:
    static AudioSystem* instance;
    std::unordered_map<std::string, std::shared_ptr<AudioClip>> clipCache;
    std::vector<AudioSource*> activeSources;
    float masterVolume;
    bool initialized;
    
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
    
    // Adjust volume for multiple sources to prevent volume increase
    void AdjustVolumeForMultipleSources();
};

#endif // AUDIO_SYSTEM_H
