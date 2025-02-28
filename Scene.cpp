#include "Scene.h"
#include <iostream>
#include <algorithm>

void Scene::Initialize() {
    // Initialize time
    time = std::make_unique<EngineTime>();
    
    // Initialize physics system if not already set
    if (!physicsSystem) {
        physicsSystem = std::make_unique<PhysicsSystem>();
    }
    
    // Initialize camera manager
    cameraManager = std::make_unique<CameraManager>();
    
    // Set running flag
    isRunning = true;
    
    std::cout << "Scene initialized" << std::endl;
}

void Scene::Load() {
    // Load scene resources
    
    // Create a default point light if none exists
    if (pointLights.empty()) {
        PointLight defaultLight;
        defaultLight.position = Vector3(0, 5, 0);
        defaultLight.color = Vector3(1.0f, 1.0f, 1.0f);
        defaultLight.intensity = 1.0f;
        defaultLight.range = 20.0f;
        AddPointLight(defaultLight);
        
        // Create a game object for the light
        GameObject* lightObj = new GameObject("Default Light");
        lightObj->SetPosition(Vector3(0, 5, 0));
        AddGameObject(lightObj);
        
        std::cout << "Created default point light" << std::endl;
    }
    
    // Create a default cube if no game objects exist
    if (gameObjects.empty() || (gameObjects.size() == 1 && gameObjects[0]->GetName() == "Default Light")) {
        GameObject* cubeObj = new GameObject("Default Cube");
        
        // Load cube model
        Model* cubeModel = new Model();
        if (cubeModel->LoadFromFile("test_assets/cube.obj")) {
            cubeObj->AddMesh(cubeModel);
            cubeObj->SetPosition(Vector3(0, 0, 0));
            AddGameObject(cubeObj);
            std::cout << "Created default cube" << std::endl;
        } else {
            std::cerr << "Failed to load default cube model" << std::endl;
            delete cubeModel;
            delete cubeObj;
        }
    }
    
    // Set up a default camera if none exists
    if (!mainCamera) {
        Camera* defaultCamera = new Camera();
        defaultCamera->SetPosition(Vector3(0, 2, 5));
        defaultCamera->LookAt(Vector3(0, 0, 0));
        defaultCamera->fieldOfView = 45.0f;
        defaultCamera->SetEnabled(true);
        
        // Add camera to scene
        AddCamera(defaultCamera);
        SetMainCamera(defaultCamera);
        
        std::cout << "Created default camera" << std::endl;
    }
    
    std::cout << "Scene loaded" << std::endl;
}

void Scene::Run() {
    if (!isRunning) {
        Initialize();
    }
    
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    
    // Main loop
    while (isRunning) {
        // Calculate delta time
        float currentTime = time->DeltaTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
        
        // Cap delta time to avoid spiral of death
        if (deltaTime > 0.25f) {
            deltaTime = 0.25f;
        }
        
        // Update scene
        Update(deltaTime);
        
        // Render scene
        Render();
        
        // Frame rate limiting
        if (targetFPS > 0.0f) {
            float targetFrameTime = 1.0f / targetFPS;
            float sleepTime = targetFrameTime - deltaTime;
            if (sleepTime > 0.0f) {
                // Sleep to maintain target frame rate
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTime * 1000)));
            }
        }
    }
}

void Scene::Stop() {
    isRunning = false;
    std::cout << "Scene stopped" << std::endl;
}

void Scene::Update(float deltaTime) {
    // Update time
    time->Update(deltaTime);
    
    // Accumulate time for physics updates
    physicsAccumulator += deltaTime;
    
    // Fixed timestep physics updates
    while (physicsAccumulator >= physicsTimeStep) {
        // Update physics
        if (physicsSystem) {
            physicsSystem->Update(physicsTimeStep);
        }
        
        // Update game objects with fixed timestep
        for (auto& gameObject : gameObjects) {
            // Update components
            for (auto& component : gameObject->components) {
                component->FixedUpdate();
            }
        }
        
        physicsAccumulator -= physicsTimeStep;
    }
    
    // Update game objects
    for (auto& gameObject : gameObjects) {
        // Update components
        for (auto& component : gameObject->components) {
            component->Update(deltaTime);
        }
    }
    
    // Update cameras
    for (auto& camera : cameras) {
        // Update camera logic if needed
    }
    
    // Increment frame counter
    frameCount++;
}

void Scene::Render() {
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Render scene
    RenderScene();
    
    // Swap buffers
    // Note: This would be handled by the window system
}

void Scene::RenderScene() {
    // Initialize camera manager if not already created
    if (!cameraManager) {
        cameraManager = std::make_unique<CameraManager>();
        
        // Set main camera if available
        if (mainCamera) {
            cameraManager->SetMainCamera(mainCamera);
        }
        
        // Set minimap camera if available
        if (minimapCamera) {
            cameraManager->SetMinimapCamera(minimapCamera);
        }
    }
    
    // Skip rendering if no cameras are available
    if (!mainCamera && cameraManager->GetActiveCameras().empty()) {
        return;
    }
    
    // Use camera manager to render from all active cameras
    cameraManager->RenderFromAllCameras();
    
    // For each active camera, render the scene
    for (auto camera : cameraManager->GetActiveCameras()) {
        if (!camera || !camera->IsEnabled()) {
            continue;
        }
        
        // Get view and projection matrices from camera
        Matrix4x4 viewMatrix = camera->GetViewMatrix();
        Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();
        
        // Render game objects
        for (auto& gameObject : gameObjects) {
            // Render meshes
            for (auto& mesh : gameObject->meshes) {
                // Set shader program if available
                Shaders::ShaderProgram* program = mesh->GetShaderProgram();
                if (program) {
                    program->Use();
                    
                    // Set global uniforms
                    SetGlobalShaderUniforms(program);
                    
                    // Set model-specific uniforms
                    mesh->UpdateUniforms(viewMatrix, projectionMatrix);
                }
                
                // Render mesh
                mesh->Render(pointLights);
            }
        }
    }
}

void Scene::SetMainCamera(Camera* camera) {
    mainCamera = camera;
    
    // Update camera manager if it exists
    if (cameraManager) {
        cameraManager->SetMainCamera(camera);
    }
}

void Scene::SetMinimapCamera(Camera* camera) {
    minimapCamera = camera;
    
    // Update camera manager if it exists
    if (cameraManager) {
        cameraManager->SetMinimapCamera(camera);
    }
}

void Scene::EnableCamera(Camera* camera) {
    if (camera) {
        camera->SetEnabled(true);
    }
}

void Scene::DisableCamera(Camera* camera) {
    if (camera) {
        camera->SetEnabled(false);
    }
}

void Scene::SetResolutionChangeAllowed(bool allowed) {
    if (cameraManager) {
        cameraManager->SetResolutionChangeAllowed(allowed);
    }
}

void Scene::UpdateResolution(int width, int height) {
    if (cameraManager) {
        cameraManager->UpdateViewports(width, height);
    }
}

void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject) {
    gameObjects.push_back(std::move(gameObject));
}

void Scene::AddGameObject(GameObject* gameObject) {
    gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
}

void Scene::AddCamera(std::unique_ptr<Camera> camera) {
    cameras.push_back(std::move(camera));
}

void Scene::AddCamera(Camera* camera) {
    cameras.push_back(std::unique_ptr<Camera>(camera));
}

void Scene::AddPointLight(const PointLight& light) {
    pointLights.push_back(light);
}

void Scene::RemovePointLight(size_t index) {
    if (index < pointLights.size()) {
        pointLights.erase(pointLights.begin() + index);
    }
}

void Scene::SetGlobalShaderUniforms(Shaders::ShaderProgram* program) {
    if (!program) {
        return;
    }
    
    // Set time uniforms
    program->SetUniform("time", time->DeltaTime());
    program->SetUniform("deltaTime", time->DeltaTime());
    
    // Set camera uniforms
    if (mainCamera) {
        program->SetUniform("viewPos", mainCamera->GetPosition());
    }
    
    // Set light uniforms
    UpdateLightUniforms(program);
}

void Scene::UpdateLightUniforms(Shaders::ShaderProgram* program) {
    if (!program) {
        return;
    }
    
    // Set light count
    program->SetUniform("lightCount", static_cast<int>(pointLights.size()));
    
    // Set light properties
    for (size_t i = 0; i < pointLights.size(); i++) {
        std::string prefix = "lights[" + std::to_string(i) + "].";
        program->SetUniform(prefix + "position", pointLights[i].position);
        program->SetUniform(prefix + "color", pointLights[i].color);
        program->SetUniform(prefix + "intensity", pointLights[i].intensity);
        program->SetUniform(prefix + "range", pointLights[i].range);
    }
}

void Scene::SetPhysicsSystem(std::unique_ptr<PhysicsSystem> system) {
    physicsSystem = std::move(system);
}

PhysicsSystem* Scene::GetPhysicsSystem() const {
    return physicsSystem.get();
}
#include "Scene.h"
#include "SceneSerializer.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>

// Scene transition methods implementation

void Scene::UnloadScene() {
    // Stop all systems
    Stop();
    
    // Clear resources
    CleanupResources();
    
    isRunning = false;
    std::cout << "Scene unloaded: " << currentScenePath << std::endl;
}

void Scene::LoadScene(const std::string& scenePath) {
    // Unload current scene if any
    if (isRunning) {
        UnloadScene();
    }
    
    currentScenePath = scenePath;
    
    // Load base scene
    Load();
    
    // Create temp directory if it doesn't exist
    std::filesystem::create_directories("temp");
    
    // Load transferred objects if any
    std::ifstream manifest("temp/transfer_manifest.json");
    if (manifest.is_open()) {
        std::string objectPath;
        while (std::getline(manifest, objectPath)) {
            auto obj = SceneSerializer::LoadObjectFromJson(objectPath);
            if (obj) {
                AddGameObject(std::move(obj));
            }
            // Remove temporary file
            std::filesystem::remove(objectPath);
        }
        manifest.close();
        
        // Clear manifest
        std::filesystem::remove("temp/transfer_manifest.json");
    }
    
    Initialize();
    std::cout << "Scene loaded: " << scenePath << std::endl;
}

void Scene::TransferObject(GameObject* obj, const std::string& targetScenePath) {
    if (!obj) {
        std::cerr << "Error: Cannot transfer null GameObject" << std::endl;
        return;
    }
    
    // Create temp directory if it doesn't exist
    std::filesystem::create_directories("temp");
    
    // Save object to temporary file
    std::string tempPath = "temp/" + obj->GetName() + "_" + std::to_string(rand()) + ".json";
    SceneSerializer::SaveObjectToJson(obj, tempPath);
    
    // Add to transfer manifest
    std::ofstream manifest("temp/transfer_manifest.json", std::ios::app);
    if (!manifest.is_open()) {
        std::cerr << "Error: Could not open transfer manifest for writing" << std::endl;
        return;
    }
    
    manifest << tempPath << std::endl;
    manifest.close();
    
    std::cout << "Object '" << obj->GetName() << "' marked for transfer to scene: " << targetScenePath << std::endl;
}

void Scene::CleanupResources() {
    // Clear all game objects
    gameObjects.clear();
    
    // Clear all cameras
    cameras.clear();
    
    // Clear all point lights
    pointLights.clear();
    
    // Reset camera pointers
    mainCamera = nullptr;
    minimapCamera = nullptr;
    
    // Reset physics system
    if (physicsSystem) {
        physicsSystem->Reset();
    }
    
    // Reset frame count
    frameCount = 0;
    
    // Reset physics accumulator
    physicsAccumulator = 0.0f;
}
