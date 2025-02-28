# Visual Studio 2022 Setup and Usage Guide for GameEngineSavi

This comprehensive guide provides detailed instructions for setting up, building, debugging, and using the GameEngineSavi engine with Visual Studio 2022.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Installation](#installation)
3. [Project Setup](#project-setup)
4. [Building the Engine](#building-the-engine)
5. [Running and Debugging](#running-and-debugging)
6. [Project Structure](#project-structure)
7. [Creating a New Game](#creating-a-new-game)
8. [Advanced Features](#advanced-features)
9. [Troubleshooting](#troubleshooting)
10. [Performance Optimization](#performance-optimization)

## Prerequisites

### Required Software

1. **Visual Studio 2022**
   - Community, Professional, or Enterprise edition
   - Download from [Visual Studio website](https://visualstudio.microsoft.com/vs/)

2. **C++ Desktop Development Workload**
   - During Visual Studio installation, select "Desktop development with C++"
   - Ensure the following components are selected:
     - MSVC C++ x64/x86 build tools
     - Windows 10/11 SDK
     - C++ CMake tools for Windows
     - C++ core features

3. **Git**
   - Download from [Git website](https://git-scm.com/downloads)
   - Required for cloning the repository

### System Requirements

- Windows 10 or 11 (64-bit recommended)
- 8GB RAM minimum (16GB recommended)
- DirectX 11 compatible graphics card
- 2GB free disk space

## Installation

### Step 1: Clone the Repository

1. Open Command Prompt or Git Bash
2. Navigate to your desired directory
3. Clone the repository:
   ```bash
   git clone https://github.com/ignaciosgithub/GameEngineSavi.git
   cd GameEngineSavi
   ```

### Step 2: Install Dependencies

#### Option 1: Using vcpkg (Recommended)

1. Install vcpkg:
   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   bootstrap-vcpkg.bat
   vcpkg integrate install
   ```

2. Install required packages:
   ```bash
   vcpkg install sdl2:x64-windows sdl2-mixer:x64-windows
   ```

#### Option 2: Manual Installation

1. Download SDL2 and SDL2_mixer development libraries for Windows:
   - SDL2: https://www.libsdl.org/download-2.0.php
   - SDL2_mixer: https://www.libsdl.org/projects/SDL_mixer/

2. Extract the libraries to a known location (e.g., `C:\Libraries\SDL2`)

3. Copy the DLL files to your system path or the project's output directory:
   - For SDL2: `SDL2.dll`
   - For SDL2_mixer: `SDL2_mixer.dll`

## Project Setup

### Step 1: Open the Solution

1. Launch Visual Studio 2022
2. Select "Open a project or solution"
3. Navigate to the GameEngineSavi directory
4. Select `GameEngineSavi.sln` and click "Open"

### Step 2: Configure Project Properties (If Using Manual Installation)

1. Right-click on the "GameEngineSavi" project in Solution Explorer
2. Select "Properties"
3. Navigate to "VC++ Directories"
4. Add the include directories:
   - Add `C:\Libraries\SDL2\include` to "Include Directories"
   - Add `C:\Libraries\SDL2\lib\x64` to "Library Directories"

5. Navigate to "Linker > Input"
6. Add the following libraries to "Additional Dependencies":
   - SDL2.lib
   - SDL2main.lib
   - SDL2_mixer.lib

7. Click "Apply" and "OK"

### Step 3: Configure Build Configuration

1. Select the desired configuration:
   - Debug: For development with debugging information
   - Release: For optimized builds without debugging information

2. Select the desired platform:
   - x64: For 64-bit builds (recommended)
   - Win32: For 32-bit builds

## Building the Engine

### Step 1: Build the Solution

1. Press F7 or select "Build > Build Solution" from the menu
2. Check the "Output" window for build progress and errors
3. The build output will be located in:
   - Debug: `$(SolutionDir)bin\$(Platform)\Debug\`
   - Release: `$(SolutionDir)bin\$(Platform)\Release\`

### Step 2: Verify Build Output

1. Navigate to the output directory:
   - For x64 Debug: `bin\x64\Debug\`
   - For x64 Release: `bin\x64\Release\`

2. Verify that the following files exist:
   - GameEngineSavi.exe
   - Required DLL files (if using manual installation)

## Running and Debugging

### Running the Engine

1. Press F5 or select "Debug > Start Debugging" to run with debugging
2. Press Ctrl+F5 or select "Debug > Start Without Debugging" to run without debugging

### Debugging Tips

1. **Setting Breakpoints**:
   - Click in the left margin of the code editor to set a breakpoint
   - Press F9 on the current line to toggle a breakpoint

2. **Watching Variables**:
   - During debugging, hover over variables to see their values
   - Use the "Watch" window to monitor specific variables
   - Add variables to the watch window by right-clicking and selecting "Add Watch"

3. **Stepping Through Code**:
   - F10: Step Over (execute the current line and move to the next)
   - F11: Step Into (enter a function call)
   - Shift+F11: Step Out (exit the current function)
   - F5: Continue (resume execution)

4. **Debugging Windows**:
   - Locals: Shows variables in the current scope
   - Watch: Shows custom watched variables
   - Call Stack: Shows the function call hierarchy
   - Breakpoints: Lists all breakpoints
   - Output: Shows debug output

## Project Structure

The GameEngineSavi project is organized as follows:

```
GameEngineSavi/
├── Animation/            # Animation system
├── Audio/                # Audio system
├── Editor/               # Editor implementation
├── GUI/                  # GUI system
├── Network/              # Networking system
├── ProjectSettings/      # Project settings
├── Shaders/              # Shader system
│   ├── Assets/           # Shader assets
│   ├── Core/             # Core shader implementation
│   └── Defaults/         # Default shaders
├── test_animations/      # Animation test files
├── test_audio/           # Audio test files
├── test_network/         # Network test files
├── test_scene/           # Scene test files
├── test_shaders/         # Shader test files
├── bin/                  # Build output
│   ├── x64/              # 64-bit builds
│   │   ├── Debug/        # Debug builds
│   │   └── Release/      # Release builds
│   └── Win32/            # 32-bit builds
│       ├── Debug/        # Debug builds
│       └── Release/      # Release builds
└── GameEngineSavi.sln    # Visual Studio solution file
```

## Creating a New Game

### Step 1: Create a New Project

1. Create a new C++ project in Visual Studio
2. Add references to the GameEngineSavi project:
   - Right-click on your project in Solution Explorer
   - Select "Add > Reference"
   - Check "GameEngineSavi" and click "OK"

### Step 2: Set Up Your Game

1. Create a main.cpp file with the following structure:

```cpp
#include "Scene.h"
#include "GameObject.h"
#include "Model.h"
#include "Camera.h"
#include "PointLight.h"
#include "Vector3.h"

int main() {
    // Create a new scene
    std::unique_ptr<Scene> scene = std::make_unique<Scene>();

    // Load the scene
    scene->Load();

    // Create a game object
    std::unique_ptr<GameObject> player = std::make_unique<GameObject>(
        "Player",                  // Name
        Vector3(0, 0, 0),          // Position
        Vector3(0, 0, 0),          // Rotation
        Vector3(1, 1, 1)           // Size
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

    return 0;
}
```

### Step 3: Configure Project Settings

1. Set the working directory to the GameEngineSavi directory:
   - Right-click on your project in Solution Explorer
   - Select "Properties"
   - Navigate to "Debugging"
   - Set "Working Directory" to `$(SolutionDir)`

2. Configure include and library directories:
   - Navigate to "VC++ Directories"
   - Add the GameEngineSavi directory to "Include Directories"
   - Add the GameEngineSavi output directory to "Library Directories"

3. Configure linker settings:
   - Navigate to "Linker > Input"
   - Add `GameEngineSavi.lib` to "Additional Dependencies"

## Advanced Features

### Using the Animation System

```cpp
#include "Animation/Animation.h"
#include "Animation/AnimationComponent.h"
#include "Animation/AnimationLoader.h"

// Load animation from file
std::shared_ptr<Animation::Animation> walkAnimation = AnimationLoader::LoadFromFile("animations/walk.savanim");

// Create a game object
auto character = std::make_unique<GameObject>("Character");

// Add animation component
auto animComponent = character->AddComponent(new AnimationComponent());

// Add animation to component
animComponent->AddAnimation("walk", walkAnimation);

// Play animation
animComponent->Play("walk");

// Add to scene
scene->AddGameObject(std::move(character));
```

### Using the Networking System

```cpp
#include "Network/NetworkSystem.h"

// Initialize network system as server
Network::NetworkSystem netSystem;
netSystem.Initialize(true, false);  // true = server, false = not peer-to-peer

// Start server on port 7777
netSystem.StartServer(7777);

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

### Using the Shader System

```cpp
#include "Shaders/Core/Shader.h"
#include "Shaders/Core/ShaderProgram.h"

// Create shader program
auto shaderProgram = std::make_unique<Shaders::ShaderProgram>();

// Load shaders
shaderProgram->LoadShader("Shaders/Defaults/standard.vert", Shaders::ShaderType::VERTEX);
shaderProgram->LoadShader("Shaders/Defaults/standard.frag", Shaders::ShaderType::FRAGMENT);

// Compile and link
shaderProgram->Compile();
shaderProgram->Link();

// Use shader program
shaderProgram->Use();

// Set uniforms
shaderProgram->SetUniform("viewPos", camera->GetPosition());
shaderProgram->SetUniform("lightColor", Vector3(1, 1, 1));
```

## Troubleshooting

### Common Build Errors

1. **Missing DLL errors when running**:
   - Copy the SDL2.dll and SDL2_mixer.dll files to the output directory
   - Alternatively, add the SDL2 bin directory to your system PATH

2. **Linker errors**:
   - Ensure that the library paths are correctly set in the project properties
   - Verify that the correct libraries are listed in "Additional Dependencies"

3. **Include errors**:
   - Check that the include paths are correctly set in the project properties

4. **Platform-specific errors**:
   - Ensure that the correct platform (x64/Win32) is selected
   - Check that the correct platform toolset is selected in project properties

### Runtime Errors

1. **Access violation**:
   - Check for null pointers
   - Verify memory management (use of new/delete, smart pointers)
   - Check array bounds

2. **Missing textures or models**:
   - Verify file paths
   - Check working directory settings
   - Ensure files are included in the project

3. **OpenGL errors**:
   - Update graphics drivers
   - Check OpenGL version compatibility
   - Verify shader syntax

### Performance Issues

1. **Low frame rate**:
   - Use Release configuration for better performance
   - Optimize rendering code
   - Reduce number of objects or lights
   - Simplify shaders

2. **Memory leaks**:
   - Use smart pointers (std::unique_ptr, std::shared_ptr)
   - Check for proper cleanup in destructors
   - Use the Visual Studio memory profiler

## Performance Optimization

### Build Configuration

1. **Release Mode**:
   - Use Release configuration for final builds
   - Enable optimizations in project properties:
     - Navigate to "C/C++ > Optimization"
     - Set "Optimization" to "Maximum Optimization (Favor Speed) (/O2)"
     - Set "Inline Function Expansion" to "Any Suitable (/Ob2)"

2. **Profile-Guided Optimization**:
   - Navigate to "C/C++ > Optimization"
   - Set "Profile Guided Optimization" to "Instrument (/LTCG:PGI)"
   - Run your application to generate profile data
   - Rebuild with "Optimize (/LTCG:PGO)"

### Code Optimization

1. **Use efficient data structures**:
   - Prefer contiguous memory (std::vector over std::list)
   - Use appropriate containers for your use case

2. **Minimize memory allocations**:
   - Pre-allocate memory where possible
   - Use object pooling for frequently created/destroyed objects

3. **Optimize rendering**:
   - Use frustum culling
   - Implement level of detail (LOD)
   - Batch similar draw calls

4. **Multithreading**:
   - Use std::thread for parallel processing
   - Separate physics, rendering, and logic into different threads
   - Use thread pools for task-based parallelism

### Profiling

1. **Visual Studio Profiler**:
   - Select "Debug > Performance Profiler"
   - Choose "CPU Usage" or "Memory Usage"
   - Run your application and analyze the results

2. **External Profilers**:
   - Intel VTune
   - AMD CodeXL
   - NVIDIA Nsight

## Conclusion

This guide provides a comprehensive overview of setting up, building, debugging, and using the GameEngineSavi engine with Visual Studio 2022. For more information, refer to the engine's documentation and source code.

For additional help, visit the [GameEngineSavi GitHub repository](https://github.com/ignaciosgithub/GameEngineSavi).
