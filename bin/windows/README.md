# Windows Executables for GameEngineSavi

This directory contains Windows executables for the GameEngineSavi physics system demos.

## Available Executables

1. **PhysicsDemo.exe** - Full physics demo with two pyramids demonstrating:
   - Rigid body physics with variable mass
   - Collision detection and response
   - Variable friction coefficients (multiplicative with normal force)
   - Configurable gravity (can be enabled/disabled per object)
   - Torque calculation and application

2. **SuperSimplePhysicsDemo.exe** - Simplified physics demo that demonstrates the core physics concepts without requiring OpenGL.

## Building from Source

To build these executables from source:

1. Open a Visual Studio Developer Command Prompt
2. Navigate to the GameEngineSavi directory
3. Run one of the following build scripts:
   - `build_windows.bat` - Builds the full physics demo
   - `build_windows_simple.bat` - Builds the simplified physics demo

## Requirements

- Windows 10 or later
- Visual C++ Redistributable for Visual Studio 2019 or later
