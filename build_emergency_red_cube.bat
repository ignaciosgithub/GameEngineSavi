@echo off
REM Build script for the emergency red cube renderer

echo Building emergency red cube renderer...

REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g
set INCLUDES=-I. -IThirdParty/OpenGL/include -IThirdParty/DirectX/include

REM Set DirectX flag to true by default
set USE_DIRECTX=true

REM First, try to compile with DirectX
echo Attempting to compile with DirectX support...
g++ %CFLAGS% %INCLUDES% -o bin\windows\EmergencyEditor_red_cube.exe ^
    Editor\EmergencyEditor_red_cube.cpp ^
    Vector3.cpp ^
    Matrix4x4.cpp ^
    TimeManager.cpp ^
    Graphics\Core\DirectXGraphicsAPI.cpp ^
    Graphics\Core\GraphicsAPIFactory.cpp ^
    -DPLATFORM_WINDOWS -DUSE_DIRECTX ^
    -L./ThirdParty/DirectX/lib -ld3d11 -ldxgi -ld3dcompiler

REM Check if DirectX compilation failed
if %ERRORLEVEL% NEQ 0 (
    echo Warning: DirectX compilation failed, falling back to OpenGL
    set USE_DIRECTX=false
    
    REM Try to compile with OpenGL instead
    echo Compiling with OpenGL fallback...
    g++ %CFLAGS% %INCLUDES% -o bin\windows\EmergencyEditor_red_cube.exe ^
        Editor\EmergencyEditor_red_cube.cpp ^
        Vector3.cpp ^
        Matrix4x4.cpp ^
        TimeManager.cpp ^
        Graphics\Core\OpenGLGraphicsAPI.cpp ^
        Graphics\Core\GraphicsAPIFactory.cpp ^
        -DPLATFORM_WINDOWS ^
        -lopengl32 -lglu32
    
    if %ERRORLEVEL% NEQ 0 (
        echo Error: OpenGL fallback compilation also failed
        exit /b 1
    ) else (
        echo Emergency red cube renderer built successfully with OpenGL fallback.
    )
) else (
    echo Emergency red cube renderer built successfully with DirectX support.
)

echo Build completed.
