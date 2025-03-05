#pragma once

class TimeManager {
public:
    TimeManager();
    ~TimeManager();
    
    void Update();
    void Reset();
    float GetTime() const;
    float DeltaTime() const;
};
