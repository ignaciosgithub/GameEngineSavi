@echo off
REM Create build directory if it doesn't exist
if not exist build mkdir build

REM Compile the editor
echo Building editor...
g++ -o build\editor.exe ^
    Editor\EditorMain.cpp ^
    Scene.cpp ^
    GameObject.cpp ^
    Vector3.cpp ^
    Matrix4x4.cpp ^
    Camera.cpp ^
    CameraManager.cpp ^
    Model.cpp ^
    Texture.cpp ^
    PointLight.cpp ^
    Debugger.cpp ^
    FrameCapture.cpp ^
    FrameCapture_png.cpp ^
    Graphics\Core\OpenGLGraphicsAPI.cpp ^
    Graphics\Core\DirectXGraphicsAPI.cpp ^
    Graphics\Core\GraphicsAPIFactory.cpp ^
    Shaders\Core\ShaderProgram.cpp ^
    Shaders\Core\Shader.cpp ^
    Shaders\Core\ShaderError.cpp ^
    -std=c++11 ^
    -I. ^
    -IThirdParty\OpenGL\include ^
    -DGL_GLEXT_PROTOTYPES ^
    -lopengl32 -lglu32 -lfreeglut

REM Check if build was successful
if %ERRORLEVEL% EQU 0 (
    echo Editor build successful.
    echo Run the editor with: build\editor.exe
) else (
    echo Editor build failed.
)

REM Run the editor if build was successful
if %ERRORLEVEL% EQU 0 (
    build\editor.exe
)
