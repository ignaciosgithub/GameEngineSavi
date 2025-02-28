#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include "HeadlessGameObject.h"
#include "../EngineTime.h"
#include "HeadlessPhysicsSystem.h"

class HeadlessScene {
private:
    std::vector<std::unique_ptr<HeadlessGameObject>> gameObjects;
    std::unique_ptr<EngineTime> time;
    std::unique_ptr<HeadlessPhysicsSystem> physicsSystem;
    std::atomic<bool> isRunning;
    unsigned int frameCount = 0;
    float physicsTimeStep = 1.0f/60.0f; // Fixed physics timestep (60Hz)
    float physicsAccumulator = 0.0f;    // Accumulator for physics updates

public:
    HeadlessScene() : isRunning(false) {}

    void Initialize() {
        time = std::make_unique<EngineTime>();
        physicsSystem = std::make_unique<HeadlessPhysicsSystem>();
        isRunning = true;
    }

    void Update(float deltaTime) {
        // Update physics with fixed timestep
        physicsAccumulator += deltaTime;
        
        int physicsSteps = 0;
        while (physicsAccumulator >= physicsTimeStep && physicsSteps < 3) {
            if (physicsSystem) {
                physicsSystem->Update(physicsTimeStep);
            }
            physicsAccumulator -= physicsTimeStep;
            physicsSteps++;
        }
        
        // If we're falling behind, discard accumulated time
        if (physicsAccumulator > physicsTimeStep * 3) {
            physicsAccumulator = physicsTimeStep;
        }
        
        frameCount++;
    }

    void AddGameObject(std::unique_ptr<HeadlessGameObject> gameObject) {
        gameObjects.push_back(std::move(gameObject));
    }

    HeadlessPhysicsSystem* GetPhysicsSystem() const { return physicsSystem.get(); }
    unsigned int GetFrameCount() const { return frameCount; }
};
