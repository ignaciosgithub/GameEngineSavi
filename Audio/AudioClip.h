#ifndef AUDIO_CLIP_H
#define AUDIO_CLIP_H

#include "AudioPlatform.h"
#include <string>

class AudioClip {
private:
    Mix_Chunk* chunk;
    std::string path;
    bool loaded;

public:
    AudioClip(const std::string& path);
    ~AudioClip();
    
    bool Load();
    void Unload();
    Mix_Chunk* GetChunk() const { return chunk; }
    bool IsLoaded() const { return loaded; }
    const std::string& GetPath() const { return path; }
};

#endif // AUDIO_CLIP_H
