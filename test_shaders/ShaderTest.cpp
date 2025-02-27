#include "../Shaders/Core/ShaderProgram.h"
#include "../Shaders/Assets/ShaderAsset.h"
#include "../Model.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../PointLight.h"
#include <iostream>

// Test loading and compiling shaders
void TestShaderCompilation() {
    // Test vertex shader
    Shaders::ShaderProgram* standardProgram = Shaders::ShaderAsset::LoadProgram(
        "Shaders/Defaults/standard.vert",
        "Shaders/Defaults/standard.frag",
        "Shaders/Defaults/standard.geom"
    );
    
    if (!standardProgram) {
        std::cerr << "Failed to load standard shader program" << std::endl;
        return;
    }
    
    // Test unlit shader
    Shaders::ShaderProgram* unlitProgram = Shaders::ShaderAsset::LoadProgram(
        "Shaders/Defaults/unlit.vert",
        "Shaders/Defaults/unlit.frag"
    );
    
    if (!unlitProgram) {
        std::cerr << "Failed to load unlit shader program" << std::endl;
        return;
    }
    
    std::cout << "Shader compilation test passed" << std::endl;
}

// Test setting up a model with shaders
void TestModelShaderSetup() {
    // Load a test model
    Model model("test_assets/cube.obj");
    
    // Load shader program
    Shaders::ShaderProgram* program = Shaders::ShaderAsset::LoadProgram(
        "Shaders/Defaults/standard.vert",
        "Shaders/Defaults/standard.frag"
    );
    
    if (!program) {
        std::cerr << "Failed to load shader program for model test" << std::endl;
        return;
    }
    
    // Set shader program on model
    model.SetShaderProgram(program);
    
    std::cout << "Model shader setup test passed" << std::endl;
}

// Test rendering with shaders
void TestShaderRendering() {
    // Create scene
    Scene scene;
    scene.Initialize();
    
    // Create camera
    Camera* camera = new Camera();
    camera->SetPosition(Vector3(0, 0, -5));
    camera->LookAt(Vector3(0, 0, 0));
    scene.SetMainCamera(camera);
    
    // Create point light
    PointLight light;
    light.position = Vector3(2, 2, -2);
    light.color = Vector3(1, 1, 1);
    light.intensity = 1.0f;
    light.range = 10.0f;
    scene.AddPointLight(light);
    
    // Create test model with shader
    Model* model = new Model("test_assets/cube.obj");
    Shaders::ShaderProgram* program = Shaders::ShaderAsset::LoadProgram(
        "Shaders/Defaults/standard.vert",
        "Shaders/Defaults/standard.frag"
    );
    
    if (!program) {
        std::cerr << "Failed to load shader program for rendering test" << std::endl;
        return;
    }
    
    model->SetShaderProgram(program);
    
    // Create game object with model
    GameObject* obj = new GameObject();
    obj->AddComponent(model);
    scene.AddGameObject(obj);
    
    // Render one frame
    scene.RenderScene();
    
    std::cout << "Shader rendering test passed" << std::endl;
}

int main() {
    std::cout << "Running shader tests..." << std::endl;
    
    TestShaderCompilation();
    TestModelShaderSetup();
    TestShaderRendering();
    
    std::cout << "All shader tests completed" << std::endl;
    return 0;
}
