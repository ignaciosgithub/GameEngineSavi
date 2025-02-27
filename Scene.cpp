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
