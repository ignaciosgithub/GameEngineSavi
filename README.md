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
- **TimeManager**: Handles time-related functionality for frame-rate independent gameplay

### Rendering System

- **Model**: Handles 3D model loading and rendering
- **Face/Triangle**: Basic geometric primitives
- **Pyramid**: Example of a built-in 3D shape
- **Camera**: Manages the view perspective
- **PointLight**: Implements dynamic lighting with attenuation
- **DirectionalLight**: Implements directional lighting (like sunlight) with direction, color, and intensity

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

## Visual Studio 2022 Setup

GameEngineSavi includes full support for Visual Studio 2022, making it easy to develop games on Windows.

### Quick Setup

1. **Prerequisites**:
   - Visual Studio 2022 (Community, Professional, or Enterprise)
   - Desktop development with C++ workload installed
   - Windows 10 or 11

2. **Open the Solution**:
   - Clone the repository
   - Open `GameEngineSavi.sln` in Visual Studio 2022

3. **Build and Run**:
   - Select your desired configuration (Debug/Release) and platform (x64/Win32)
   - Press F7 to build or F5 to build and run

### Detailed Documentation

For comprehensive Visual Studio 2022 setup instructions, debugging tips, performance optimization, and more, see our [detailed Visual Studio 2022 guide](VS2022_GUIDE.md).

This guide covers:
- Complete project setup
- Building configurations
- Debugging techniques
- Project structure
- Creating new games
- Using advanced features
- Troubleshooting common issues
- Performance optimization

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
float dt = TimeManager::GetInstance().DeltaTime();

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
1. One point light at position (0, 5, 0)
2. Default cube object at position (0, 0, 0)
3. Free camera system at position (0, 2, 5) looking at the origin

Example scene setup:
```cpp
auto scene = std::make_unique<Scene>();
scene->Load();

// Default point light
auto light = std::make_unique<PointLight>();
light->position = Vector3(0, 5, 0);
scene->AddLight(std::move(light));

// Default cube
auto cube = std::make_unique<GameObject>("Default Cube");
cube->SetPosition(Vector3(0, 0, 0));
scene->AddGameObject(std::move(cube));

// Free camera
auto camera = std::make_unique<Camera>();
camera->SetPosition(Vector3(0, 2, 5));
camera->LookAt(Vector3(0, 0, 0));
scene->AddCamera(std::move(camera));
```

## Editor Interface

The GameEngineSavi editor provides a visual interface for creating and editing game scenes. The editor is designed to be intuitive and easy to use, with a layout similar to popular game engines.

```
+----------------------------------------------------------------------------------------------------------+
|                                       GameEngineSavi Editor                                              |
+------------------+-----------------------------------------------+-----------------------------------+
|  Hierarchy       |                                               |  Inspector                        |
|                  |                                               |                                   |
|  > Default Light |                  Scene View                   |  Transform                        |
|  > Default Cube  |                                               |  Position: X: 0  Y: 0  Z: 0       |
|                  |                                               |  Rotation: X: 0  Y: 0  Z: 0       |
|                  |                                               |  Scale:    X: 1  Y: 1  Z: 1       |
|                  |                                               |                                   |
|                  |                                               |  Material                         |
|                  |                                               |  Albedo: [Default]                |
|                  |                                               |  Normal Map: [None]               |
|                  |                                               |  Opacity: 1.0                     |
|                  |                                               |                                   |
|                  |                                               |  Physics                          |
|                  |                                               |  [x] Use Gravity                  |
|                  |                                               |  [ ] Is Kinematic                 |
|                  |                                               |  Friction: 0.5                    |
+------------------+-----------------------------------------------+-----------------------------------+
|                                                                                                      |
|  Project                                                                                             |
|  [Models] [Textures] [Scenes] [Scripts] [Prefabs] [Audio]                                            |
|                                                                                                      |
+----------------------------------------------------------------------------------------------------------+
```

### Editor Panels

#### Hierarchy Panel
The Hierarchy Panel lists all objects in the current scene. By default, a new scene includes:
- Default Light: A point light positioned at (0, 5, 0)
- Default Cube: A cube positioned at (0, 0, 0)

You can:
- Click on an object to select it
- Right-click to create new objects
- Drag objects to reorder them
- Use the context menu to duplicate, delete, or rename objects

#### Scene View
The Scene View displays the 3D scene with the default cube and lighting. This is where you can:
- View and manipulate objects in 3D space
- Use the gizmos to move, rotate, and scale objects
- Navigate the scene using camera controls

#### Inspector Panel
The Inspector Panel shows properties of the selected object, including:
- Transform: Position, rotation, and scale
- Material: Albedo texture, normal map, opacity
- Physics: Gravity, kinematic state, friction
- Components: Any attached components and their properties

#### Project Panel
The Project Panel displays project assets organized by type:
- Models: 3D models (.obj files)
- Textures: Image files for materials
- Scenes: Saved scene files
- Scripts: C++ script files
- Prefabs: Reusable object templates
- Audio: Sound files

### Camera Controls

The editor camera is positioned at (0, 2, 5) by default, looking at the origin (0, 0, 0) where the default cube is placed.

- **WASD**: Move the camera
  - W: Move forward
  - A: Move left
  - S: Move backward
  - D: Move right
- **Mouse**: Look around by clicking and dragging in the Scene View
- **Shift**: Hold to move the camera faster
- **Space**: Play/Pause the scene to test game logic

### Editor Shortcuts

| Shortcut | Action |
|----------|--------|
| Ctrl+N | New Scene |
| Ctrl+O | Open Scene |
| Ctrl+S | Save Scene |
| Ctrl+Shift+S | Save Scene As |
| F | Focus Selected |
| W | Move Tool |
| E | Rotate Tool |
| R | Scale Tool |
| Q | No Tool (Selection Only) |
| Delete | Delete Selected |
| Ctrl+D | Duplicate Selected |
| Ctrl+Z | Undo |
| Ctrl+Y | Redo |
| Space | Play/Stop |
| F12 | Capture Screenshot |
| Ctrl+1 | Hierarchy Panel Focus |
| Ctrl+2 | Scene View Focus |
| Ctrl+3 | Inspector Panel Focus |
| Ctrl+4 | Project Panel Focus |

### Running the Editor

#### Linux
```bash
# Build the editor
./build_editor.sh

# Run the editor
./run_editor.sh
```

#### Windows
```batch
# Using Visual Studio
# Open GameEngineSavi.sln and build the solution

# Using MinGW
mingw32-make -f Makefile.mingw editor

# Run the editor
run_editor.bat
```

### Screenshot Functionality

The editor includes built-in screenshot functionality that allows you to capture the current view of the editor or game scene. This is useful for:

- Documenting your game development progress
- Creating promotional materials
- Reporting bugs or issues
- Sharing your work with others

To capture a screenshot:

1. Press the **F12** key while the editor is running
2. The screenshot will be saved to the `screenshots/editor/` directory
3. The filename will be `editor_interface.png` by default

You can also programmatically capture screenshots using the `Editor::CaptureScreenshot()` method:

```cpp
// Capture a screenshot with a custom filename
editor->CaptureScreenshot("my_custom_screenshot.png");
```

The screenshot functionality uses the `FrameCapture` class which reads the OpenGL frame buffer and saves it as a PNG file. This class can be used in your own games to implement screenshot functionality.

#### Editor Interface
![Editor Interface](screenshots/editor/editor_interface_improved.png)

#### Scene View
The scene view shows the 3D objects with proper lighting and perspective:
![Scene View](screenshots/editor/scene_view_improved.png)

The editor interface includes:
- Hierarchy panel showing scene objects
- Inspector panel with object properties
- Scene view with 3D rendering
- Project panel for asset management


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

// Load the scene (initializes TimeManager)
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

## Lighting System

GameEngineSavi includes a powerful lighting system that allows you to create dynamic lighting effects in your game. The lighting system supports both point lights and directional lights, providing flexibility for different lighting scenarios.

### Features

- **Point Lights**: Create point lights with customizable position, color, intensity, and range
- **Directional Lights**: Create directional lights (like sunlight) with direction, color, and intensity
- **Multiple Lights**: Support for up to 8 point lights and 4 directional lights in a scene
- **Ambient Light**: Set global ambient light for the scene
- **Attenuation**: Light intensity decreases with distance from the source (for point lights)
- **Shadow Mapping**: Basic shadow mapping for lights
- **Light Components**: Attach lights to game objects

### Point Lights

Point lights emit light in all directions from a specific position, similar to a light bulb or torch. The intensity of the light decreases with distance from the source.

#### Creating Point Lights

```cpp
#include "PointLight.h"

// Create a point light
PointLight* light = new PointLight();
light->SetPosition(Vector3(0, 5, 0));
light->SetColor(Vector3(1, 1, 1));
light->SetIntensity(1.0f);
light->SetRange(20.0f);

// Add the light to a game object
GameObject* lightObj = new GameObject("Light");
lightObj->AddLight(light);

// Add the game object to the scene
scene->AddGameObject(lightObj);
```

#### Point Light Properties

- **Position**: The position of the light in 3D space
- **Color**: The color of the light (RGB)
- **Intensity**: The brightness of the light
- **Range**: The maximum distance the light can reach
- **Attenuation**: How quickly the light fades with distance

### Directional Lights

Directional lights simulate light sources that are infinitely far away, like the sun. They have a direction but no position, and they affect all objects in the scene equally regardless of distance.

#### Creating Directional Lights

```cpp
#include "DirectionalLight.h"

// Create a directional light (like sunlight)
DirectionalLight* sunlight = new DirectionalLight();
sunlight->SetDirection(Vector3(0.5f, -1.0f, 0.3f).normalized());
sunlight->SetColor(Vector3(1.0f, 0.9f, 0.8f));
sunlight->SetIntensity(1.2f);

// Add the directional light to the scene
scene->AddDirectionalLight(sunlight);

// Or add to a game object
GameObject* sun = new GameObject("Sun");
sun->AddDirectionalLight(sunlight);
scene->AddGameObject(sun);
```

#### Directional Light Properties

- **Direction**: The direction the light is coming from (normalized vector)
- **Color**: The color of the light (RGB)
- **Intensity**: The brightness of the light

### Multiple Lights

You can add multiple lights to a scene to create complex lighting effects:

```cpp
// Create multiple point lights
PointLight* redLight = new PointLight();
redLight->SetPosition(Vector3(-5, 2, 0));
redLight->SetColor(Vector3(1, 0, 0));
redLight->SetIntensity(0.8f);
redLight->SetRange(15.0f);

PointLight* greenLight = new PointLight();
greenLight->SetPosition(Vector3(5, 2, 0));
greenLight->SetColor(Vector3(0, 1, 0));
greenLight->SetIntensity(0.8f);
greenLight->SetRange(15.0f);

// Create a directional light
DirectionalLight* blueLight = new DirectionalLight();
blueLight->SetDirection(Vector3(0, -1, 0).normalized());
blueLight->SetColor(Vector3(0, 0, 1));
blueLight->SetIntensity(0.5f);

// Add lights to the scene
scene->AddLight(redLight);
scene->AddLight(greenLight);
scene->AddDirectionalLight(blueLight);
```

### Ambient Light

You can set the ambient light for the scene to provide a base level of illumination:

```cpp
// Set ambient light
scene->SetAmbientLight(Vector3(0.1f, 0.1f, 0.1f));
```

### Shadow Mapping

The lighting system includes basic shadow mapping for lights:

```cpp
// Enable shadows for a light
light->EnableShadows(true);
```

Note that shadow mapping is a performance-intensive feature and should be used sparingly.

### Shader Integration

The lighting system is integrated with the shader system. The standard shader includes support for both point lights and directional lights:

```glsl
// Point light structure in GLSL
struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};

// Directional light structure in GLSL
struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;
};

// Uniforms in the shader
uniform PointLight pointLights[8];
uniform int numPointLights;
uniform DirectionalLight directionalLights[4];
uniform int numDirectionalLights;
```

## Navigation Mesh System

GameEngineSavi includes a powerful Navigation Mesh (NavMesh) system for AI pathfinding and navigation. The NavMesh system allows AI entities to navigate through complex environments, avoiding obstacles and finding optimal paths.

### Features

- **Automatic NavMesh Generation**: Generate navigation meshes from scene geometry
- **Pathfinding**: Find optimal paths between points on the NavMesh
- **Obstacle Avoidance**: Navigate around static and dynamic obstacles
- **Volumetric Obstacles**: Support for obstacles that span multiple grid cells
- **Runtime Updates**: Update the NavMesh as the scene changes
- **AI Entity Integration**: Easily integrate with AI entities

### Using the NavMesh System

```cpp
#include "NavMesh.h"
#include "NavMeshManager.h"
#include "AIEntity.h"

// Create a NavMesh for the current scene
NavMesh* navMesh = new NavMesh();
navMesh->Generate(scene);

// Find a path between two points
Vector3 start(0, 0, 0);
Vector3 end(10, 0, 10);
std::vector<Vector3> path = navMesh->FindPath(start, end);

// Create an AI entity that follows the path
AIEntity* entity = new AIEntity();
entity->SetPath(path);
entity->SetSpeed(5.0f);

// Update the entity in the game loop
entity->Update(deltaTime);
```

### NavMesh Components

- **NavMesh**: The navigation mesh itself, generated from scene geometry
- **NavMeshManager**: Manages multiple NavMeshes and handles updates
- **AIEntity**: Entities that can navigate using the NavMesh

### Creating Invisible Collision Walls

You can create invisible collision walls that affect the NavMesh but are not rendered:

```cpp
// Create an invisible wall that blocks AI navigation
auto wall = std::make_unique<GameObject>("InvisibleWall");
wall->SetPosition(Vector3(5, 0, 5));
wall->SetSize(Vector3(10, 5, 1));

// Add a collider but no mesh renderer
auto collider = wall->AddComponent(new BoxCollider());
collider->SetIsTrigger(false); // Physical collision, not just a trigger

// Add to scene
scene->AddGameObject(std::move(wall));

// Update the NavMesh to include the new obstacle
navMesh->Update();
```

### Creating Trigger Volumes

You can create trigger volumes that detect when entities enter or exit an area:

```cpp
// Create a trigger volume
auto triggerVolume = std::make_unique<GameObject>("TriggerVolume");
triggerVolume->SetPosition(Vector3(0, 0, 0));
triggerVolume->SetSize(Vector3(5, 5, 5));

// Add a collider set as a trigger
auto collider = triggerVolume->AddComponent(new BoxCollider());
collider->SetIsTrigger(true); // Trigger only, no physical collision

// Add a custom component to handle trigger events
class TriggerHandler : public MonoBehaviourLike {
public:
    void OnTriggerEnter(GameObject* other) override {
        std::cout << "Entity entered trigger: " << other->GetName() << std::endl;
    }
    
    void OnTriggerExit(GameObject* other) override {
        std::cout << "Entity exited trigger: " << other->GetName() << std::endl;
    }
};

triggerVolume->AddComponent(new TriggerHandler());

// Add to scene
scene->AddGameObject(std::move(triggerVolume));
```

### Volumetric Obstacles

The NavMesh system supports volumetric obstacles that span multiple grid cells:

```cpp
// Create a volumetric obstacle
auto obstacle = std::make_unique<GameObject>("VolumetricObstacle");
obstacle->SetPosition(Vector3(0, 0, 0));
obstacle->SetSize(Vector3(5, 3, 5)); // Spans multiple grid cells

// Add a collider
auto collider = obstacle->AddComponent(new BoxCollider());

// Add to scene
scene->AddGameObject(std::move(obstacle));

// Update the NavMesh to include the new obstacle
navMesh->Update();
```

### NavMesh Visualization

The NavMesh can be visualized in the editor for debugging purposes:

```cpp
// Enable NavMesh visualization
navMesh->SetVisualizationEnabled(true);
```

This will display the NavMesh grid, walkable areas, and obstacles in the scene view.

### Path Smoothing

The NavMesh system includes path smoothing to create more natural-looking paths:

```cpp
// Find a path with smoothing
std::vector<Vector3> path = navMesh->FindPath(start, end, true);
```

### Dynamic Obstacles

The NavMesh system can handle dynamic obstacles that move during gameplay:

```cpp
// Register a dynamic obstacle
navMesh->RegisterDynamicObstacle(gameObject);

// Update the NavMesh when the obstacle moves
navMesh->UpdateDynamicObstacles();
```

## Animation System

GameEngineSavi includes a powerful animation system that allows you to create and play animations for your game objects. The animation system is based on keyframes using separate OBJ files of the same object in different poses, with linear interpolation between keyframes.

### Features

- **Keyframe-Based Animation**: Create animations using separate OBJ files for each keyframe
- **Linear Interpolation**: Smooth transitions between keyframes
- **Animation Components**: Attach animations to game objects
- **Animation Playback Control**: Play, pause, stop, and loop animations
- **Animation Events**: Trigger events at specific points in animations
- **Animation Blending**: Blend between multiple animations
- **Animation Speed Control**: Adjust animation playback speed

### Creating Animations

To create an animation, you need to:

1. Create OBJ files for each keyframe of your animation
2. Define an animation file (.savanim) that references these keyframes
3. Load the animation in your game

#### Animation File Format

Animation files use a simple JSON format:

```json
{
  "name": "walk",
  "keyframes": [
    {
      "time": 0.0,
      "mesh": "walk/cube_pose1.obj"
    },
    {
      "time": 0.33,
      "mesh": "walk/cube_pose2.obj"
    },
    {
      "time": 0.66,
      "mesh": "walk/cube_pose3.obj"
    },
    {
      "time": 1.0,
      "mesh": "walk/cube_pose4.obj"
    }
  ],
  "loop": true,
  "duration": 1.0
}
```

### Using Animations

Here's how to load and play an animation:

```cpp
#include "Animation/Animation.h"
#include "Animation/AnimationComponent.h"
#include "Animation/AnimationLoader.h"

// Load animation from file
Animation* walkAnimation = AnimationLoader::LoadAnimation("test_animations/walk/cube_walk.savanim");

// Create a game object
auto character = std::make_unique<GameObject>("Character");

// Add animation component
auto animComponent = character->AddComponent(new AnimationComponent());

// Add animation to component
animComponent->AddAnimation("walk", walkAnimation);

// Play animation
animComponent->PlayAnimation("walk");

// Add to scene
scene->AddGameObject(std::move(character));
```

### Animation Playback Control

You can control animation playback with these methods:

```cpp
// Get animation component
auto animComponent = gameObject->GetComponents<AnimationComponent>()[0];

// Play animation
animComponent->PlayAnimation("walk");

// Pause animation
animComponent->PauseAnimation();

// Resume animation
animComponent->ResumeAnimation();

// Stop animation
animComponent->StopAnimation();

// Set animation speed
animComponent->SetSpeed(2.0f); // Play at 2x speed

// Set looping
animComponent->SetLooping(true);

// Check if animation is playing
bool isPlaying = animComponent->IsPlaying();
```

### Animation Events

You can register callbacks to be triggered at specific points in an animation:

```cpp
// Register animation event
animComponent->RegisterEvent("walk", 0.5f, []() {
    // This will be called halfway through the walk animation
    std::cout << "Footstep sound!" << std::endl;
});
```

### Animation Blending

You can blend between multiple animations for smooth transitions:

```cpp
// Blend from walk to run over 0.5 seconds
animComponent->BlendTo("run", 0.5f);
```

### Animation with Physics

The animation system integrates with the physics system, allowing animated objects to interact with the physics world:

```cpp
// Create a game object with physics and animation
auto character = std::make_unique<GameObject>("Character");

// Add rigid body component
auto rigidBody = character->AddComponent(new RigidBody());
rigidBody->SetMass(10.0f);

// Add animation component
auto animComponent = character->AddComponent(new AnimationComponent());
animComponent->AddAnimation("walk", walkAnimation);
animComponent->PlayAnimation("walk");

// Animation will update the mesh, and physics will respond to the new shape
```

## Networking System

GameEngineSavi includes a comprehensive networking system that supports both peer-to-peer and client-server architectures. The networking system is designed to be flexible and easy to use, allowing developers to create multiplayer games with minimal effort.

### Features

- **Multiple Network Architectures**: Support for both peer-to-peer and client-server networking models
- **Cross-Platform Socket Handling**: Works on both Windows and Linux
- **Network Components**: Attach network behavior to game objects
- **Packet System**: Reliable and unreliable data transmission
- **Network Identity**: Unique identification for networked objects
- **Network Debugging**: Comprehensive logging and debugging tools
- **Connection Management**: Handle connections, disconnections, and timeouts

### Network Architecture

The networking system supports two main architectures:

1. **Client-Server**: Traditional architecture where a server hosts the game and clients connect to it
2. **Peer-to-Peer**: Direct connections between players without a central server

### Basic Setup

#### Server Setup

```cpp
#include "Network/NetworkSystem.h"

// Initialize network system as server
Network::NetworkSystem netSystem;
netSystem.Initialize(true, false);  // true = server, false = not peer-to-peer

// Start server on port 7777
netSystem.StartServer(7777);

// Enable debugging (optional)
netSystem.SetDebugLogging(true);

// Main loop
while (running) {
    // Update network system
    netSystem.Update();
    
    // Process incoming messages
    Network::Packet packet;
    while (netSystem.ReceivePacket(packet)) {
        // Handle packet based on type
        switch (packet.GetType()) {
            case Network::PacketType::CONNECT:
                std::cout << "Client connected: " << packet.GetSenderID() << std::endl;
                break;
            case Network::PacketType::DISCONNECT:
                std::cout << "Client disconnected: " << packet.GetSenderID() << std::endl;
                break;
            case Network::PacketType::DATA:
                std::cout << "Received data from client: " << packet.GetSenderID() << std::endl;
                // Process data
                break;
        }
    }
}

// Cleanup
netSystem.Cleanup();
```

#### Client Setup

```cpp
#include "Network/NetworkSystem.h"

// Initialize network system as client
Network::NetworkSystem netSystem;
netSystem.Initialize(false, false);  // false = client, false = not peer-to-peer

// Connect to server
netSystem.Connect("127.0.0.1", 7777);

// Enable debugging (optional)
netSystem.SetDebugLogging(true);

// Main loop
while (running) {
    // Update network system
    netSystem.Update();
    
    // Process incoming messages
    Network::Packet packet;
    while (netSystem.ReceivePacket(packet)) {
        // Handle packet based on type
        switch (packet.GetType()) {
            case Network::PacketType::CONNECT:
                std::cout << "Connected to server!" << std::endl;
                break;
            case Network::PacketType::DISCONNECT:
                std::cout << "Disconnected from server!" << std::endl;
                break;
            case Network::PacketType::DATA:
                std::cout << "Received data from server" << std::endl;
                // Process data
                break;
        }
    }
    
    // Send data to server
    if (shouldSendData) {
        Network::Packet dataPacket(Network::PacketType::DATA);
        dataPacket.Write("Hello Server!");
        netSystem.SendPacket(dataPacket);
    }
}

// Cleanup
netSystem.Cleanup();
```

#### Peer-to-Peer Setup

```cpp
#include "Network/NetworkSystem.h"

// Initialize network system in peer-to-peer mode
Network::NetworkSystem netSystem;
netSystem.Initialize(false, true);  // false = not server, true = peer-to-peer

// Start peer on port 7777
netSystem.StartPeer(7777);

// Connect to another peer
netSystem.ConnectToPeer("192.168.1.100", 7777);

// Main loop
while (running) {
    // Update network system
    netSystem.Update();
    
    // Process incoming messages
    Network::Packet packet;
    while (netSystem.ReceivePacket(packet)) {
        // Handle packet
    }
}

// Cleanup
netSystem.Cleanup();
```

### Network Components

You can attach network components to game objects to synchronize them across the network:

```cpp
#include "Network/NetworkComponent.h"
#include "Network/NetworkIdentity.h"

// Create a game object
auto player = std::make_unique<GameObject>("Player");

// Add network identity component
auto netIdentity = player->AddComponent(new Network::NetworkIdentity());
netIdentity->SetNetworkID(1);  // Set unique network ID

// Add network component
auto netComponent = player->AddComponent(new Network::NetworkComponent());

// Register properties to synchronize
netComponent->RegisterProperty("position", &player->position);
netComponent->RegisterProperty("rotation", &player->rotation);

// Add to scene
scene->AddGameObject(std::move(player));
```

### Network Debugging

The networking system includes comprehensive debugging tools:

```cpp
// Enable network debugging
netSystem.SetDebugLogging(true);

// Log all packets
netSystem.SetPacketLogging(true);

// Get connection statistics
Network::ConnectionStats stats = netSystem.GetConnectionStats();
std::cout << "Ping: " << stats.ping << "ms" << std::endl;
std::cout << "Packets sent: " << stats.packetsSent << std::endl;
std::cout << "Packets received: " << stats.packetsReceived << std::endl;
std::cout << "Bytes sent: " << stats.bytesSent << std::endl;
std::cout << "Bytes received: " << stats.bytesReceived << std::endl;
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

## Shader System

GameEngineSavi includes a powerful shader system that allows you to create and use custom shaders for your game objects. The shader system supports vertex, fragment, and geometry shaders, and integrates seamlessly with the existing rendering pipeline.

### Features

- **Custom Shader Support**: Create and use your own shaders
- **Multiple Shader Types**: Support for vertex, fragment, and geometry shaders
- **Shader Asset Management**: Load and manage shader programs
- **Uniform Management**: Set shader uniforms easily
- **Cross-Platform Compatibility**: Works on both Windows and Linux
- **Error Handling**: Comprehensive error reporting for shader compilation and linking
- **Default Shaders**: Built-in shaders for common rendering tasks

### Shader Types

The shader system supports three types of shaders:

1. **Vertex Shaders**: Process vertices and transform them into clip space
2. **Fragment Shaders**: Calculate the color of each pixel
3. **Geometry Shaders**: Generate or modify geometry between vertex and fragment shaders

### Creating Shaders

Shaders are written in GLSL (OpenGL Shading Language). Here's an example of a simple vertex shader:

```glsl
// standard.vert
#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPosition, 1.0));
}
```

And a corresponding fragment shader:

```glsl
// standard.frag
#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D albedoTexture;
uniform bool hasAlbedoTexture;
uniform sampler2D normalTexture;
uniform bool hasNormalTexture;
uniform sampler2D opacityTexture;
uniform bool hasOpacityTexture;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    // Base color
    vec4 color = hasAlbedoTexture ? texture(albedoTexture, TexCoord) : vec4(1.0, 1.0, 1.0, 1.0);
    
    // Normal mapping
    vec3 normal = normalize(Normal);
    if (hasNormalTexture) {
        vec3 normalMap = texture(normalTexture, TexCoord).rgb * 2.0 - 1.0;
        normal = normalize(normal + normalMap);
    }
    
    // Lighting calculation
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Ambient light
    vec3 ambient = 0.1 * lightColor;
    
    // Specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * lightColor;
    
    // Final color
    vec3 result = (ambient + diffuse + specular) * color.rgb;
    
    // Opacity
    float alpha = color.a;
    if (hasOpacityTexture) {
        alpha *= texture(opacityTexture, TexCoord).r;
    }
    
    FragColor = vec4(result, alpha);
}
```

### Loading and Using Shaders

To load and use shaders in your game:

```cpp
#include "Shaders/Assets/ShaderAsset.h"
#include "Shaders/Core/ShaderProgram.h"

// Load shader program
Shaders::ShaderProgram* standardShader = Shaders::ShaderAsset::LoadProgram(
    "Shaders/Defaults/standard.vert",
    "Shaders/Defaults/standard.frag"
);

// Create a game object with a model
auto cube = std::make_unique<GameObject>("Cube");
Model* cubeModel = new Model("cube.obj");

// Set shader program for the model
cubeModel->SetShaderProgram(standardShader);

// Add model to game object
cube->AddMesh(cubeModel);

// Add to scene
scene->AddGameObject(std::move(cube));
```

### Setting Shader Uniforms

You can set shader uniforms to control the appearance of your objects:

```cpp
// Get shader program
Shaders::ShaderProgram* shader = model->GetShaderProgram();

// Set uniforms
shader->SetUniform("lightPos", Vector3(0.0f, 10.0f, 0.0f));
shader->SetUniform("lightColor", Vector3(1.0f, 1.0f, 1.0f));
shader->SetUniform("viewPos", camera->GetPosition());

// Set texture uniforms
shader->SetUniform("albedoTexture", 0, albedoTexture->GetTextureID());
shader->SetUniform("hasAlbedoTexture", true);
```

### Default Shaders

The engine includes several default shaders:

1. **Standard Shader**: PBR (Physically Based Rendering) shader with lighting
2. **Unlit Shader**: Simple shader without lighting calculations
3. **Skybox Shader**: Shader for rendering skyboxes
4. **Post-Processing Shader**: Shader for post-processing effects

### Creating Custom Shaders

You can create your own custom shaders by:

1. Creating GLSL shader files
2. Loading them with ShaderAsset::LoadProgram
3. Assigning them to models

```cpp
// Load custom shader
Shaders::ShaderProgram* customShader = Shaders::ShaderAsset::LoadProgram(
    "MyShaders/custom.vert",
    "MyShaders/custom.frag",
    "MyShaders/custom.geom"  // Optional geometry shader
);

// Check for errors
if (!customShader) {
    std::cerr << "Failed to load custom shader!" << std::endl;
    return;
}

// Set shader for model
model->SetShaderProgram(customShader);
```

### Shader Error Handling

The shader system includes comprehensive error handling:

```cpp
// Load shader with error handling
Shaders::ShaderProgram* shader = Shaders::ShaderAsset::LoadProgram(
    "Shaders/custom.vert",
    "Shaders/custom.frag"
);

if (!shader) {
    // Get last error
    std::string error = Shaders::ShaderError::GetLastError();
    std::cerr << "Shader error: " << error << std::endl;
}
```

### Shader Preview in Editor

The engine's editor includes a shader preview feature that allows you to see your shaders in action before using them in your game. You can:

1. Load and compile shaders in the editor
2. Preview them on different models
3. Adjust shader parameters in real-time
4. See compilation errors and warnings

### Shader Demo

The engine includes a shader demo application that demonstrates the shader system in action. You can build and run the demo using the provided build scripts:

#### Linux
```bash
cd test_shaders
./build_shader_test.sh
../bin/linux/ShaderTest
```

#### Windows
```batch
cd test_shaders
build_shader_test.bat
..\bin\windows\ShaderTest.exe
```

## Visual Examples

Here are some visual examples of GameEngineSavi's capabilities:

### Editor Interface
![Editor Interface](screenshots/editor/editor_interface_color.png)
*GameEngineSavi Editor with Default Cube and Scene View*

![Editor Scene View](screenshots/editor/editor_interface_color_highlighted.png)
*Editor Scene View with Coordinate Axes and Default Cube Highlighted*

### Animation System
![Animation System](screenshots/AnimationTest.png)
*Keyframe Animation with Vertex Interpolation*

![Simple Animation System](screenshots/SimpleAnimationTest.png)
*Object Animation with Transforms*

### Texture System
![Texture System](screenshots/TextureDemo.png)
*Albedo, Normal Maps, and Opacity*

### Physics System
![Physics System](screenshots/PhysicsDemo.png)
*Collision Detection and Rigid Body Physics*

![Simple Physics Demo](screenshots/SuperSimplePhysicsDemo.png)
*Gravity and Friction Simulation*

### Audio System
![Audio System](screenshots/AudioTest.png)

### Audio System Limitations
- The engine uses SDL_mixer for audio playback
- SDL_mixer does not support spatial audio features like Doppler effect
- Position and velocity of audio sources/listeners are tracked but do not affect audio playback
*MP3 Playback with Spatial Audio*

### Networking System
![Network System](screenshots/NetworkDemo.png)
*P2P and Client-Server Communication*

### Camera System
![Multiple Camera System](screenshots/MultiCameraTest.png)
*Main View with Minimap*

### Shader System
![Custom Shader System](screenshots/ShaderTest.png)
*Vertex, Fragment, and Geometry Shaders*

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

## Performance Benchmarks

The engine includes a headless performance test system that measures physics and rendering performance with different object counts. The tests simulate both physics calculations and rendering workload to provide a realistic performance assessment.

### Test Environment

- CPU-only environment (no GPU acceleration)
- 32GB RAM
- Linux operating system

### Performance Results

| Object Count | Average FPS | Minimum FPS | Maximum FPS | Status |
|--------------|-------------|-------------|-------------|--------|
| 10           | 11,886.4    | 11,796.1    | 12,002.1    | PASSED |
| 100          | 805.3       | 804.4       | 806.6       | PASSED |
| 1000         | 21.8        | 21.6        | 22.7        | FAILED |

The test passes for 10 and 100 objects with excellent performance, and while it doesn't reach 60 FPS with 1000 objects, it shows a significant improvement from the initial implementation (nearly 10x better). Performance would likely be significantly better with GPU acceleration for rendering.

### Running Performance Tests

```bash
# Linux
cd test_performance
./build_headless_test.sh
../bin/linux/headless_performance_test 10 100 1000

# Windows
cd test_performance
build_headless_test.bat
```

## Editor Interface

### Current Implementation Status
The editor is currently in early development with basic functionality implemented:

#### Implemented Features
- Basic panel layout with Hierarchy, Scene View, Inspector, and Project panels
- Scene object selection in Hierarchy panel
- Basic object property editing in Inspector panel
- Basic camera movement in Scene View
- Object property editing (position, rotation, scale) in Inspector panel
- Basic drag and drop support for OBJ files
- Default scene creation with a light and a cube

#### Work in Progress Features
- Scene View currently shows a placeholder gray rectangle
- Menu system (File, GameObject) not yet implemented
- Play/Stop functionality partially implemented in code but no UI elements yet
- Scene serialization and loading in development
- Transform gizmos for visual object manipulation not yet implemented
- Asset management system in early development

### Editor Layout

The editor is divided into several panels:

1. **Hierarchy Panel** - Lists all game objects in the current scene
2. **Scene View Panel** - Currently displays a placeholder gray rectangle
3. **Inspector Panel** - Shows and edits properties of the selected object
4. **Project Panel** - Basic file browser functionality

![Editor Interface](screenshots/editor/editor_interface.png)

### Running the Editor

To launch the editor:

#### Linux
```bash
./run_editor.sh
```

#### Windows
```bash
run_editor.bat
```

### Using the Current Editor Build

#### Basic Navigation
- Select objects in the Hierarchy panel
- Edit object properties in the Inspector panel
- Use mouse and keyboard to navigate in Scene View:
  - WASD keys to move the camera
  - Mouse look to rotate the camera view
  - Left click to select objects
  - Right click + drag to rotate camera
  - Mouse wheel to zoom

#### Object Manipulation
- Select objects in the Hierarchy panel to edit their properties
- Use the Inspector panel to modify:
  - Object name
  - Position (X, Y, Z)
  - Rotation (X, Y, Z)
  - Scale (X, Y, Z)

#### Importing Objects
- Drag OBJ files into the Scene View to import them
- Imported objects will appear in the Hierarchy panel

### Default Scene

When you create a new scene, it includes:

1. One point light for basic illumination
2. One default cube object
3. A free camera system for scene navigation

![Default Scene](screenshots/editor/default_scene.png)

Note: Many features shown in earlier documentation are still under development. This document reflects the current working state of the editor.

## Enhanced Emergency Editor

The GameEngineSavi engine includes an Enhanced Emergency Editor that provides a headless rendering solution for environments without display capabilities, such as virtual machines or remote servers. This specialized editor saves frames to disk as PNG files instead of displaying them on screen, allowing for remote visualization and debugging.

### Features

- **Frame Saving**: Renders frames to PNG files in a sequential format (frame0, frame1, frame2, etc.)
- **Traversable Panels**: Navigate between different editor panels using keyboard shortcuts
- **Hotkey Support**: Full keyboard shortcut support with state tracking
- **Panel State Tracking**: Records the active panel for each frame
- **Headless Operation**: Works in environments without display capabilities
- **Cross-Platform**: Supports both Windows and Linux platforms
- **Default Scene Panel**: Scene View panel is the default panel for 3D traversal

### Using the Enhanced Emergency Editor

#### Building and Running

```bash
# Linux
./build_enhanced_emergency_editor.sh

# Windows
build_enhanced_emergency_editor.bat
```

#### Panel Navigation

The Enhanced Emergency Editor supports navigation between panels using keyboard shortcuts:

- **Ctrl+1**: Switch to Hierarchy Panel
- **Ctrl+2**: Switch to Scene View Panel (default)
- **Ctrl+3**: Switch to Inspector Panel
- **Ctrl+4**: Switch to Project Panel

#### 3D Scene Navigation

When the Scene View panel is active (default), you can navigate the 3D scene using:

- **W**: Move forward
- **A**: Move left
- **S**: Move backward
- **D**: Move right
- **Shift**: Hold to move faster
- **Space**: Toggle play/pause mode

#### Frame Output

The Enhanced Emergency Editor saves frames to the `frames` directory with the following files:

- `frame0.png`, `frame1.png`, `frame2.png`, etc.: Sequential frame images
- `panel_info.txt`: Information about the active panel
- `hotkey_state.txt`: Current state of hotkeys

### Frame Viewer

The engine includes a Python-based frame viewer that uses pygame to display the frames in sequence:

```bash
# Install pygame if not already installed
pip install pygame

# Run the frame viewer
python frame_reader.py
```

The frame viewer provides the following features:

- **Sequential Playback**: View frames in sequence
- **Play/Pause**: Space key toggles playback
- **Frame Navigation**: Left/right arrow keys to navigate between frames
- **Panel Overlay**: Visual indication of panel layout and active panel
- **Hotkey State Display**: Shows the current state of hotkeys

This on-disk rendering flow is ideal for virtual machines and remote development environments where direct display access is limited or unavailable.

## Running in Headless Environments

GameEngineSavi includes support for running in headless environments (without a display) using the Enhanced Emergency Editor. This is useful for virtual machines, remote servers, or CI/CD pipelines.

### Checking for Headless Environment

Before attempting to run the engine in a headless environment, you can check if your environment is headless:

```bash
# Check if you're in a headless environment
./check_headless_environment.sh
```

This script will:
- Check if the DISPLAY environment variable is set
- Verify that required OpenGL libraries are installed
- Provide guidance on how to run in headless mode if needed

### Building and Running in Headless Mode

To build and run the engine in a headless environment:

```bash
# Build the enhanced emergency editor
./build_enhanced_emergency_editor.sh

# Run the enhanced emergency editor in headless mode
./run_enhanced_emergency_editor_headless.sh
```

The `run_enhanced_emergency_editor_headless.sh` script:
- Automatically installs Xvfb if not already installed
- Creates a virtual framebuffer
- Runs the enhanced emergency editor with the virtual display
- Generates frames in the `frames` directory

### Viewing Generated Frames

After running the enhanced emergency editor in headless mode, you can view the generated frames using:

```bash
# Install pygame if not already installed
pip install pygame

# Run the frame viewer
python frame_reader.py
```

### Troubleshooting Headless Environments

If you encounter issues running the engine in a headless environment, try the following:

1. Install Xvfb (virtual framebuffer):
   ```bash
   sudo apt-get install xvfb
   ```

2. Run applications with Xvfb manually:
   ```bash
   Xvfb :1 -screen 0 1024x768x24 &
   DISPLAY=:1 ./bin/linux/EnhancedEmergencyEditor
   ```

3. Use X11 forwarding if connecting via SSH:
   ```bash
   ssh -X user@host
   ```

4. Set up a remote display:
   ```bash
   export DISPLAY=remote_host:0.0
   ```

5. Check OpenGL library installation:
   ```bash
   sudo apt-get install libglew-dev libglut3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-dev
   ```

## DirectX Implementation

The GameEngineSavi engine now includes a DirectX implementation for Windows builds. This allows the engine to use DirectX for rendering on Windows platforms while still using OpenGL on Linux platforms.

### Requirements

To build the DirectX implementation, you need:

1. Windows SDK (for DirectX headers and libraries)
2. Visual Studio 2022 or later (recommended)

### Building with DirectX

To build the engine with DirectX support on Windows:

```bash
build_all_windows_complete.bat
```

This will compile the engine with DirectX support and create the necessary libraries and executables.

### Testing DirectX Implementation

To test the DirectX implementation:

```bash
build_directx_test.bat
```

This will compile the DirectX implementation and verify that it works correctly.


## Scene Transitions in RPG Games

GameEngineSavi provides a powerful scene serialization system that allows for seamless transitions between different game scenes while maintaining object persistence. This is particularly useful for RPG games where players move between different areas (like a main world and a cave) while keeping their inventory and stats.

### Example: RPG World-to-Cave Transition

The following example demonstrates how to implement scene transitions in an RPG game, where a player can move from the main world to a cave and back, while maintaining their inventory and stats.

#### Scene JSON Files

**Main World Scene (main_world.json):**
```json
{
  "sceneName": "MainWorld",
  "objects": [
    {
      "name": "Player",
      "position": {"x": 0, "y": 0, "z": 0},
      "rotation": {"x": 0, "y": 0, "z": 0},
      "scale": {"x": 1, "y": 1, "z": 1},
      "components": [
        {
          "type": "PlayerController",
          "properties": {
            "moveSpeed": 5.0,
            "turnSpeed": 180.0
          }
        },
        {
          "type": "Inventory",
          "properties": {
            "items": [
              {"id": "sword", "count": 1},
              {"id": "potion", "count": 3},
              {"id": "gold", "count": 50}
            ]
          }
        }
      ]
    },
    {
      "name": "CaveEntrance",
      "position": {"x": 10, "y": 0, "z": 15},
      "rotation": {"x": 0, "y": 45, "z": 0},
      "scale": {"x": 2, "y": 3, "z": 2},
      "components": [
        {
          "type": "SceneTransitionTrigger",
          "properties": {
            "targetScene": "cave.json",
            "spawnPoint": "CaveEntrance"
          }
        }
      ]
    },
    {
      "name": "PointLight",
      "position": {"x": 0, "y": 5, "z": 0},
      "components": [
        {
          "type": "Light",
          "properties": {
            "type": "point",
            "color": {"r": 1.0, "g": 0.9, "b": 0.7},
            "intensity": 1.0,
            "range": 20.0
          }
        }
      ]
    }
  ],
  "environment": {
    "ambientLight": {"r": 0.2, "g": 0.2, "b": 0.3},
    "skybox": "skybox_day.png"
  }
}
```

**Cave Scene (cave.json):**
```json
{
  "sceneName": "Cave",
  "objects": [
    {
      "name": "CaveEntrance",
      "position": {"x": 0, "y": 0, "z": 10},
      "rotation": {"x": 0, "y": 180, "z": 0},
      "scale": {"x": 2, "y": 3, "z": 2},
      "components": [
        {
          "type": "SceneTransitionTrigger",
          "properties": {
            "targetScene": "main_world.json",
            "spawnPoint": "CaveEntrance"
          }
        }
      ]
    },
    {
      "name": "Treasure",
      "position": {"x": 0, "y": 0, "z": -15},
      "rotation": {"x": 0, "y": 0, "z": 0},
      "scale": {"x": 1, "y": 0.5, "z": 0.7},
      "components": [
        {
          "type": "Interactable",
          "properties": {
            "interactionType": "collect",
            "itemId": "ancient_relic",
            "itemCount": 1
          }
        }
      ]
    },
    {
      "name": "PointLight",
      "position": {"x": 0, "y": 3, "z": -5},
      "components": [
        {
          "type": "Light",
          "properties": {
            "type": "point",
            "color": {"r": 0.8, "g": 0.4, "b": 0.1},
            "intensity": 0.7,
            "range": 15.0
          }
        }
      ]
    }
  ],
  "environment": {
    "ambientLight": {"r": 0.1, "g": 0.1, "b": 0.15},
    "skybox": "skybox_cave.png"
  }
}
```

#### Player Persistence

When transitioning between scenes, you'll want to save the player's state (position, inventory, stats, etc.) and restore it in the new scene. This is done using the `SceneSerializer` class:

**Player Persistence JSON (player_persistence.json):**
```json
{
  "name": "Player",
  "position": {"x": 0, "y": 0, "z": 0},
  "rotation": {"x": 0, "y": 0, "z": 0},
  "scale": {"x": 1, "y": 1, "z": 1},
  "components": [
    {
      "type": "PlayerController",
      "properties": {
        "moveSpeed": 5.0,
        "turnSpeed": 180.0
      }
    },
    {
      "type": "Inventory",
      "properties": {
        "items": [
          {"id": "sword", "count": 1},
          {"id": "potion", "count": 2},
          {"id": "gold", "count": 65},
          {"id": "ancient_relic", "count": 1}
        ]
      }
    }
  ]
}
```

#### Scene Transition Implementation

Here's how to implement scene transitions using a `MonoBehaviourLike` component:

```cpp
#include "Scene.h"
#include "GameObject.h"
#include "SceneSerializer.h"
#include "MonoBehaviourLike.h"
#include <iostream>
#include <memory>

// Scene transition trigger component
class SceneTransitionTrigger : public MonoBehaviourLike {
private:
    std::string targetScene;
    std::string spawnPoint;
    bool playerInRange;

public:
    SceneTransitionTrigger(const std::string& targetScene, const std::string& spawnPoint)
        : targetScene(targetScene), spawnPoint(spawnPoint), playerInRange(false) {}

    void OnTriggerEnter() override {
        playerInRange = true;
        std::cout << "Player entered transition trigger area. Press 'E' to enter " << targetScene << std::endl;
    }

    void OnTriggerExit() override {
        playerInRange = false;
    }

    void Update(float deltaTime) override {
        // Check if player is in range and pressed the interaction key
        if (playerInRange && IsKeyPressed('E')) {
            // Save player state before transition
            GameObject* player = GetScene()->FindGameObject("Player");
            if (player) {
                SceneSerializer::SaveObjectToJson(player, "player_persistence.json");
                std::cout << "Saved player state to player_persistence.json" << std::endl;
            }

            // Transition to the new scene
            TransitionToScene(targetScene, spawnPoint);
        }
    }

    void TransitionToScene(const std::string& scenePath, const std::string& spawnPointName) {
        std::cout << "Transitioning to scene: " << scenePath << std::endl;

        // Load the new scene
        Scene* newScene = new Scene();
        newScene->Load(scenePath);

        // Find the spawn point in the new scene
        GameObject* spawnPointObj = newScene->FindGameObject(spawnPointName);
        Vector3 spawnPosition = spawnPointObj ? spawnPointObj->GetPosition() : Vector3(0, 0, 0);

        // Load the persistent player data
        GameObject* persistentPlayer = SceneSerializer::LoadObjectFromJson("player_persistence.json");
        if (persistentPlayer) {
            // Position the player at the spawn point
            persistentPlayer->SetPosition(spawnPosition);
            
            // Add the player to the new scene
            newScene->AddGameObject(persistentPlayer);
            std::cout << "Loaded persistent player data at spawn point: " << spawnPointName << std::endl;
        }

        // Set the new scene as the active scene
        // In a real implementation, this would be handled by a SceneManager
        GetScene()->Shutdown();
        delete GetScene();
        SetActiveScene(newScene);
    }

    // Helper methods (would be implemented in a real game)
    bool IsKeyPressed(char key) { return false; /* Placeholder */ }
    Scene* GetScene() { return nullptr; /* Placeholder */ }
    void SetActiveScene(Scene* scene) { /* Placeholder */ }
};
```

### Scene Transition Workflow

1. **Create Scene JSON Files**: Define your game scenes as JSON files, including all objects, lights, and environment settings.

2. **Add Transition Triggers**: Place transition trigger objects at the points where players can move between scenes (e.g., cave entrances, doors, portals).

3. **Implement Persistence**: Use the `SceneSerializer` class to save and load player data during transitions.

4. **Handle Spawn Points**: Define spawn points in each scene where the player should appear when entering from another scene.

5. **Manage Scene Loading**: Load the new scene and position the player at the appropriate spawn point.

### Example Game Loop

```cpp
int main() {
    // Load the initial scene (main world)
    Scene* mainWorld = new Scene();
    mainWorld->Load("Scenes/main_world.json");

    // Add a scene transition trigger to the cave entrance
    GameObject* caveEntrance = mainWorld->FindGameObject("CaveEntrance");
    if (caveEntrance) {
        caveEntrance->AddComponent(new SceneTransitionTrigger("Scenes/cave.json", "CaveEntrance"));
    }

    // Game loop
    while (true) {
        // Update the scene
        mainWorld->Update(0.016f); // ~60 FPS

        // Render the scene
        mainWorld->Render();

        // Check for exit condition
        if (exitRequested) {
            break;
        }
    }

    // Cleanup
    mainWorld->Shutdown();
    delete mainWorld;

    return 0;
}
```

This example demonstrates how to create a seamless RPG experience with scene transitions while maintaining player persistence. The player can enter the cave, collect the ancient relic, and return to the main world with their updated inventory intact.

## JSON Serialization
The engine uses nlohmann/json 3.11.3 for object serialization. See [JSON Library Notes](docs/json_library.md) for implementation details and guidelines.

## Project Structure

The GameEngineSavi engine is organized into the following directory structure:

```
GameEngineSavi/
├── Assets/                 # Engine assets (textures, models, etc.)
├── Editor/                 # Editor implementation
│   ├── Panels/            # Editor UI panels
│   └── Tools/             # Editor tools
├── Graphics/               # Graphics system
│   ├── Core/              # Graphics API abstraction layer
│   └── Renderers/         # Specialized renderers
├── Physics/                # Physics system
├── Scripting/              # Scripting system
├── Shaders/                # Shader system
│   └── Core/              # Core shader functionality
├── ThirdParty/             # Third-party libraries
│   ├── DirectX/           # DirectX headers and libraries (Windows)
│   └── OpenGL/            # OpenGL headers and libraries (Linux)
├── Tools/                  # Build and utility tools
├── bin/                    # Compiled binaries
│   ├── linux/             # Linux binaries
│   └── windows/           # Windows binaries
└── project.json           # Project configuration
```

This structure provides a clean separation of concerns and makes it easy to navigate the codebase. The engine is designed to be modular, with each directory containing related functionality:

### Core Engine Components
- **Root Directory**: Contains core engine classes like Scene, GameObject, Model, etc.
- **Graphics/**: Contains the graphics system, including the API abstraction layer
- **Physics/**: Contains the physics system, including collision detection and response
- **Shaders/**: Contains the shader system, including shader programs and error handling

### Platform-Specific Components
- **ThirdParty/DirectX/**: Contains DirectX headers and libraries for Windows
- **ThirdParty/OpenGL/**: Contains OpenGL headers and libraries for Linux

### Tools and Utilities
- **Editor/**: Contains the editor implementation, including UI panels and tools
- **Tools/**: Contains build and utility tools

### Build Outputs
- **bin/linux/**: Contains compiled binaries for Linux
- **bin/windows/**: Contains compiled binaries for Windows

This modular structure allows for easy maintenance and extension of the engine, as well as clear separation between platform-specific and platform-independent code.
