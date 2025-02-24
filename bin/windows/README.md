# Windows Physics Demo Executables

## About the Placeholder Files
The files in this directory are placeholder files for Windows executables. Due to platform limitations, the actual executables cannot be compiled on a Linux machine. However, you can build them yourself using the provided build scripts.

## Building the Executables
To build the physics demo executables on Windows:

1. Open a Visual Studio Developer Command Prompt
2. Navigate to the GameEngineSavi directory
3. Run one of the following build scripts:
   - `build_windows.bat` - Builds the full physics demo
   - `build_windows_simple.bat` - Builds the simplified physics demo

## Running the Demos
After building the executables, you can run them using:
```
run_demo.bat
```

This script will automatically detect and run the appropriate physics demo executable.

## Demo Features
The physics demos demonstrate:
- Rigid body physics with variable mass
- Collision detection and response
- Variable friction coefficients
- Configurable gravity
- Torque calculation and application

## Linux Executables
For Linux users, pre-compiled executables are available in the `bin/linux` directory.
