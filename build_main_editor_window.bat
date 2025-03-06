@echo off
REM Build script for the main editor window

echo Building main editor window...

REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Set compiler options
set CFLAGS=-std=c++11 -Wall -Wextra -g
set INCLUDES=-I. -IThirdParty/OpenGL/include -IThirdParty/DirectX/include

REM Set DirectX flag to true by default
set USE_DIRECTX=true

REM First, try to compile with DirectX
echo Attempting to compile with DirectX support...
g++ %CFLAGS% %INCLUDES% -o bin\windows\main_editor_window.exe ^
    main_editor_window.cpp ^
    Vector3.cpp ^
    Matrix4x4.cpp ^
    TimeManager.cpp ^
    Graphics\Core\DirectXGraphicsAPI.cpp ^
    Graphics\Core\GraphicsAPIFactory.cpp ^
    Editor\Editor.cpp ^
    Editor\HierarchyPanel.cpp ^
    Editor\InspectorPanel.cpp ^
    Editor\ProjectPanel.cpp ^
    Editor\SceneViewPanel.cpp ^
    Scene.cpp ^
    GameObject.cpp ^
    Camera.cpp ^
    PhysicsSystem.cpp ^
    CameraManager.cpp ^
    Model.cpp ^
    EngineCondition.cpp ^
    Shaders\Core\ShaderProgram.cpp ^
    -DPLATFORM_WINDOWS -DUSE_DIRECTX ^
    -L./ThirdParty/DirectX/lib -ld3d11 -ldxgi -ld3dcompiler

REM Check if DirectX compilation failed
if %ERRORLEVEL% NEQ 0 (
    echo Warning: DirectX compilation failed, falling back to OpenGL
    set USE_DIRECTX=false
    
    REM Try to compile with OpenGL instead
    echo Compiling with OpenGL fallback...
    g++ %CFLAGS% %INCLUDES% -o bin\windows\main_editor_window.exe ^
        main_editor_window.cpp ^
        Vector3.cpp ^
        Matrix4x4.cpp ^
        TimeManager.cpp ^
        Graphics\Core\OpenGLGraphicsAPI.cpp ^
        Graphics\Core\GraphicsAPIFactory.cpp ^
        Editor\Editor.cpp ^
        Editor\HierarchyPanel.cpp ^
        Editor\InspectorPanel.cpp ^
        Editor\ProjectPanel.cpp ^
        Editor\SceneViewPanel.cpp ^
        Scene.cpp ^
        GameObject.cpp ^
        Camera.cpp ^
        PhysicsSystem.cpp ^
        CameraManager.cpp ^
        Model.cpp ^
        EngineCondition.cpp ^
        Shaders\Core\ShaderProgram.cpp ^
        -DPLATFORM_WINDOWS ^
        -lopengl32 -lglu32 -lglew32
    
    if %ERRORLEVEL% NEQ 0 (
        echo Error: OpenGL fallback compilation also failed
        exit /b 1
    ) else (
        echo Main editor window built successfully with OpenGL fallback.
    )
) else (
    echo Main editor window built successfully with DirectX support.
)

echo Build completed.

REM Run the main editor window
echo Running main editor window...
bin\windows\main_editor_window.exe
