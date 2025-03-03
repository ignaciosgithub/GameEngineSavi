@echo off

REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Build the editor
g++ -std=c++11 -o bin\windows\Editor.exe ^
    Editor\EditorMain.cpp ^
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
    Time.cpp ^
    EngineTime.cpp ^
    Raycast.cpp ^
    RigidBody.cpp ^
    CollisionSystem.cpp ^
    PhysicsSystem.cpp ^
    Scene_fixed_v3.cpp ^
    PointLight.cpp ^
    CameraManager.cpp ^
    Shaders\Core\ShaderProgram.cpp ^
    Shaders\Core\Shader.cpp ^
    Shaders\Core\ShaderError.cpp ^
    Editor\TextField.cpp ^
    Texture.cpp ^
    EngineCondition.cpp ^
    FrameCapture.cpp ^
    ProjectSettings\ProjectSettings.cpp ^
    ProjectSettings\ProjectManager.cpp ^
    Profiler.cpp ^
    -I. ^
    -IThirdParty\OpenGL\include ^
    -DGL_GLEXT_PROTOTYPES ^
    -lopengl32 -lglu32 -lglut32

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Editor build successful.
) else (
    echo Editor build failed.
)
