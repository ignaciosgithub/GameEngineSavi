@echo off
REM Create bin directory if it doesn't exist
if not exist bin\windows mkdir bin\windows

REM Create frames directory if it doesn't exist
if not exist frames mkdir frames

REM Build the enhanced emergency editor
cl.exe /EHsc /std:c++14 /Fe:bin\windows\EnhancedEmergencyEditor.exe ^
    Editor\EnhancedEmergencyEditor.cpp ^
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
    Graphics\Core\DirectXGraphicsAPI.cpp ^
    /I. ^
    /IThirdParty\DirectX\include ^
    /DPLATFORM_WINDOWS ^
    /link /LIBPATH:ThirdParty\DirectX\lib d3d11.lib dxgi.lib d3dcompiler.lib

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Enhanced emergency editor build successful.
    
    REM Run the enhanced emergency editor to generate frames
    echo Running enhanced emergency editor to generate frames...
    bin\windows\EnhancedEmergencyEditor.exe
    
    REM Generate enhanced frames with proper shading and point light
    echo Generating enhanced frames with proper shading and point light...
    python create_enhanced_frames_pygame.py
    
    REM Check if frames were generated
    if exist frames\frame0.png (
        echo Enhanced frames generated successfully.
        echo You can view the frames using the frame viewer:
        echo   - Open frame_viewer.html in a web browser
        echo   - Run 'python frame_reader.py' for pygame-based viewing
    ) else (
        echo No frames were generated. Check the enhanced emergency editor output for errors.
    )
) else (
    echo Enhanced emergency editor build failed.
)
