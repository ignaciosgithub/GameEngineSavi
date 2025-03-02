#include "Profiler.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

// Simulate components for memory tracking
struct PointLight {
    Vector3 position;
    Vector3 color;
    float intensity;
    float range;
};

struct Cube {
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    std::vector<Vector3> vertices;
};

struct FreeCamera {
    Vector3 position;
    Vector3 rotation;
    Matrix4x4 viewMatrix;
    Matrix4x4 projectionMatrix;
};

// Simulate a default scene
void CreateDefaultScene() {
    // Create a point light
    PointLight* light = new PointLight();
    light->position = Vector3(0.0f, 5.0f, 0.0f);
    light->color = Vector3(1.0f, 1.0f, 1.0f);
    light->intensity = 1.0f;
    light->range = 10.0f;
    
    // Create a cube
    Cube* cube = new Cube();
    cube->position = Vector3(0.0f, 0.0f, 0.0f);
    cube->rotation = Vector3(0.0f, 0.0f, 0.0f);
    cube->scale = Vector3(1.0f, 1.0f, 1.0f);
    
    // Add vertices to the cube (simplified)
    for (int i = 0; i < 8; i++) {
        cube->vertices.push_back(Vector3(0.0f, 0.0f, 0.0f));
    }
    
    // Create a camera
    FreeCamera* camera = new FreeCamera();
    camera->position = Vector3(0.0f, 2.0f, 5.0f);
    camera->rotation = Vector3(0.0f, 0.0f, 0.0f);
    
    // Track memory for components
    Profiler::GetInstance().TrackComponentMemory("Point Light", sizeof(PointLight));
    Profiler::GetInstance().TrackComponentMemory("Cube", sizeof(Cube) + sizeof(Vector3) * cube->vertices.size());
    Profiler::GetInstance().TrackComponentMemory("Camera", sizeof(FreeCamera));
    
    // Simulate some processing time
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // Clean up
    delete light;
    delete cube;
    delete camera;
}

int main() {
    // Enable profiling
    Profiler::GetInstance().Enable();
    
    // Take initial memory snapshot
    Profiler::GetInstance().TakeMemorySnapshot("Initial");
    
    // Allocate some memory to simulate engine initialization
    std::vector<int> engineData(1000000, 0);
    
    // Take memory snapshot after initialization
    Profiler::GetInstance().TakeMemorySnapshot("After Engine Initialization");
    
    // Create default scene
    CreateDefaultScene();
    
    // Take memory snapshot after creating default scene
    Profiler::GetInstance().TakeMemorySnapshot("After Default Scene Creation");
    
    // Allocate more memory to simulate loading assets
    std::vector<float> assetData(2000000, 0.0f);
    
    // Take memory snapshot after loading assets
    Profiler::GetInstance().TakeMemorySnapshot("After Asset Loading");
    
    // Print memory report
    Profiler::GetInstance().PrintMemoryReport();
    
    // Save memory report to file
    Profiler::GetInstance().SaveMemoryReportToFile("memory_report.txt");
    
    std::cout << "Memory profiling complete. Press Enter to exit..." << std::endl;
    std::cin.get();
    
    return 0;
}
