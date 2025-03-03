#include "../Scene.h"
#include "../GameObject.h"
#include "../PhysicsSystem.h"
#include "../RigidBody.h"
#include "../Vector3.h"
#include "../TimeManager.h"
#include "../Model.h"
#include "../Camera.h"
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

// Performance test configuration
const int NUM_OBJECTS = 1000;  // Number of physical objects to create
const float TEST_DURATION = 10.0f;  // Test duration in seconds
const float REPORT_INTERVAL = 1.0f;  // How often to report FPS (in seconds)

class PerformanceTest {
private:
    Scene scene;
    std::unique_ptr<PhysicsSystem> physicsSystem;
    std::vector<std::unique_ptr<GameObject>> objects;
    float elapsedTime = 0.0f;
    float lastReportTime = 0.0f;
    int frameCount = 0;
    std::vector<float> fpsReadings;

public:
    PerformanceTest() {
        // Initialize physics system with realistic settings
        physicsSystem = std::make_unique<PhysicsSystem>();
        physicsSystem->SetGravity(Vector3(0, -9.81f, 0));
        physicsSystem->SetFixedTimeStep(0.016f);  // ~60 updates per second
        
        // Set up the scene
        scene.SetPhysicsSystem(std::move(physicsSystem));
        scene.Initialize();
        
        // Create a camera
        auto camera = std::make_unique<Camera>(
            Vector3(0, 10, -20),  // Position
            Vector3(0, -0.5f, 1),  // Look direction
            60.0f  // Field of view
        );
        scene.SetMainCamera(camera.get());
        scene.AddCamera(std::move(camera));
        
        // Create a ground plane
        auto ground = std::make_unique<GameObject>();
        ground->position = Vector3(0, -1, 0);
        ground->scale = Vector3(50, 1, 50);
        ground->AddComponent<RigidBody>(false);  // Static rigid body
        scene.AddGameObject(std::move(ground));
        
        // Create physical objects
        std::cout << "Creating " << NUM_OBJECTS << " physical objects..." << std::endl;
        for (int i = 0; i < NUM_OBJECTS; i++) {
            auto obj = std::make_unique<GameObject>();
            
            // Randomize position
            float x = (rand() % 40) - 20.0f;
            float y = (rand() % 20) + 5.0f;
            float z = (rand() % 40) - 20.0f;
            obj->position = Vector3(x, y, z);
            
            // Randomize scale (small variations)
            float scale = 0.5f + (rand() % 100) / 100.0f;
            obj->scale = Vector3(scale, scale, scale);
            
            // Add physics components
            auto rb = obj->AddComponent<RigidBody>(true);  // Dynamic rigid body
            rb->SetMass(1.0f);
            rb->SetFriction(0.5f);
            rb->SetRestitution(0.3f);
            
            // Store the object
            objects.push_back(std::move(obj));
        }
        
        // Add objects to scene
        for (auto& obj : objects) {
            scene.AddGameObject(std::move(obj));
        }
        
        std::cout << "Performance test initialized with " << NUM_OBJECTS << " objects" << std::endl;
        std::cout << "Running test for " << TEST_DURATION << " seconds..." << std::endl;
        std::cout << "Target framerate: 60+ FPS" << std::endl;
    }
    
    void Run() {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        while (elapsedTime < TEST_DURATION) {
            // Calculate delta time
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - startTime).count() - elapsedTime;
            elapsedTime = std::chrono::duration<float>(currentTime - startTime).count();
            
            // Update scene
            scene.Update(deltaTime);
            
            // Render scene (simulate rendering by sleeping a small amount)
            scene.Render();
            
            // Count frames
            frameCount++;
            
            // Report FPS at intervals
            if (elapsedTime - lastReportTime >= REPORT_INTERVAL) {
                float fps = frameCount / (elapsedTime - lastReportTime);
                fpsReadings.push_back(fps);
                std::cout << "Time: " << std::fixed << std::setprecision(1) << elapsedTime 
                          << "s, FPS: " << std::fixed << std::setprecision(1) << fps << std::endl;
                lastReportTime = elapsedTime;
                frameCount = 0;
            }
        }
        
        // Report results
        ReportResults();
    }
    
    void ReportResults() {
        std::cout << "\n===== Performance Test Results =====" << std::endl;
        std::cout << "Objects: " << NUM_OBJECTS << std::endl;
        std::cout << "Test duration: " << TEST_DURATION << " seconds" << std::endl;
        
        // Calculate average FPS
        float totalFps = 0.0f;
        float minFps = fpsReadings[0];
        float maxFps = fpsReadings[0];
        
        for (float fps : fpsReadings) {
            totalFps += fps;
            minFps = std::min(minFps, fps);
            maxFps = std::max(maxFps, fps);
        }
        
        float avgFps = totalFps / fpsReadings.size();
        
        std::cout << "Average FPS: " << std::fixed << std::setprecision(1) << avgFps << std::endl;
        std::cout << "Minimum FPS: " << std::fixed << std::setprecision(1) << minFps << std::endl;
        std::cout << "Maximum FPS: " << std::fixed << std::setprecision(1) << maxFps << std::endl;
        
        // Performance assessment
        std::string assessment;
        if (minFps >= 60.0f) {
            assessment = "EXCELLENT - All frames above 60 FPS";
        } else if (avgFps >= 60.0f) {
            assessment = "GOOD - Average above 60 FPS";
        } else if (avgFps >= 30.0f) {
            assessment = "ACCEPTABLE - Average above 30 FPS";
        } else {
            assessment = "POOR - Average below 30 FPS";
        }
        
        std::cout << "Performance assessment: " << assessment << std::endl;
        std::cout << "===================================" << std::endl;
    }
};

int main() {
    PerformanceTest test;
    test.Run();
    return 0;
}
