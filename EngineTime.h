#ifndef TIME_MANAGER_H
#define TIME_MANAGER_H

#include <chrono>
#include <ctime> // Add explicit include for time.h

// Renamed from Time to TimeManager to avoid conflict with ctime in Windows
class TimeManager
{
private:
    // Using high_resolution_clock as our game timer
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime; // Time between current frame and last frame

public:
    // Constructor initializes the timing variables
    TimeManager() : deltaTime(0.0f)
    {
        lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    // Update the timer every frame
    void Update();

    // Get the time it took to render the last frame
    float DeltaTime() const { return deltaTime; }
   
    // Since we're not dynamically allocating memory, we don't need a destructor in this case
};

#endif // TIME_MANAGER_H
