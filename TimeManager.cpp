#include "TimeManager.h"
#include <iostream>

TimeManager::TimeManager() {
    std::cout << "TimeManager initialized" << std::endl;
}

TimeManager::~TimeManager() {
    std::cout << "TimeManager destroyed" << std::endl;
}

void TimeManager::Update() {
    // Stub implementation
}

void TimeManager::Reset() {
    // Stub implementation
}

float TimeManager::GetTime() const {
    return 0.0f; // Stub implementation
}

float TimeManager::DeltaTime() const {
    return 0.016f; // Stub implementation (60 FPS)
}

float TimeManager::GetDeltaTime() const {
    return DeltaTime(); // Call the existing DeltaTime method
}
