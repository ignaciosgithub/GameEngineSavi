#include"Vector3.h"
#include"Triangle.h"
#include"Matrix4x4.h"
#include"Raycast.h"
#include"PointLight.h"
#include"Camera.h"
#include"Face.h"
#include"Pyramid.h"
#include"Model.h"
#include"MonobEhaviourLike.h"
#include"GameObject.h"
#include"Prefab.h"
#include"Time.h"
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>



#include<vector>
#include<iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <chrono>
#include <memory>
#include <cmath>
#include <thread>
#include <atomic>

class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::vector<std::unique_ptr<Camera>> cameras;
    std::unique_ptr<Time> time;
    std::atomic<bool> isRunning;

public:
    Scene() : isRunning(false) {}

    void Load() {
        // Load a scene here (e.g., create GameObjects and Cameras)
        // For simplicity, this just initializes the Time class
        time.reset(new Time());//time = Time;
    }

    void Run() {
        // Initialize objects and components
        for (auto& gameObject : gameObjects) {
            for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
                mb->Awake();
                mb->Start();
            }
        }

        isRunning = true;

        while (isRunning) {
            time->Update(); // Update the Time class to get the deltaTime

            // Call Update() on all MonoBehaviourLike components in the scene
            for (auto& gameObject : gameObjects) {
                for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
                    mb->Update();
                }
            }

            RenderScene(); // Renders all objects in the scene

            std::this_thread::sleep_for(std::chrono::milliseconds(16)); // Simulate frame rate (60 FPS)
        }
    }

    void Stop() {
        // Call OnDestroy() on all MonoBehaviourLike components
        for (auto& gameObject : gameObjects) {
            for (auto& mb : gameObject->GetComponents<MonoBehaviourLike>()) {
                mb->OnDestroy();
            }
        }

        gameObjects.clear();
        cameras.clear();
        isRunning = false;
    }

    void RenderScene() {
        // Render each GameObject in the scene
        for (const auto& gameObject : gameObjects) {
            gameObject->Render(gameObject->lights);
        }

        // Render each Camera view in the scene
        for (const auto& camera : cameras) {
            //camera->Render();
        }
    }

    // Add GameObjects and Cameras to the scene
    void AddGameObject(std::unique_ptr<GameObject> gameObject) {
        gameObjects.emplace_back(std::move(gameObject));
    }
    void AddCamera(std::unique_ptr<Camera> camera) {
        cameras.emplace_back(std::move(camera));
    }
};