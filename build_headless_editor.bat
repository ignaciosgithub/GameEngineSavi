@echo off
setlocal enabledelayedexpansion

REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Create frames directory if it doesn't exist
if not exist frames mkdir frames

REM Build the headless editor
g++ -std=c++11 -o bin\windows\HeadlessEditor.exe ^
    Editor\FrameSavingEditor.cpp ^
    Editor\Editor.cpp ^
    Editor\HierarchyPanel.cpp ^
    Editor\InspectorPanel.cpp ^
    Editor\ProjectPanel.cpp ^
    Editor\SceneViewPanel.cpp ^
    Editor\Vector3Field.cpp ^
    GameObject.cpp ^
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
    DirectionalLight.cpp ^
    CameraManager.cpp ^
    Shaders\Core\ShaderProgram.cpp ^
    Shaders\Core\Shader.cpp ^
    Shaders\Core\ShaderError.cpp ^
    Editor\TextField.cpp ^
    Texture.cpp ^
    EngineCondition.cpp ^
    FrameCapture_png.cpp ^
    ProjectSettings\ProjectSettings.cpp ^
    ProjectSettings\ProjectManager.cpp ^
    Profiler.cpp ^
    Debugger.cpp ^
    RedundancyDetector.cpp ^
    Graphics\Core\GraphicsAPIFactory.cpp ^
    Graphics\Core\OpenGLGraphicsAPI.cpp ^
    ThirdParty\stb\stb_image_write_impl.cpp ^
    -I. ^
    -IThirdParty\OpenGL\include ^
    -DGL_GLEXT_PROTOTYPES ^
    -DGLEW_STATIC ^
    -DHEADLESS_MODE ^
    -lopengl32 -lglu32 -lglut -lgdi32 -lglew32

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Headless editor build successful.
) else (
    echo Headless editor build failed.
)
