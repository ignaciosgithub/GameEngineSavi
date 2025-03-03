#include "TimeManager.h"

// Initialize static instance
TimeManager* TimeManager::instance = nullptr;

// Constructor
TimeManager::TimeManager() {
    // Initialize time values
    startTime = std::chrono::high_resolution_clock::now();
    lastFrameTime = startTime;
    deltaTime = 0.0f;
    totalTime = 0.0f;
    timeScale = 1.0f;
    fixedDeltaTime = 1.0f / 60.0f; // Default to 60 Hz for physics
    
    // Set instance
    if (!instance) {
        instance = this;
    }
}

// Destructor
TimeManager::~TimeManager() {
    if (instance == this) {
        instance = nullptr;
    }
}

void TimeManager::Update() {
    // Get the current time
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    
    // Calculate the time difference in seconds
    std::chrono::duration<float> timeDiff = currentFrameTime - lastFrameTime;
    deltaTime = timeDiff.count() * timeScale;
    
    // Update total time
    std::chrono::duration<float> totalTimeDiff = currentFrameTime - startTime;
    totalTime = totalTimeDiff.count();
    
    // Update lastFrameTime to the current time for the next Update call
    lastFrameTime = currentFrameTime;
}

// Get delta time
float TimeManager::DeltaTime() const {
    return deltaTime;
}

// Get total time since start
float TimeManager::GetTime() const {
    return totalTime;
}

// Reset time values
void TimeManager::Reset() {
    startTime = std::chrono::high_resolution_clock::now();
    lastFrameTime = startTime;
    totalTime = 0.0f;
    deltaTime = 0.0f;
}
