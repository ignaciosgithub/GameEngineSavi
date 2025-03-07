@echo off

REM Build shader test
g++ -std=c++14 -I.. ^
    ShaderTest.cpp ^
    ..\Model.cpp ^
    ..\Scene.cpp ^
    ..\Camera.cpp ^
    ..\GameObject.cpp ^
    ..\Vector3.cpp ^
    ..\Matrix4x4.cpp ^
    ..\Shaders\Core\Shader.cpp ^
    ..\Shaders\Core\ShaderProgram.cpp ^
    ..\Shaders\Core\ShaderError.cpp ^
    ..\Shaders\Assets\ShaderAsset.cpp ^
    -I"%GLEW_HOME%\include" ^
    -L"%GLEW_HOME%\lib" ^
    -lopengl32 -lglew32 -lglfw3 -o shader_test.exe

REM Make executable
if exist shader_test.exe (
    echo Build successful
) else (
    echo Build failed
    exit /b 1
)
