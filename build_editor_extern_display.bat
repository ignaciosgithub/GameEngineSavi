@echo off
   
REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows
   
REM Build the external display editor
g++ -std=c++11 -o bin\windows\Editor_extern_display.exe ^
    Editor\Editor_extern_display_main.cpp ^
    Editor\Editor_extern_display.cpp ^
    FrameCapture_png.cpp ^
    Vector3.cpp ^
    Matrix4x4.cpp ^
    Camera.cpp ^
    Model.cpp ^
    MonoBehaviourLike.cpp ^
    TimeManager.cpp ^
    Raycast.cpp ^
    RigidBody.cpp ^
    CollisionSystem.cpp ^
    PhysicsSystem.cpp ^
    Scene.cpp ^
    PointLight.cpp ^
    CameraManager.cpp ^
    Shaders\Core\ShaderProgram.cpp ^
    Shaders\Core\Shader.cpp ^
    Shaders\Core\ShaderError.cpp ^
    Texture.cpp ^
    EngineCondition.cpp ^
    ProjectSettings\ProjectSettings.cpp ^
    ProjectSettings\ProjectManager.cpp ^
    Profiler.cpp ^
    Debugger.cpp ^
    -I. ^
    -IThirdParty\OpenGL\include ^
    -DGL_GLEXT_PROTOTYPES ^
    -DPLATFORM_WINDOWS ^
    -lopengl32 -lglu32 -lglut32 -luser32 -lgdi32
   
REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo External display editor build successful.
    
    REM Create frames directory if it doesn't exist
    if not exist frames mkdir frames
    
    REM Run the external display editor
    echo Running external display editor to generate frames...
    bin\windows\Editor_extern_display.exe
) else (
    echo External display editor build failed.
)
