#ifndef IGRAPHICS_API_H
#define IGRAPHICS_API_H

#include <vector>
#include <string>
#include "../../Vector3.h"
#include "../../Vector2.h"
#include "../../Matrix4x4.h"

// Forward declarations
class ShaderProgram;
class Texture;

// Buffer types
enum class BufferType {
    VERTEX_BUFFER,
    INDEX_BUFFER,
    TEXTURE_COORD_BUFFER,
    NORMAL_BUFFER
};

// Draw modes
enum class DrawMode {
    TRIANGLES,
    LINES,
    POINTS
};

// Graphics API interface
class IGraphicsAPI {
public:
    virtual ~IGraphicsAPI() = default;
    
    // Initialization and cleanup
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;
    
    // Buffer management
    virtual unsigned int CreateVertexArray() = 0;
    virtual void BindVertexArray(unsigned int vao) = 0;
    virtual void DeleteVertexArray(unsigned int vao) = 0;
    
    virtual unsigned int CreateBuffer() = 0;
    virtual void BindBuffer(BufferType type, unsigned int buffer) = 0;
    virtual void DeleteBuffer(unsigned int buffer) = 0;
    virtual void BufferData(BufferType type, const void* data, size_t size, bool dynamic = false) = 0;
    
    // Attribute configuration
    virtual void EnableVertexAttrib(unsigned int index) = 0;
    virtual void DisableVertexAttrib(unsigned int index) = 0;
    virtual void VertexAttribPointer(unsigned int index, int size, bool normalized, size_t stride, const void* pointer) = 0;
    
    // Drawing
    virtual void DrawArrays(DrawMode mode, int first, int count) = 0;
    virtual void DrawElements(DrawMode mode, int count, const void* indices) = 0;
    
    // Viewport and clear
    virtual void SetViewport(int x, int y, int width, int height) = 0;
    virtual void Clear(bool colorBuffer, bool depthBuffer) = 0;
    virtual void SetClearColor(float r, float g, float b, float a) = 0;
    
    // Shader management
    virtual void UseShaderProgram(ShaderProgram* program) = 0;
    
    // Texture management
    virtual unsigned int CreateTexture() = 0;
    virtual void BindTexture(unsigned int texture, unsigned int unit) = 0;
    virtual void DeleteTexture(unsigned int texture) = 0;
    virtual void TexImage2D(int width, int height, const void* data, bool hasAlpha) = 0;
    
    // Debug utilities
    virtual void DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) = 0;
    virtual void DrawDebugAxes() = 0;
    
    // Get API name for debugging
    virtual const char* GetAPIName() const = 0;
};

#endif // IGRAPHICS_API_H
