# Visual Studio 2022 Setup for GameEngineSavi

This document provides instructions for setting up and using GameEngineSavi with Visual Studio 2022.

## Prerequisites

1. Visual Studio 2022 (Community, Professional, or Enterprise edition)
2. C++ Desktop Development workload installed
3. Windows 10 or 11

## Required Dependencies

The following dependencies are required to build and run GameEngineSavi:

1. SDL2 (Simple DirectMedia Layer)
2. SDL2_mixer (for audio support)
3. OpenGL libraries (included with Windows)

## Setup Instructions

### 1. Clone the Repository

```bash
git clone https://github.com/ignaciosgithub/GameEngineSavi.git
cd GameEngineSavi
```

### 2. Open the Solution

1. Launch Visual Studio 2022
2. Open the solution file: `GameEngineSavi.sln`

### 3. Install Dependencies

#### Option 1: Using vcpkg (Recommended)

1. Install vcpkg if you haven't already:
```bash
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
bootstrap-vcpkg.bat
vcpkg integrate install
```

2. Install the required packages:
```bash
vcpkg install sdl2:x64-windows sdl2-mixer:x64-windows
```

#### Option 2: Manual Installation

1. Download SDL2 and SDL2_mixer development libraries for Windows from:
   - SDL2: https://www.libsdl.org/download-2.0.php
   - SDL2_mixer: https://www.libsdl.org/projects/SDL_mixer/

2. Extract the libraries to a known location

3. In Visual Studio, right-click on the project in Solution Explorer and select "Properties"
4. Navigate to "VC++ Directories"
5. Add the include and library directories for SDL2 and SDL2_mixer
6. Navigate to "Linker > Input"
7. Add the following libraries to "Additional Dependencies":
   - SDL2.lib
   - SDL2main.lib
   - SDL2_mixer.lib

### 4. Build and Run

1. Select the desired configuration (Debug/Release) and platform (x86/x64)
2. Build the solution (F7 or Ctrl+Shift+B)
3. Run the application (F5)

## Troubleshooting

### Common Issues

1. **Missing DLL errors when running**:
   - Copy the SDL2.dll and SDL2_mixer.dll files to the output directory
   - Alternatively, add the SDL2 bin directory to your system PATH

2. **Linker errors**:
   - Ensure that the library paths are correctly set in the project properties
   - Verify that the correct libraries are listed in "Additional Dependencies"

3. **Include errors**:
   - Check that the include paths are correctly set in the project properties

## Additional Notes

- The solution is configured to use C++14 standard
- The working directory is set to the solution directory
- OpenGL libraries (opengl32.lib, glu32.lib) are linked by default
