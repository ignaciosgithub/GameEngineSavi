#include "Scene.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include "EngineCondition.h"
#include "Scene_includes.h"
#include "platform.h"
#include "Graphics/Core/GraphicsAPIFactory.h"

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
        light.SetPosition(Vector3(0, 1, 5)); // Position light in front of objects, not directly above
        light.SetColor(Vector3(1, 1, 1));
        light.SetIntensity(1.0f);
        light.SetRange(10.0f);

        // Create a game object for the light
        GameObject* lightObj = new GameObject("Default Light");
        lightObj->position = Vector3(0, 1, 5); // Match the position set in the light
        lightObj->AddLight(light);
        AddGameObject(lightObj);

        std::cout << "Created default point light" << std::endl;
    }

    // Create a default cube if it doesn't already exist
    bool hasDefaultCube = false;
    for (auto& gameObject : gameObjects) {
        if (gameObject->GetName() == "Default Cube") {
            hasDefaultCube = true;
            break;
        }
    }
    
    if (!hasDefaultCube) {
        // Create a cube
        GameObject* cubeObj = new GameObject("Default Cube");

        // Load a cube model
        Model* cubeModel = new Model();
        if (cubeModel->LoadFromFile("test_assets/cube.obj")) {
            // Create and set a default shader program for the cube
            ShaderProgram* defaultShader = CreateDefaultShaderProgram();
            if (defaultShader) {
                cubeModel->SetShaderProgram(defaultShader);
                std::cout << "Set default shader program for cube" << std::endl;
            }
            
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
        camera->SetPosition(Vector3(0, 2, 5)); // Position camera behind the light, looking at cube
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

// SetPhysicsTimeStep and GetPhysicsTimeStep are now inline in Scene.h

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
    // Clear the screen using the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        graphics->Clear(true, true);
    }

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
    std::cout << "Scene::RenderScene - Starting scene rendering" << std::endl;
    
    // Initialize camera manager if not already created
    if (!cameraManager) {
        std::cout << "Scene::RenderScene - Creating new camera manager" << std::endl;
        cameraManager = std::unique_ptr<CameraManager>(new CameraManager());

        // Set main camera if available
        if (mainCamera) {
            std::cout << "Scene::RenderScene - Setting main camera in new camera manager" << std::endl;
            cameraManager->SetMainCamera(mainCamera);
        } else {
            std::cout << "Scene::RenderScene - WARNING: No main camera available" << std::endl;
        }
    }

    // Get active cameras
    std::cout << "Scene::RenderScene - Getting active cameras" << std::endl;
    std::vector<Camera*> cameras = cameraManager->GetActiveCameras();
    std::cout << "Scene::RenderScene - Found " << cameras.size() << " active cameras" << std::endl;

    // Render from all cameras
    for (auto& camera : cameras) {
        if (!camera) {
            std::cout << "Scene::RenderScene - WARNING: Skipping null camera" << std::endl;
            continue;
        }
        
        std::cout << "Scene::RenderScene - Rendering from camera at position: " 
                  << camera->GetPosition().x << ", " 
                  << camera->GetPosition().y << ", " 
                  << camera->GetPosition().z << std::endl;

        // Set viewport using the graphics API
        int viewportX = 0;
        int viewportY = 0;
        int viewportWidth = camera->GetViewportWidth();
        int viewportHeight = camera->GetViewportHeight();
        
        std::cout << "Scene::RenderScene - Setting viewport: " 
                  << viewportX << ", " << viewportY << ", " 
                  << viewportWidth << ", " << viewportHeight << std::endl;

        auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
        if (graphics) {
            graphics->SetViewport(viewportX, viewportY, viewportWidth, viewportHeight);
        } else {
            std::cout << "Scene::RenderScene - ERROR: Failed to get graphics API" << std::endl;
            return;
        }

        // Get view and projection matrices
        std::cout << "Scene::RenderScene - Getting view and projection matrices" << std::endl;
        Matrix4x4 viewMatrix = camera->GetViewMatrix();
        Matrix4x4 projectionMatrix = camera->GetProjectionMatrix();

        // Get camera position
        Vector3 cameraPosition = camera->GetPosition();

        // Render game objects
        std::cout << "Scene::RenderScene - Rendering " << gameObjects.size() << " game objects" << std::endl;
        for (auto& gameObject : gameObjects) {
            if (gameObject) {
                std::cout << "Scene::RenderScene - Rendering game object: " << gameObject->GetName() << std::endl;
                RenderGameObject(gameObject, viewMatrix, projectionMatrix, cameraPosition);
            } else {
                std::cout << "Scene::RenderScene - WARNING: Skipping null game object" << std::endl;
            }
        }

        // Draw coordinate axes for debugging (only in editor mode)
        if (EngineCondition::IsInEditor()) {
            std::cout << "Scene::RenderScene - Drawing debug axes (editor mode)" << std::endl;
            DrawDebugAxes();
        }
    }
    
    std::cout << "Scene::RenderScene - Scene rendering completed" << std::endl;
}

void Scene::RenderMesh(Model* mesh, const Matrix4x4& modelMatrix, const Matrix4x4& viewMatrix, const Matrix4x4& projectionMatrix, const Vector3& cameraPosition) {
    std::cout << "Scene::RenderMesh - Starting mesh rendering" << std::endl;
    
    if (!mesh) {
        std::cout << "Scene::RenderMesh - ERROR: Mesh is null" << std::endl;
        return;
    }

    // Get all point lights in the scene
    std::cout << "Scene::RenderMesh - Collecting point lights" << std::endl;
    std::vector<PointLight> pointLights;
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            for (auto& light : gameObject->lights) {
                pointLights.push_back(light);
            }
        }
    }
    std::cout << "Scene::RenderMesh - Collected " << pointLights.size() << " point lights" << std::endl;

    // Get all directional lights in the scene
    std::cout << "Scene::RenderMesh - Collecting directional lights" << std::endl;
    std::vector<DirectionalLight> dirLights;
    for (auto& gameObject : gameObjects) {
        if (gameObject) {
            for (auto& light : gameObject->directionalLights) {
                dirLights.push_back(light);
            }
        }
    }
    // Add scene-level directional lights
    dirLights.insert(dirLights.end(), directionalLights.begin(), directionalLights.end());
    std::cout << "Scene::RenderMesh - Collected " << dirLights.size() << " directional lights" << std::endl;

    // Render the mesh with the lights and camera matrices
    std::cout << "Scene::RenderMesh - Calling mesh->Render with collected lights and matrices" << std::endl;
    mesh->Render(pointLights, dirLights, viewMatrix, projectionMatrix);
    std::cout << "Scene::RenderMesh - Mesh rendering completed" << std::endl;
}

void Scene::DrawDebugAxes() {
    // Draw coordinate axes for debugging using the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        return;
    }
    
    // Draw the debug axes using the graphics API
    graphics->DrawDebugAxes();
}

void Scene::SetGlobalShaderUniforms(ShaderProgram* program) {
    if (!program) {
        return;
    }

    // Set time uniforms
    if (time) {
        program->SetUniform("time", time->GetTime());
        program->SetUniform("deltaTime", time->GetDeltaTime());
    }

    // Set camera uniforms
    if (mainCamera) {
         program->SetUniform("viewMatrix", mainCamera->GetViewMatrix());
         program->SetUniform("projectionMatrix", mainCamera->GetProjectionMatrix());
         program->SetUniform("cameraPosition", mainCamera->GetPosition());
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
     program->SetUniform("pointLightCount", pointLightCount);

    for (int i = 0; i < pointLightCount; i++) {
        std::string prefix = "pointLights[" + std::to_string(i) + "].";
         program->SetUniform(prefix + "position", pointLights[i].GetPosition());
         program->SetUniform(prefix + "color", pointLights[i].GetColor());
         program->SetUniform(prefix + "intensity", pointLights[i].GetIntensity());
         program->SetUniform(prefix + "range", pointLights[i].GetRange());
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

// Directional light methods
void Scene::AddDirectionalLight(const DirectionalLight& light) {
    directionalLights.push_back(light);
}

const std::vector<DirectionalLight>& Scene::GetDirectionalLights() const {
    return directionalLights;
}

// Create a default shader program for models without a shader
ShaderProgram* Scene::CreateDefaultShaderProgram() {
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "Failed to get graphics API" << std::endl;
        return nullptr;
    }
    
    // Create and compile vertex shader
    unsigned int vertexShader = graphics->CreateShader(0x8B31); // GL_VERTEX_SHADER
    const char* vertexShaderSource = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aNormal;\n"
        "out vec3 FragPos;\n"
        "out vec3 Normal;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "}\0";
    graphics->ShaderSource(vertexShader, std::string(vertexShaderSource));
    graphics->CompileShader(vertexShader);
    
    // Create and compile fragment shader
    unsigned int fragmentShader = graphics->CreateShader(0x8B30); // GL_FRAGMENT_SHADER
    const char* fragmentShaderSource = 
        "#version 330 core\n"
        "in vec3 FragPos;\n"
        "in vec3 Normal;\n"
        "out vec4 FragColor;\n"
        "uniform vec3 viewPos;\n"
        "\n"
        "struct PointLight {\n"
        "    vec3 position;\n"
        "    vec3 color;\n"
        "    float intensity;\n"
        "    float range;\n"
        "};\n"
        "\n"
        "uniform int pointLightCount;\n"
        "uniform PointLight pointLights[8];\n"
        "\n"
        "void main()\n"
        "{\n"
        "   // Define material properties\n"
        "   vec3 materialDiffuse = vec3(1.0, 0.2, 0.2);\n" // Bright red diffuse material
        "   vec3 materialSpecular = vec3(0.5, 0.5, 0.5);\n" // Gray specular highlights
        "   float materialShininess = 32.0;\n"
        "   \n"
        "   // Calculate base ambient lighting\n"
        "   vec3 ambient = 0.1 * materialDiffuse;\n"
        "   \n"
        "   // Normalize vectors\n"
        "   vec3 norm = normalize(Normal);\n"
        "   vec3 viewDir = normalize(viewPos - FragPos);\n"
        "   \n"
        "   // Initialize lighting results\n"
        "   vec3 diffuse = vec3(0.0);\n"
        "   vec3 specular = vec3(0.0);\n"
        "   \n"
        "   // Calculate lighting for each point light\n"
        "   for(int i = 0; i < pointLightCount; i++) {\n"
        "       // Calculate light direction and distance\n"
        "       vec3 lightDir = normalize(pointLights[i].position - FragPos);\n"
        "       float distance = length(pointLights[i].position - FragPos);\n"
        "       \n"
        "       // Calculate attenuation based on distance and range\n"
        "       float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);\n"
        "       if(distance > pointLights[i].range) {\n"
        "           attenuation = 0.0;\n"
        "       }\n"
        "       \n"
        "       // Calculate diffuse lighting\n"
        "       float diff = max(dot(norm, lightDir), 0.0);\n"
        "       diffuse += diff * pointLights[i].color * pointLights[i].intensity * attenuation;\n"
        "       \n"
        "       // Calculate specular lighting\n"
        "       vec3 reflectDir = reflect(-lightDir, norm);\n"
        "       float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);\n"
        "       specular += spec * pointLights[i].color * pointLights[i].intensity * attenuation;\n"
        "   }\n"
        "   \n"
        "   // If no lights, use a fallback light\n"
        "   if(pointLightCount == 0) {\n"
        "       vec3 lightDir = normalize(vec3(0.0, 1.0, 5.0) - FragPos);\n"
        "       float diff = max(dot(norm, lightDir), 0.0);\n"
        "       diffuse = diff * vec3(1.0);\n"
        "       \n"
        "       vec3 reflectDir = reflect(-lightDir, norm);\n"
        "       float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);\n"
        "       specular = spec * vec3(0.5);\n"
        "   }\n"
        "   \n"
        "   // Combine all lighting components\n"
        "   vec3 result = (ambient + diffuse * materialDiffuse + specular * materialSpecular);\n"
        "   FragColor = vec4(result, 1.0);\n"
        "}\0";
    graphics->ShaderSource(fragmentShader, std::string(fragmentShaderSource));
    graphics->CompileShader(fragmentShader);
    
    // Create shader program
    unsigned int shaderProgramId = graphics->CreateProgram();
    graphics->AttachShader(shaderProgramId, vertexShader);
    graphics->AttachShader(shaderProgramId, fragmentShader);
    graphics->LinkProgram(shaderProgramId);
    
    // Create a ShaderProgram object
    ShaderProgram* program = new ShaderProgram();
    program->SetProgramId(shaderProgramId);
    
    // Clean up shader objects
    graphics->DeleteShader(vertexShader);
    graphics->DeleteShader(fragmentShader);
    
    return program;
}

// SetCreateDefaultObjects and GetCreateDefaultObjects are now inline in Scene.h

// SetRunning and IsRunning are now inline in Scene.h

void Scene::RenderFromCamera(Camera* camera) {
    if (!camera) {
        return;
    }

    // Set viewport using the graphics API
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = camera->GetViewportWidth();
    int viewportHeight = camera->GetViewportHeight();

    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (graphics) {
        graphics->SetViewport(viewportX, viewportY, viewportWidth, viewportHeight);
    }

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

// GetMinimapCamera, GetMainCamera, and SetResolutionChangeAllowed are now inline in Scene.h

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
