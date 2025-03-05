#ifndef OPENGL_GRAPHICS_API_H
#define OPENGL_GRAPHICS_API_H

#include "IGraphicsAPI.h"
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
    
    // Shader management
    void UseShaderProgram(ShaderProgram* program) override;
    
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
    
private:
    // Convert enum values to OpenGL constants
    GLenum ConvertBufferType(BufferType type) const;
    GLenum ConvertDrawMode(DrawMode mode) const;
    
    // OpenGL state tracking
    GLuint currentVAO;
    GLuint currentBuffer;
    GLuint currentShader;
};

#endif // OPENGL_GRAPHICS_API_H
