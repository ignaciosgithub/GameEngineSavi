# GameEngineSavi

GameEngineSavi is a lightweight C++ game engine designed for creating 3D games with OpenGL. The engine provides a component-based architecture similar to popular game engines like Unity, making it intuitive for developers familiar with those frameworks.

## Features

- **Component-Based Architecture**: Utilize GameObject and MonoBehaviourLike components to create modular game logic
- **3D Rendering**: Built-in support for 3D models with OBJ file loading
- **Scene Management**: Create and manage game scenes with multiple objects
- **Lighting System**: Dynamic point light system with customizable properties
- **Camera System**: Flexible camera implementation for scene viewing
- **Time Management**: Precise time tracking for frame-rate independent gameplay
- **Math Utilities**: Vector3 and Matrix4x4 implementations for 3D mathematics
- **Prefab System**: Create reusable game object templates
- **Physics System**: Rigid body physics with collision detection and response
- **Texture System**: Support for texture mapping with tiling, opacity, and normal maps
- **Engine Condition System**: Different engine states for editing, playing, and compiling
- **Debugger System**: Comprehensive error handling for script errors

## Architecture

The engine is structured around these core components:

### Core Components

- **GameObject**: The fundamental entity in the engine that can contain components, meshes, and child objects
- **MonoBehaviourLike**: Base class for all behavior components with lifecycle methods (Awake, Start, Update, etc.)
- **Scene**: Manages game objects, cameras, and the game loop
- **Time**: Handles time-related functionality for frame-rate independent gameplay

### Rendering System

- **Model**: Handles 3D model loading and rendering
- **Face/Triangle**: Basic geometric primitives
- **Pyramid**: Example of a built-in 3D shape
- **Camera**: Manages the view perspective
- **PointLight**: Implements dynamic lighting with attenuation

### Physics System

- **PhysicsSystem**: Manages physics simulation with fixed timestep
- **RigidBody**: Handles physical properties like mass, velocity, and forces
- **CollisionSystem**: Detects and resolves collisions between objects

### Math Utilities

- **Vector3**: 3D vector implementation with common operations
- **Matrix4x4**: Matrix operations for 3D transformations
- **Raycast**: Ray casting for collision detection

### Additional Features

- **Prefab**: Template system for creating reusable game objects
- **GUI**: User interface system for creating editor and game interfaces
- **ProjectSettings**: Project configuration management
- **Debugger**: Error handling and reporting system

## Getting Started

To use GameEngineSavi in your project:

1. Include the necessary header files in your project
2. Create a Scene to manage your game
3. Add GameObjects to your scene
4. Attach MonoBehaviourLike components to implement game logic
5. Call the Scene's Run method to start the game loop

## Example Usage

```cpp
// Create a new scene
std::unique_ptr<Scene> scene = std::make_unique<Scene>();

// Load the scene (initializes Time)
scene->Load();

// Create a game object
std::unique_ptr<GameObject> player = std::make_unique<GameObject>(
    "Player",                  // Name
    Vector3(0, 0, 0),          // Position
    Vector3(0, 0, 0),          // Rotation
    Vector3(1, 1, 1),          // Size
    std::vector<PointLight>()  // Lights
);

// Add a model to the game object
Model* playerModel = new Model("player.obj", Vector3(0, 0, 0), Vector3(0, 0, 0));
player->AddMesh(playerModel);

// Add the game object to the scene
scene->AddGameObject(std::move(player));

// Create and add a camera
std::unique_ptr<Camera> mainCamera = std::make_unique<Camera>();
scene->AddCamera(std::move(mainCamera));

// Run the game
scene->Run();
```

## Test Suite

The engine includes a comprehensive test suite that verifies each component through console output. To run the tests:

### Linux
```bash
cd TestSuite
make
./run_tests
```

### Windows
```batch
cd TestSuite
run_tests.bat
```

The test suite verifies:
- Vector3 operations (addition, subtraction, multiplication, normalization, etc.)
- Physics simulation (gravity, velocity integration, collision detection)
- GameObject component management
- Scene management
- Engine condition state transitions
- Debugger error handling

Each test outputs detailed information to the console, allowing verification of component functionality without requiring visual inspection.

## Requirements

- Windows or Linux operating system
- OpenGL support
- C++ compiler with C++11 support

## License

This project is available for use under open-source terms.

## Contributing

Contributions to GameEngineSavi are welcome! Feel free to submit pull requests or open issues to improve the engine.
