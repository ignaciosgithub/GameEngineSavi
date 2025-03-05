#ifndef OPENGL_GRAPHICS_API_H
#define OPENGL_GRAPHICS_API_H

#include "IGraphicsAPI.h"
#include "../../platform.h"

#ifndef PLATFORM_WINDOWS
#include "../../ThirdParty/OpenGL/include/GL/gl_definitions.h"

class OpenGLGraphicsAPI : public IGraphicsAPI {
public:
    OpenGLGraphicsAPI();
    ~OpenGLGraphicsAPI() override;
    
    // Initialization and cleanup
    bool Initialize() override;
    void Shutdown() override;
    
    // Buffer management
    unsigned int CreateVertexArray() override;
    void BindVertexArray(unsigned int vao) override;
    void DeleteVertexArray(unsigned int vao) override;
    
    unsigned int CreateBuffer() override;
    void BindBuffer(BufferType type, unsigned int buffer) override;
    void DeleteBuffer(unsigned int buffer) override;
    void BufferData(BufferType type, const void* data, size_t size, bool dynamic = false) override;
    
    // Attribute configuration
    void EnableVertexAttrib(unsigned int index) override;
    void DisableVertexAttrib(unsigned int index) override;
    void VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) override;
    
    // Drawing
    void DrawArrays(DrawMode mode, int first, int count) override;
    void DrawElements(DrawMode mode, int count, const void* indices) override;
    
    // Viewport and clear
    void SetViewport(int x, int y, int width, int height) override;
    void Clear(bool colorBuffer, bool depthBuffer) override;
    void SetClearColor(float r, float g, float b, float a) override;
    
    // Depth and culling
    void SetDepthTest(bool enable) override;
    void SetDepthFunc(int func) override;
    void SetCullFace(bool enable) override;
    void SetCullFaceMode(int mode) override;
    
    // Shader management
    void UseShaderProgram(ShaderProgram* program) override;
    unsigned int CreateShader(int shaderType) override;
    void DeleteShader(unsigned int shader) override;
    void ShaderSource(unsigned int shader, const std::string& source) override;
    void CompileShader(unsigned int shader) override;
    bool GetShaderCompileStatus(unsigned int shader) override;
    std::string GetShaderInfoLog(unsigned int shader) override;
    bool GetProgramLinkStatus(unsigned int program) override;
    std::string GetProgramInfoLog(unsigned int program) override;
    void AttachShader(unsigned int program, unsigned int shader) override;
    void LinkProgram(unsigned int program) override;
    unsigned int CreateProgram() override;
    void DeleteProgram(unsigned int program) override;
    
    // Uniform setters
    void SetUniform1f(unsigned int program, const std::string& name, float value) override;
    void SetUniform1i(unsigned int program, const std::string& name, int value) override;
    void SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) override;
    void SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) override;
    void SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose = false) override;
    
    // Texture management
    unsigned int CreateTexture() override;
    void BindTexture(unsigned int texture, unsigned int unit) override;
    void DeleteTexture(unsigned int texture) override;
    void TexImage2D(int width, int height, const void* data, bool hasAlpha) override;
    
    // Debug utilities
    void DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) override;
    void DrawDebugAxes() override;
    
    // Get API name for debugging
    const char* GetAPIName() const override { return "OpenGL"; }
    
    // Platform-specific operations
    void SwapBuffers() override;
    
private:
    // Convert enum values to OpenGL constants
    GLenum ConvertBufferType(BufferType type) const;
    GLenum ConvertDrawMode(DrawMode mode) const;
    
    // OpenGL state tracking
    GLuint currentVAO;
    GLuint currentBuffer;
    GLuint currentShader;
};

#endif // !PLATFORM_WINDOWS

#endif // OPENGL_GRAPHICS_API_H
