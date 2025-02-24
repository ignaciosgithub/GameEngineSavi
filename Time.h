class Time
{
private:
    // Using high_resolution_clock as our game timer
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    float deltaTime; // Time between current frame and last frame

public:
    // Constructor initializes the timing variables
    Time() : deltaTime(0.0f)
    {
        lastFrameTime = std::chrono::high_resolution_clock::now();
    }

    // Update the timer every frame
    void Update();

    // Get the time it took to render the last frame
    float DeltaTime() const;
   
    // Since we're not dynamically allocating memory, we don't need a destructor in this case
};  