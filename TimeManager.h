#pragma once

#include <ctime>  // Include ctime before chrono to avoid circular dependencies
#include <chrono>

class TimeManager {
public:
    TimeManager();
    ~TimeManager();
    
    // Update time values
    void Update();
    
    // Get delta time between frames
    float DeltaTime() const;
    
    // Get total time since start
    float GetTime() const;
    
    // Get delta time (alias for compatibility)
    float GetDeltaTime() const { return DeltaTime(); }
    
    // Reset time values
    void Reset();
    
    // Time scale factor
    float timeScale;
    
    // Delta time between frames
    float deltaTime;
    
    // Fixed time step for physics
    float fixedDeltaTime;
    
private:
    // Time point of the last frame
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    
    // Time point of the start
    std::chrono::high_resolution_clock::time_point startTime;
    
    // Total time since start
    float totalTime;
    
    // Static instance
    static TimeManager* instance;
};
