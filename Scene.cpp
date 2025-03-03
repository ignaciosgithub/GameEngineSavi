#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include "EngineCondition.h"
#include "Scene_includes.h"
// GameObject extensions included via GameObject.h

void Scene::Initialize() {
    // Initialize time
    time = std::unique_ptr<TimeManager>(new TimeManager());

    // Initialize physics system if not alre   ady set
    if (!physicsSystem) {
        physicsSystem = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
    }

    // Initialize camera manager
    cameraManager = std::unique_ptr<CameraManager>(new CameraManager());

    // Set running flag
    isRunning = true;

    // Game objects are initialized via constructor

    std::cout << "Scene initialized" << std::endl;
}

void Scene::CreateDefaultObjects() {
    // Create a default point light if none exists
    bool hasPointLight = false;
    for (auto& gameObject : gameObjects) {
        if (!gameObject->lights.empty()) {
            hasPointLight = true;
            break;
        }
    }

    if (!hasPointLight) {
        // Create a point light
        PointLight light;
        light.position = Vector3(0, 5, 0);
        light.color = Vector3(1, 1, 1);
        light.intensity = 1.0f;
        light.range = 10.0f;

        // Create a game object for the light
        GameObject* lightObj = new GameObject("Default Light");
        lightObj->position = Vector3(0, 5, 0);
        AddGameObject(lightObj);

        std::cout << "Created default point light" << std::endl;
    }

    // Create a default cube if no game objects exist
    if (gameObjects.size() <= 1) {
        // Create a cube
        GameObject* cubeObj = new GameObject("Default Cube");

        // Load a cube model
        Model* cubeModel = new Model();
        if (cubeModel->LoadFromFile("test_assets/cube.obj")) {
            cubeObj->AddMesh(cubeModel);
            cubeObj->position = Vector3(0, 0, 0);
            AddGameObject(cubeObj);
            std::cout << "Created default cube" << std::endl;
        } else {
            delete cubeModel;
            delete cubeObj;
            std::cout << "Failed to load cube model" << std::endl;
        }
    }

    // Create a default camera if none exists
    if (!mainCamera) {
        // Create a camera
        Camera* camera = new Camera();
        camera->SetPosition(Vector3(0, 2, -5));
        // Use LookAt instead of SetTarget
        camera->LookAt(Vector3(0, 0, 0));
        // Use fieldOfView instead of SetFOV
        camera->fieldOfView = 60.0f;
        camera->SetNearPlane(0.1f);
        camera->SetFarPlane(1000.0f);

        // Set as main camera
        SetMainCamera(camera);

        std::cout << "Created default camera" << std::endl;
    }
}

void Scene::AddGameObject(GameObject* gameObject) {
    if (!gameObject) {
        return;
    }

    // Check if the game object is already in the scene
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it == gameObjects.end()) {
        // Add the game object to the scene
        gameObjects.push_back(gameObject);

        // Game object is already initialized via constructor

        std::cout << "Added game object: " << gameObject->GetName() << std::endl;
    }
}

void Scene::RemoveGameObject(GameObject* gameObject) {
    if (!gameObject) {
        return;
    }

    // Find the game object in the scene
    auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
    if (it != gameObjects.end()) {
        // Remove the game object from the scene
        gameObjects.erase(it);

        std::cout << "Removed game object: " << gameObject->GetName() << std::endl;
    }
}

void Scene::SetMainCamera(Camera* camera) {
    if (!camera) {
        return;
    }

    // Set the main camera
    mainCamera = camera;

    // Add the camera to the camera manager
    if (cameraManager) {
        cameraManager->SetMainCamera(camera);
    }

    std::cout << "Set main camera" << std::endl;
}

void Scene::SetGravity(const Vector3& gravity) {
    if (physicsSystem) {
        // Convert Vector3 to float (use y component)
        physicsSystem->SetGravity(gravity.y);
    }
}

Vector3 Scene::GetGravity() const {
    if (physicsSystem) {
        // Convert float to Vector3 (set y component)
        float gravityValue = physicsSystem->GetGravity();
        return Vector3(0, gravityValue, 0);
    }

    return Vector3(0, -9.81f, 0); // Default gravity
}

void Scene::SetPhysicsTimeStep(float timeStep) {
    physicsTimeStep = timeStep;
}

float Scene::GetPhysicsTimeStep() const {
    return physicsTimeStep;
}

void Scene::Update(float deltaTime) {
    // Update time
    time->Update();

    // Accumulate time for physics updates
    physicsAccumulator += deltaTime;

    // Update physics with fixed time step
    while (physicsAccumulator >= physicsTimeStep) {
        // Update physics system
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
    if (mainCamera) {
        mainCamera->Update(deltaTime);
    }

    // Update frame count
    frameCount++;
}

void Scene::Render() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the scene
    RenderScene();
}

void Scene::RenderGameObject(GameObject* gameObject, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Vector3& cameraPosition) {
    if (!gameObject) {
        return;
    }

    // Skip disabled game objects
    if (!gameObject->IsEnabled()) {
        return;
    }

    // Get the model matrix
    Matrix4x4 model = gameObject->GetModelMatrix();

    // Render meshes
    for (auto& mesh : gameObject->GetMeshes()) {
        if (mesh) {
            RenderMesh(mesh, model, viewMatrix, projectionMatrix, cameraPosition);
        }
    }

    // Render children
    for (auto& child : gameObject->GetChildren()) {
        if (child) {
            RenderGameObject(child, viewMatrix, projectionMatrix, cameraPosition);
        }
    }
}

void Scene::RenderScene() {
    // Initialize camera manager if not already created
    if (!cameraManager) {
        cameraManager = std::unique_ptr<CameraManager>(new CameraManager());

        // Set main camera if available
        if (mainCamera) {
            cameraManager->SetMainCamera(mainCamera);
        }
    }

    // Get active cameras
    std::vector<Camera*> cameras = cameraManager->GetActiveCameras();

    // Render from all cameras
    for (auto& camera : cameras) {
        if (!camera) {
            continue;
        }

        // Set viewport
        int viewportX = 0;
        int viewportY = 0;
        int viewportWidth = camera->GetViewportWidth();
        int viewportHeight = camera->GetViewportHeight();

        glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

        // Get view and projection matrices
        Matrix4x4 viewMatrix = camera->GetViewMatrix();
        Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();

        // Get camera position
        Vector3 cameraPosition = camera->GetPosition();

        // Render game objects
        for (auto& gameObject : gameObjects) {
            if (gameObject) {
                RenderGameObject(gameObject, viewMatrix, projectionMatrix, cameraPosition);
            }
        }

        // Draw coordinate axes for debugging (only in editor mode)
        if (EngineCondition::IsInEditor()) {
            DrawDebugAxes();
        }
    }
}

void Scene::RenderMesh(Model* mesh, const Matrix4x4& modelMatrix, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Vector3& cameraPosition) {
    if (!mesh) {
        return;
    }

    // Get all point lights in the scene
    std::vector<PointLight> pointLights;
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            for (auto& light : gameObject->lights) {
                pointLights.push_back(light);
            }
        }
    }

    // Render the mesh with the lights
    mesh->Render(pointLights);
}

void Scene::DrawDebugAxes() {
    // Draw coordinate axes for debugging
    glBegin(GL_LINES);

    // X axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);

    // Y axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);

    // Z axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);

    glEnd();
}

void Scene::SetGlobalShaderUniforms(ShaderProgram* program) {
    if (!program) {
        return;
    }

    // Set time uniforms
    if (time) {
        ShaderProgram::SetUniform("time", time->GetTime());
        ShaderProgram::SetUniform("deltaTime", time->GetDeltaTime());
    }

    // Set camera uniforms
    if (mainCamera) {
         program::SetUniform("viewMatrix", mainCamera->GetViewMatrix());
         program::SetUniform("projectionMatrix", mainCamera->GetProjectionMatrix());
         program::SetUniform("cameraPosition", mainCamera->GetPosition());
    }

    // Set light uniforms
    UpdateLightUniforms(program);
}

void Scene::UpdateLightUniforms(ShaderProgram* program) {
    if (!program) {
        return;
    }

    // Get all point lights in the scene
    std::vector<PointLight> pointLights;
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            for (auto& light : gameObject->lights) {
                pointLights.push_back(light);
            }
        }
    }

    // Set point light uniforms
    int pointLightCount = std::min((int)pointLights.size(), 8); // Limit to 8 point lights
     program::SetUniform("pointLightCount", pointLightCount);

    for (int i = 0; i < pointLightCount; i++) {
        std::string prefix = "pointLights[" + std::to_string(i) + "].";
         program::SetUniform(prefix + "position", pointLights[i].position);
         program::SetUniform(prefix + "color", pointLights[i].color);
         program::SetUniform(prefix + "intensity", pointLights[i].intensity);
         program::SetUniform(prefix + "range", pointLights[i].range);
    }
}

void Scene::Reset() {
    // Reset time
    if (time) {
        time->Reset();
    }

    // Reset physics system
    if (physicsSystem) {
        // Reset physics system
        // physicsSystem->Reset(); // Uncomment when Reset method is implemented
    }

    // Reset frame count
    frameCount = 0;

    // Reset physics accumulator
    physicsAccumulator = 0.0f;

    // Reset game objects
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            gameObject->Reset();
        }
    }
}

void Scene::Shutdown() {
    // Shutdown game objects
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            gameObject->Shutdown();
        }
    }

    // Clear game objects
    gameObjects.clear();

    // Reset main camera
    mainCamera = nullptr;

    // Reset camera manager
    cameraManager.reset();

    // Reset physics system
    physicsSystem.reset();

    // Reset time
    time.reset();

    // Reset running flag
    isRunning = false;
}

// Add missing destructor implementation
Scene::~Scene() {
    Shutdown();
}

void Scene::SetCreateDefaultObjects(bool create) {
    createDefaultObjects = create;
}

bool Scene::GetCreateDefaultObjects() const {
    return createDefaultObjects;
}

void Scene::SetRunning(bool running) {
    isRunning = running;
}

bool Scene::IsRunning() const {
    return isRunning;
}

void Scene::RenderFromCamera(Camera* camera) {
    if (!camera) {
        return;
    }

    // Set viewport
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = camera->GetViewportWidth();
    int viewportHeight = camera->GetViewportHeight();

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);

    // Get view and projection matrices
    Matrix4x4 viewMatrix = camera->GetViewMatrix();
    Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();

    // Get camera position
    Vector3 cameraPosition = camera->GetPosition();

    // Render game objects
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            RenderGameObject(gameObject, viewMatrix, projectionMatrix, cameraPosition);
        }
    }
}

void Scene::SetMinimapCamera(Camera* camera) {
    minimapCamera = camera;
}

Camera* Scene::GetMinimapCamera() const {
    return minimapCamera;
}

Camera* Scene::GetMainCamera() const {
    return mainCamera;
}

void Scene::SetResolutionChangeAllowed(bool allowed) {
    resolutionChangeAllowed = allowed;
}

void Scene::UpdateResolution(int width, int height) {
    if (!resolutionChangeAllowed) {
        return;
    }

    // Update viewport for all cameras
    if (mainCamera) {
        mainCamera->SetViewport(0, 0, width, height);
    }

    if (minimapCamera) {
        // Set minimap to a smaller size in the corner
        int minimapSize = std::min(width, height) / 4;
        minimapCamera->SetViewport(0, 0, minimapSize, minimapSize);
    }
}
