#ifndef AUDIO_SOURCE_H
#define AUDIO_SOURCE_H

#include "../MonoBehaviourLike.h"
#include "AudioClip.h"
#include <memory>
#include <functional>

class AudioSource : public MonoBehaviourLike {
private:
    std::shared_ptr<AudioClip> clip;
    int channel;
    float volume;
    bool loop;
    std::function<bool()> stopCondition;
    bool playing;

public:
    AudioSource();
    ~AudioSource();

    void Play();
    void Stop();
    void Pause();
    void Resume();
    
    void SetClip(std::shared_ptr<AudioClip> clip);
    void SetVolume(float volume); // 0.0 to 1.0
    void SetLoop(bool loop);
    void SetStopCondition(std::function<bool()> condition);
    
    bool IsPlaying() const { return playing; }
    bool IsLooping() const { return loop; }
    float GetVolume() const { return volume; }
    int GetChannel() const { return channel; }
    
    // MonoBehaviourLike overrides
    void Start() override;
    void Update(float deltaTime) override;
    void OnDestroy() override;
};

#endif // AUDIO_SOURCE_H
