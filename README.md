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

## Installation

### Linux
1. Clone the repository:
   ```bash
   git clone https://github.com/ignaciosgithub/GameEngineSavi.git
   cd GameEngineSavi
   ```

2. Build the engine:
   ```bash
   make
   ```

### Windows
1. Clone the repository:
   ```batch
   git clone https://github.com/ignaciosgithub/GameEngineSavi.git
   cd GameEngineSavi
   ```

2. Using Visual Studio:
   - Open the solution file
   - Build using Visual Studio

3. Using MinGW:
   ```batch
   mingw32-make -f Makefile.mingw
   ```

## Creating a New Project

1. Create project structure:
   ```cpp
   #include "ProjectSettings/ProjectSettings.h"

   auto& settings = ProjectSettings::GetInstance();
   settings.CreateNewProject("MyGame", "path/to/project");
   ```

2. Project Structure:
   ```
   MyGame/
   ├── Assets/
   │   ├── Models/
   │   ├── Textures/
   │   ├── Sounds/
   │   └── Scripts/
   ├── Build/
   └── project.json
   ```

3. Configure project settings in project.json:
   ```json
   {
       "projectName": "MyGame",
       "engineVersion": "1.0.0",
       "buildSettings": {
           "debugSymbols": true,
           "optimization": false,
           "targetPlatforms": ["Windows", "Linux"]
       },
       "engineSettings": {
           "physics": {
               "fixedTimeStep": 0.01666667,
               "gravity": 9.81,
               "enableCollisions": true
           },
           "rendering": {
               "targetFPS": 60,
               "vsync": true
           }
       }
   }
   ```

## Time Management

The engine uses a high-resolution clock for precise timing:

```cpp
// Get delta time between frames
float dt = Time::GetInstance().DeltaTime();

// Physics uses fixed timestep
float fixedDt = ProjectSettings::GetInstance().GetFixedTimeStep();
```

Time is managed through two systems:
1. EngineTime: Frame-based timing for rendering
2. Fixed timestep: Physics simulation at 60Hz

## Engine States

The engine operates in different states:
1. IN_EDITOR_PLAYING: Game running in editor window
2. DEBUG_BUILD: Debug build with detailed error reporting
3. IN_EDITOR_EDITING: Editor mode state
4. RELEASE: Release build state
5. IN_EDITOR_COMPILING: Game compilation state

Switch states using:
```cpp
EngineCondition::SetState(EngineCondition::State::DEBUG_BUILD_STATE);
```

## Default Scene Configuration

New scenes include:
1. One point light
2. Default cube object
3. Free camera system

Example scene setup:
```cpp
auto scene = std::make_unique<Scene>();
scene->Load();

// Default point light
auto light = std::make_unique<PointLight>();
scene->AddLight(std::move(light));

// Default cube
auto cube = std::make_unique<GameObject>("Cube");
scene->AddGameObject(std::move(cube));

// Free camera
auto camera = std::make_unique<Camera>();
scene->AddCamera(std::move(camera));
```

## Error Handling

The engine uses a robust error handling system:
1. Script errors are caught and reported
2. Warning messages instead of crashes
3. Debugger class for error management

Example error handling:
```cpp
try {
    // Your game code
} catch (const std::exception& e) {
    Debugger::LogError(e.what());
    // Engine continues running
}
```

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

### Build Requirements
- C++ compiler with C++14 support
- OpenGL development libraries
  - Linux: `sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev`
  - Windows: Included with Visual Studio or MinGW
- X11 development libraries (Linux only)
  - `sudo apt-get install libx11-dev`

### Build Tools
- Linux: GCC/G++ and Make
- Windows: Visual Studio or MinGW-w64 with GCC

### IDE Support
- Visual Studio Code (recommended)
- Visual Studio
- Any C++ IDE with CMake support

## License

This project is available for use under open-source terms.

## Contributing

Contributions to GameEngineSavi are welcome! Feel free to submit pull requests or open issues to improve the engine.

## Networking System

The GameEngineSavi engine includes a comprehensive networking system that supports both peer-to-peer and client-server architectures. The networking system is designed to be flexible and easy to use, allowing developers to create multiplayer games with minimal effort.

### Features

- Support for both peer-to-peer and client-server networking models
- Cross-platform socket handling for Windows and Linux
- Network component that can be attached to game objects
- Packet system for reliable and unreliable data transmission
- Network debugging tools with packet logging and connection quality metrics

### Usage

To use the networking system, you need to include the appropriate headers:

```cpp
#include "Network/NetworkSystem.h"
#include "Network/NetworkComponent.h"
#include "Network/Packet.h"
```

#### Server Example

```cpp
// Initialize the network system as a server
Network::NetworkSystem netSystem;
netSystem.Initialize(true, false);  // true = server, false = not peer-to-peer
netSystem.StartServer(7777);        // Start server on port 7777

// Main loop
while (running) {
    netSystem.Update();
    // Handle network events
}

// Cleanup
netSystem.Cleanup();
```

#### Client Example

```cpp
// Initialize the network system as a client
Network::NetworkSystem netSystem;
netSystem.Initialize(false, false);  // false = client, false = not peer-to-peer
netSystem.Connect("127.0.0.1", 7777); // Connect to server at 127.0.0.1:7777

// Main loop
while (running) {
    netSystem.Update();
    // Handle network events
}

// Cleanup
netSystem.Cleanup();
```

#### Peer-to-Peer Example

```cpp
// Initialize the network system in peer-to-peer mode
Network::NetworkSystem netSystem;
netSystem.Initialize(false, true);  // false = not server, true = peer-to-peer
netSystem.StartPeer();

// Main loop
while (running) {
    netSystem.Update();
    // Handle network events
}

// Cleanup
netSystem.Cleanup();
```

### Network Demo

The engine includes a network demo application that demonstrates the networking system in action. You can build and run the demo using the provided build scripts:

#### Linux
```bash
cd test_network
./build_network_demo.sh
../bin/linux/NetworkDemo --server     # Run in server mode
../bin/linux/NetworkDemo --client     # Run in client mode
../bin/linux/NetworkDemo --p2p        # Run in peer-to-peer mode
../bin/linux/NetworkDemo --debug      # Enable debugging
```

#### Windows
```batch
cd test_network
build_network_demo.bat
..\bin\windows\NetworkDemo.exe --server     # Run in server mode
..\bin\windows\NetworkDemo.exe --client     # Run in client mode
..\bin\windows\NetworkDemo.exe --p2p        # Run in peer-to-peer mode
..\bin\windows\NetworkDemo.exe --debug      # Enable debugging
```
