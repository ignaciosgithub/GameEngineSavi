#include "HeadlessScene.h"
#include "HeadlessGameObject.h"
#include "HeadlessPhysicsSystem.h"
#include "HeadlessRigidBody.h"
#include "RenderingSimulator.h"
#include "../Vector3.h"
#include "../TimeManager.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <random>
#include <fstream>

// Performance test configuration
const int TEST_DURATION = 10.0f;  // Test duration in seconds
const float REPORT_INTERVAL = 1.0f;  // How often to report FPS (in seconds)

class HeadlessPerformanceTest {
private:
    std::unique_ptr<HeadlessScene> scene;
    std::vector<HeadlessGameObject*> objects;
    std::unique_ptr<RenderingSimulator> renderingSimulator;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point lastFrameTime;
    std::chrono::high_resolution_clock::time_point lastReportTime;
    int frameCount = 0;
    std::vector<float> fpsReadings;
    float minFps = std::numeric_limits<float>::max();
    float maxFps = 0.0f;
    float totalFps = 0.0f;
    int numObjects;
    std::ofstream logFile;

public:
    HeadlessPerformanceTest(int objectCount) : numObjects(objectCount) {
        // Open log file
        std::string filename = "performance_test_" + std::to_string(numObjects) + ".log";
        logFile.open(filename);
        if (!logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
        
        // Set up scene
        scene = std::make_unique<HeadlessScene>();
        scene->Initialize();
        
        // Configure physics
        auto physics = scene->GetPhysicsSystem();
        physics->SetGravity(-9.81f);
        physics->SetFixedTimeStep(1.0f/60.0f);
        
        // Create rendering simulator
        renderingSimulator = std::make_unique<RenderingSimulator>();
        renderingSimulator->GenerateGeometry(numObjects);
        
        // Create ground plane
        auto ground = new HeadlessGameObject();
        ground->position = Vector3(0, -1, 0);
        auto groundRb = new HeadlessRigidBody();
        groundRb->SetDynamic(false);
        ground->AddRigidBody(groundRb);
        scene->AddGameObject(std::unique_ptr<HeadlessGameObject>(ground));
        
        // Create physical objects
        std::cout << "Creating " << numObjects << " physical objects..." << std::endl;
        logFile << "Creating " << numObjects << " physical objects..." << std::endl;
        
        // Use a proper random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> xz_dist(-20.0f, 20.0f);
        std::uniform_real_distribution<float> y_dist(5.0f, 25.0f);
        std::uniform_real_distribution<float> mass_dist(0.5f, 5.0f);
        
        for (int i = 0; i < numObjects; i++) {
            auto obj = new HeadlessGameObject();
            
            // Random position
            obj->position = Vector3(xz_dist(gen), y_dist(gen), xz_dist(gen));
            
            // Add physics
            auto rb = new HeadlessRigidBody();
            rb->SetDynamic(true);
            rb->SetMass(mass_dist(gen)); // Random mass
            rb->EnableGravity(true);
            obj->AddRigidBody(rb);
            scene->GetPhysicsSystem()->AddBody(rb);
            
            scene->AddGameObject(std::unique_ptr<HeadlessGameObject>(obj));
            objects.push_back(obj);
        }
        
        std::cout << "Performance test initialized with " << numObjects << " objects" << std::endl;
        std::cout << "Running test for " << TEST_DURATION << " seconds..." << std::endl;
        std::cout << "Target framerate: 60+ FPS" << std::endl;
        
        logFile << "Performance test initialized with " << numObjects << " objects" << std::endl;
        logFile << "Running test for " << TEST_DURATION << " seconds..." << std::endl;
        logFile << "Target framerate: 60+ FPS" << std::endl;
    }
    
    ~HeadlessPerformanceTest() {
        // Scene will clean up automatically through RAII
        scene.reset();
        objects.clear();
        renderingSimulator.reset();
        
        if (logFile.is_open()) {
            logFile.close();
        }
    }
    
    void Run() {
        startTime = std::chrono::high_resolution_clock::now();
        lastFrameTime = startTime;
        lastReportTime = startTime;
        
        while (std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - startTime).count() < TEST_DURATION) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
            lastFrameTime = currentTime;
            
            // Update physics
            scene->Update(deltaTime);
            
            // Simulate rendering
            renderingSimulator->SimulateRenderPass(deltaTime);
            
            frameCount++;
            
            float elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
            float reportDuration = std::chrono::duration<float>(currentTime - lastReportTime).count();
            
            // Report FPS at intervals
            if (reportDuration >= REPORT_INTERVAL) {
                float fps = static_cast<float>(frameCount) / reportDuration;
                minFps = std::min(minFps, fps);
                maxFps = std::max(maxFps, fps);
                totalFps += fps;
                fpsReadings.push_back(fps);
                
                std::cout << "Time: " << std::fixed << std::setprecision(1) << elapsedTime 
                          << "s, FPS: " << fps << " (Min: " << minFps << ", Max: " << maxFps << ")" << std::endl;
                
                logFile << "Time: " << std::fixed << std::setprecision(1) << elapsedTime 
                        << "s, FPS: " << fps << " (Min: " << minFps << ", Max: " << maxFps << ")" << std::endl;
                
                lastReportTime = currentTime;
                frameCount = 0;
            }
        }
        
        ReportResults();
    }
    
    void ReportResults() {
        float avgFps = totalFps / fpsReadings.size();
        
        std::cout << "\n===== Performance Test Results =====" << std::endl;
        std::cout << "Objects: " << numObjects << std::endl;
        std::cout << "Test duration: " << TEST_DURATION << " seconds" << std::endl;
        std::cout << "Average FPS: " << std::fixed << std::setprecision(1) << avgFps << std::endl;
        std::cout << "Minimum FPS: " << minFps << std::endl;
        std::cout << "Maximum FPS: " << maxFps << std::endl;
        
        std::string assessment = minFps >= 60.0f ? "PASSED" : "FAILED";
        std::cout << "Performance assessment: " << assessment << std::endl;
        std::cout << "===================================" << std::endl;
        
        logFile << "\n===== Performance Test Results =====" << std::endl;
        logFile << "Objects: " << numObjects << std::endl;
        logFile << "Test duration: " << TEST_DURATION << " seconds" << std::endl;
        logFile << "Average FPS: " << std::fixed << std::setprecision(1) << avgFps << std::endl;
        logFile << "Minimum FPS: " << minFps << std::endl;
        logFile << "Maximum FPS: " << maxFps << std::endl;
        logFile << "Performance assessment: " << assessment << std::endl;
        logFile << "===================================" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    // Default object counts to test
    std::vector<int> objectCounts = {10, 100, 1000};
    
    // Allow command-line override
    if (argc > 1) {
        objectCounts.clear();
        for (int i = 1; i < argc; i++) {
            objectCounts.push_back(std::atoi(argv[i]));
        }
    }
    
    // Run tests for each object count
    for (int count : objectCounts) {
        std::cout << "\n\n========================================" << std::endl;
        std::cout << "STARTING PERFORMANCE TEST WITH " << count << " OBJECTS" << std::endl;
        std::cout << "========================================\n" << std::endl;
        
        HeadlessPerformanceTest test(count);
        test.Run();
    }
    
    return 0;
}
