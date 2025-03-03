#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include "EngineCondition.h"
#include "Scene_includes.h"
#include "GameObject_extensions_fixed.h"

void Scene::Initialize() {
    // Initialize time
    time = std::unique_ptr<TimeManager>(new TimeManager());
    
    // Initialize physics system if not already set
    if (!physicsSystem) {
        physicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
    }
    
    // Initialize camera manager
    cameraManager = std::unique_ptr<CameraManager>(new CameraManager());
    
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
        lightObj->position = Vector3(0, 5, 0);
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
            cubeObj->position = Vector3(0, 0, 0);
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
    time->Update();
    
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
            // Update components through public method
            gameObject->UpdateComponents(physicsTimeStep);
        }
        
        physicsAccumulator -= physicsTimeStep;
    }
    
    // Update game objects
    for (auto& gameObject : gameObjects) {
        // Update components through public method
        gameObject->UpdateComponents(deltaTime);
    }
    
    // Update cameras
    for (auto& camera : cameras) {
        // Update camera logic if needed
        camera->Update(deltaTime);
    }
    
    // Increment frame counter
    frameCount++;
}

void Scene::Render() {
    // Set up OpenGL state
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    
    // Set up background color (light gray)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    
    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Set up default light if we have one
    if (!pointLights.empty()) {
        const PointLight& defaultLight = pointLights[0];
        
        // Set light position
        GLfloat lightPos[] = {
            defaultLight.position.x,
            defaultLight.position.y,
            defaultLight.position.z,
            1.0f  // Positional light
        };
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
        
        // Set light color
        GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat lightDiffuse[] = {
            defaultLight.color.x * defaultLight.intensity,
            defaultLight.color.y * defaultLight.intensity,
            defaultLight.color.z * defaultLight.intensity,
            1.0f
        };
        GLfloat lightSpecular[] = {
            defaultLight.color.x * defaultLight.intensity,
            defaultLight.color.y * defaultLight.intensity,
            defaultLight.color.z * defaultLight.intensity,
            1.0f
        };
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
        
        // Set light attenuation based on range
        float attenuation = 1.0f / (defaultLight.range * defaultLight.range);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, attenuation);
    }
    
    // Render scene
    RenderScene();
    
    // Swap buffers
    // Note: This would be handled by the window system
}

void Scene::RenderScene() {
    // Initialize camera manager if not already created
    if (!cameraManager) {
        cameraManager = std::unique_ptr<CameraManager>(new CameraManager());
        
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
        std::cerr << "Warning: No active cameras available for rendering" << std::endl;
        return;
    }
    
    // Use camera manager to render from all active cameras
    if (cameraManager) {
        cameraManager->RenderFromAllCameras();
    }
    
    // For each active camera, render the scene
    std::vector<Camera*> activeCameras;
    
    // Add main camera if it exists and is enabled
    if (mainCamera && mainCamera->IsEnabled()) {
        activeCameras.push_back(mainCamera);
    }
    
    // Add cameras from camera manager if available
    if (cameraManager) {
        auto managerCameras = cameraManager->GetActiveCameras();
        activeCameras.insert(activeCameras.end(), managerCameras.begin(), managerCameras.end());
    }
    
    // If no active cameras, use the first camera in the list
    if (activeCameras.empty() && !cameras.empty()) {
        activeCameras.push_back(cameras[0].get());
    }
    
    // Render from each active camera
    for (auto camera : activeCameras) {
        if (!camera || !camera->IsEnabled()) {
            continue;
        }
        
        // Set viewport for this camera
        int viewportX = static_cast<int>(camera->viewportX * 800); // Assuming 800x600 window
        int viewportY = static_cast<int>(camera->viewportY * 600);
        int viewportWidth = static_cast<int>(camera->viewportWidth * 800);
        int viewportHeight = static_cast<int>(camera->viewportHeight * 600);
        
        glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
        
        // Set projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        // Get projection matrix from camera
        Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();
        glLoadMatrixf(&projectionMatrix.elements[0][0]);
        
        // Set modelview matrix
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Get view matrix from camera
        Matrix4x4 viewMatrix = camera->GetViewMatrix();
        glLoadMatrixf(&viewMatrix.elements[0][0]);
        
        // Debug output
        std::cout << "Rendering from camera at position: (" 
                  << camera->position.x << ", " 
                  << camera->position.y << ", " 
                  << camera->position.z << ")" << std::endl;
        
        // Render game objects
        for (auto& gameObject : gameObjects) {
            if (!gameObject) {
                continue;
            }
            
            // Save current matrix
            glPushMatrix();
            
            // Apply object transformation
            Vector3 position = gameObject->GetPosition();
            Vector3 rotation = gameObject->GetRotation();
            Vector3 scale = gameObject->GetScale();
            
            // Translate to object position
            glTranslatef(position.x, position.y, position.z);
            
            // Apply rotation
            glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
            glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
            glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
            
            // Apply scale
            glScalef(scale.x, scale.y, scale.z);
            
            // Render meshes
            for (auto& mesh : gameObject->meshes) {
                if (!mesh) {
                    continue;
                }
                
                // Set shader program if available
                ShaderProgram* program = mesh->GetShaderProgram();
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
            
            // Restore matrix
            glPopMatrix();
        }
        
        // Draw coordinate axes for debugging (only in editor mode)
        if (EngineCondition::IsInEditor()) {
            DrawDebugAxes();
        }
    }
}

void Scene::DrawDebugAxes() {
    // Disable lighting for drawing axes
    glDisable(GL_LIGHTING);
    
    // Draw X axis (red)
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glEnd();
    
    // Draw Y axis (green)
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();
    
    // Draw Z axis (blue)
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
    
    // Re-enable lighting
    glEnable(GL_LIGHTING);
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

void Scene::SetGlobalShaderUniforms(ShaderProgram* program) {
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

void Scene::UpdateLightUniforms(ShaderProgram* program) {
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

GameObject* Scene::FindGameObject(const std::string& name) const {
    for (const auto& gameObject : gameObjects) {
        if (gameObject->GetName() == name) {
            return gameObject.get();
        }
    }
    return nullptr;
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
        // Reset physics system
        // physicsSystem->Reset(); // Uncomment when Reset method is implemented
    }
    
    // Reset frame count
    frameCount = 0;
    
    // Reset physics accumulator
    physicsAccumulator = 0.0f;
}
