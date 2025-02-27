@echo off
echo Building shader compilation test...

REM Build shader compilation test
g++ -std=c++14 -I.. ^
    ShaderCompilationTest.cpp ^
    ..\Shaders\Core\Shader.cpp ^
    ..\Shaders\Core\ShaderProgram.cpp ^
    ..\Shaders\Core\ShaderError.cpp ^
    -I"%GLEW_HOME%\include" ^
    -L"%GLEW_HOME%\lib" ^
    -lopengl32 -lglew32 -lglfw3 -o shader_compilation_test.exe

if exist shader_compilation_test.exe (
    echo Build successful
    echo Run shader_compilation_test.exe to test shader compilation
) else (
    echo Build failed
    exit /b 1
)

pause
