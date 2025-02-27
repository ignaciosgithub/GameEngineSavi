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

## Contributing

Contributions to GameEngineSavi are welcome! Feel free to submit pull requests or open issues to improve the engine.
