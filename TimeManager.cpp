#include "TimeManager.h"

// Initialize static instance
TimeManager* TimeManager::instance = nullptr;

void TimeManager::Update()
{
    // Get the current time
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    
    // Calculate the time difference in seconds
    std::chrono::duration<float> timeDiff = currentFrameTime - lastFrameTime;
    deltaTime = timeDiff.count();
    
    // Update lastFrameTime to the current time for the next Update call
    lastFrameTime = currentFrameTime;
}
