#include "EngineTime.h"

void TimeManager::Update()
{
    auto currentTime = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<float> timeDiff = currentTime - lastFrameTime;
    
    deltaTime = timeDiff.count();
    
    lastFrameTime = currentTime;
}
