#include "OpenGLGraphicsAPI.h"
#include <iostream>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#else
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <cstring>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/glew.h>
#endif

#ifndef PLATFORM_WINDOWS
// Function pointer type for glXCreateContextAttribsARB
typedef GLXContext (*PFNGLXCREATECONTEXTATTRIBSARBPROC)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
#endif

OpenGLGraphicsAPI::OpenGLGraphicsAPI()
    : currentVAO(0), currentBuffer(0), currentShader(0), currentTexture(0), windowOpen(false)
{
    std::cout << "OpenGLGraphicsAPI constructor called" << std::endl;
    
#ifdef PLATFORM_WINDOWS
    hWnd = NULL;
    hDC = NULL;
    hRC = NULL;
#else
    display = NULL;
    window = 0;
    context = NULL;
#endif
}

OpenGLGraphicsAPI::~OpenGLGraphicsAPI() {
    std::cout << "OpenGLGraphicsAPI destructor called" << std::endl;
    
    #ifndef PLATFORM_WINDOWS
    if (display) {
        std::cout << "Destructor - Cleaning up X display connection" << std::endl;
        if (context) {
            glXMakeCurrent(display, None, NULL);
            glXDestroyContext(display, context);
            context = NULL;
        }
        if (window) {
            XDestroyWindow(display, window);
            window = 0;
        }
        XCloseDisplay(display);
        display = NULL;
    }
    #endif
}

bool OpenGLGraphicsAPI::Initialize() {
    std::cout << "Initializing OpenGL graphics API" << std::endl;
    
    // Note: GLEW initialization should happen after OpenGL context creation
    // This will be done in CreateWindow method after context is created
    
    #ifndef PLATFORM_WINDOWS
    if (display) {
        std::cout << "Initialize - Cleaning up existing X display connection" << std::endl;
        XCloseDisplay(display);
        display = NULL;
    }
    window = 0;
    context = NULL;
    #endif
    
    windowOpen = false;
    return true;
}

void OpenGLGraphicsAPI::Shutdown() {
    std::cout << "Shutting down OpenGL graphics API" << std::endl;
}

unsigned int OpenGLGraphicsAPI::CreateVertexArray() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    return vao;
}

void OpenGLGraphicsAPI::BindVertexArray(unsigned int vao) {
    if (currentVAO != vao) {
        glBindVertexArray(vao);
        currentVAO = vao;
    }
}

void OpenGLGraphicsAPI::DeleteVertexArray(unsigned int vao) {
    if (currentVAO == vao) {
        currentVAO = 0;
    }
    glDeleteVertexArrays(1, &vao);
}

unsigned int OpenGLGraphicsAPI::CreateBuffer() {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return buffer;
}

void OpenGLGraphicsAPI::BindBuffer(BufferType type, unsigned int buffer) {
    glBindBuffer(ConvertBufferType(type), buffer);
    currentBuffer = buffer;
}

void OpenGLGraphicsAPI::DeleteBuffer(unsigned int buffer) {
    if (currentBuffer == buffer) {
        currentBuffer = 0;
    }
    glDeleteBuffers(1, &buffer);
}

void OpenGLGraphicsAPI::BufferData(BufferType type, const void* data, size_t size, bool dynamic) {
    glBufferData(ConvertBufferType(type), size, data, ConvertBufferUsage(dynamic));
}

void OpenGLGraphicsAPI::EnableVertexAttrib(unsigned int index) {
    glEnableVertexAttribArray(index);
}

void OpenGLGraphicsAPI::DisableVertexAttrib(unsigned int index) {
    glDisableVertexAttribArray(index);
}

void OpenGLGraphicsAPI::VertexAttribPointer(unsigned int index, int size, bool normalized, 
                                          size_t stride, const void* pointer) {
    glVertexAttribPointer(index, size, GL_FLOAT, normalized ? GL_TRUE : GL_FALSE, stride, pointer);
}

unsigned int OpenGLGraphicsAPI::CreateShader(int shaderType) {
    GLenum glShaderType = GL_VERTEX_SHADER;
    switch (shaderType) {
        case 0: // Vertex shader
            glShaderType = GL_VERTEX_SHADER;
            break;
        case 1: // Fragment shader
            glShaderType = GL_FRAGMENT_SHADER;
            break;
        case 2: // Geometry shader
            glShaderType = GL_GEOMETRY_SHADER;
            break;
    }
    return glCreateShader(glShaderType);
}

void OpenGLGraphicsAPI::ShaderSource(unsigned int shader, const std::string& source) {
    const char* sourcePtr = source.c_str();
    glShaderSource(shader, 1, &sourcePtr, nullptr);
}

void OpenGLGraphicsAPI::CompileShader(unsigned int shader) {
    glCompileShader(shader);
}

void OpenGLGraphicsAPI::DeleteShader(unsigned int shader) {
    glDeleteShader(shader);
}

bool OpenGLGraphicsAPI::GetShaderCompileStatus(unsigned int shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return success == GL_TRUE;
}

std::string OpenGLGraphicsAPI::GetShaderInfoLog(unsigned int shader) {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        std::string log(length, ' ');
        glGetShaderInfoLog(shader, length, &length, &log[0]);
        return log;
    }
    return "";
}

unsigned int OpenGLGraphicsAPI::CreateProgram() {
    return glCreateProgram();
}

void OpenGLGraphicsAPI::AttachShader(unsigned int program, unsigned int shader) {
    glAttachShader(program, shader);
}

void OpenGLGraphicsAPI::LinkProgram(unsigned int program) {
    glLinkProgram(program);
}

void OpenGLGraphicsAPI::UseShaderProgram(ShaderProgram* program) {
    std::cout << "OpenGLGraphicsAPI::UseShaderProgram - " 
              << (program ? "Using program with handle: " + std::to_string(program->GetHandle()) 
                          : "Using null program (unbinding)") << std::endl;
    
    if (program) {
        unsigned int handle = program->GetHandle();
        if (handle == 0) {
            std::cout << "OpenGLGraphicsAPI::UseShaderProgram - WARNING: Program handle is 0" << std::endl;
        }
        glUseProgram(handle);
        currentShader = handle;
    } else {
        std::cout << "OpenGLGraphicsAPI::UseShaderProgram - Unbinding shader program (using 0)" << std::endl;
        glUseProgram(0);
        currentShader = 0;
    }
}

void OpenGLGraphicsAPI::DeleteProgram(unsigned int program) {
    if (currentShader == program) {
        currentShader = 0;
    }
    glDeleteProgram(program);
}

bool OpenGLGraphicsAPI::GetProgramLinkStatus(unsigned int program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    return success == GL_TRUE;
}

std::string OpenGLGraphicsAPI::GetProgramInfoLog(unsigned int program) {
    GLint length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length > 0) {
        std::string log(length, ' ');
        glGetProgramInfoLog(program, length, &length, &log[0]);
        return log;
    }
    return "";
}

void OpenGLGraphicsAPI::SetUniform1i(unsigned int program, const std::string& name, int value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void OpenGLGraphicsAPI::SetUniform1f(unsigned int program, const std::string& name, float value) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void OpenGLGraphicsAPI::SetUniform3f(unsigned int program, const std::string& name, float x, float y, float z) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform3f(location, x, y, z);
    }
}

void OpenGLGraphicsAPI::SetUniform4f(unsigned int program, const std::string& name, float x, float y, float z, float w) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform4f(location, x, y, z, w);
    }
}

void OpenGLGraphicsAPI::SetUniformMatrix4fv(unsigned int program, const std::string& name, const float* value, bool transpose) {
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE, value);
    }
}

// Get uniform location
int OpenGLGraphicsAPI::GetUniformLocation(unsigned int program, const std::string& name) {
#ifndef PLATFORM_WINDOWS
    return glGetUniformLocation(program, name.c_str());
#else
    return -1;
#endif
}

// Set uniform float array
void OpenGLGraphicsAPI::SetUniformFloatArray(unsigned int program, const std::string& name, const float* values, int count) {
#ifndef PLATFORM_WINDOWS
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform1fv(location, count, values);
    }
#endif
}

// Set uniform int array
void OpenGLGraphicsAPI::SetUniformIntArray(unsigned int program, const std::string& name, const int* values, int count) {
#ifndef PLATFORM_WINDOWS
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform1iv(location, count, values);
    }
#endif
}

// Set uniform vec3 array
void OpenGLGraphicsAPI::SetUniformVec3Array(unsigned int program, const std::string& name, const float* values, int count) {
#ifndef PLATFORM_WINDOWS
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniform3fv(location, count, values);
    }
#endif
}

// Set uniform matrix4 array
void OpenGLGraphicsAPI::SetUniformMatrix4Array(unsigned int program, const std::string& name, const float* values, int count, bool transpose) {
#ifndef PLATFORM_WINDOWS
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location != -1) {
        glUniformMatrix4fv(location, count, transpose ? GL_TRUE : GL_FALSE, values);
    }
#endif
}

void OpenGLGraphicsAPI::Clear(bool colorBuffer, bool depthBuffer) {
    GLbitfield clearMask = 0;
    if (colorBuffer) clearMask |= GL_COLOR_BUFFER_BIT;
    if (depthBuffer) clearMask |= GL_DEPTH_BUFFER_BIT;
    glClear(clearMask);
}

void OpenGLGraphicsAPI::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void OpenGLGraphicsAPI::DrawArrays(DrawMode mode, int first, int count) {
    std::cout << "OpenGLGraphicsAPI::DrawArrays - Drawing " << count << " vertices with mode: " 
              << static_cast<int>(mode) << " starting at: " << first << std::endl;
    glDrawArrays(ConvertDrawMode(mode), first, count);
}

void OpenGLGraphicsAPI::DrawElements(DrawMode mode, int count, const void* indices) {
    std::cout << "OpenGLGraphicsAPI::DrawElements - Drawing " << count << " indices with mode: " 
              << static_cast<int>(mode) << " indices ptr: " << indices << std::endl;
    glDrawElements(ConvertDrawMode(mode), count, GL_UNSIGNED_INT, indices);
}

unsigned int OpenGLGraphicsAPI::CreateTexture() {
    GLuint texture;
    glGenTextures(1, &texture);
    return texture;
}

void OpenGLGraphicsAPI::BindTexture(unsigned int texture, unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    currentTexture = texture;
}

void OpenGLGraphicsAPI::DeleteTexture(unsigned int texture) {
    if (currentTexture == texture) {
        currentTexture = 0;
    }
    glDeleteTextures(1, &texture);
}

void OpenGLGraphicsAPI::TexImage2D(int width, int height, const void* data, bool hasAlpha) {
    GLenum format = hasAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    
    // Set default texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void OpenGLGraphicsAPI::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void OpenGLGraphicsAPI::Begin2D() {
    // Save current OpenGL state
    glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    // Disable depth testing so GUI elements aren't occluded
    glDisable(GL_DEPTH_TEST);
    
    // Set up orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
    
    // Set modelview matrix to identity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLGraphicsAPI::End2D() {
    // Restore modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
    // Restore projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    // Restore previous OpenGL state
    glPopAttrib();
}

GLenum OpenGLGraphicsAPI::ConvertBufferType(BufferType type) {
    switch (type) {
        case BufferType::VERTEX_BUFFER:
            return GL_ARRAY_BUFFER;
        case BufferType::INDEX_BUFFER:
            return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::TEXTURE_COORD_BUFFER:
            return GL_ARRAY_BUFFER;
        case BufferType::NORMAL_BUFFER:
            return GL_ARRAY_BUFFER;
        default:
            return GL_ARRAY_BUFFER;
    }
}

GLenum OpenGLGraphicsAPI::ConvertDrawMode(DrawMode mode) {
    switch (mode) {
        case DrawMode::POINTS:
            return GL_POINTS;
        case DrawMode::LINES:
            return GL_LINES;
        case DrawMode::TRIANGLES:
            return GL_TRIANGLES;
        default:
            return GL_TRIANGLES;
    }
}

GLenum OpenGLGraphicsAPI::ConvertDataType(int type) {
    switch (type) {
        case 0: // BYTE
            return GL_BYTE;
        case 1: // UNSIGNED_BYTE
            return GL_UNSIGNED_BYTE;
        case 2: // SHORT
            return GL_SHORT;
        case 3: // UNSIGNED_SHORT
            return GL_UNSIGNED_SHORT;
        case 4: // INT
            return GL_INT;
        case 5: // UNSIGNED_INT
            return GL_UNSIGNED_INT;
        case 6: // FLOAT
            return GL_FLOAT;
        case 7: // DOUBLE
            return GL_DOUBLE;
        default:
            return GL_FLOAT;
    }
}

GLenum OpenGLGraphicsAPI::ConvertBufferUsage(bool dynamic) {
    return dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
}

GLenum OpenGLGraphicsAPI::ConvertTextureType(unsigned int unit) {
    return GL_TEXTURE_2D; // We only support 2D textures for now
}

GLenum OpenGLGraphicsAPI::ConvertTextureParameter(int param) {
    switch (param) {
        case 0: // MIN_FILTER
            return GL_TEXTURE_MIN_FILTER;
        case 1: // MAG_FILTER
            return GL_TEXTURE_MAG_FILTER;
        case 2: // WRAP_S
            return GL_TEXTURE_WRAP_S;
        case 3: // WRAP_T
            return GL_TEXTURE_WRAP_T;
        default:
            return GL_TEXTURE_MIN_FILTER;
    }
}

GLenum OpenGLGraphicsAPI::ConvertPixelFormat(bool hasAlpha) {
    return hasAlpha ? GL_RGBA : GL_RGB;
}

// Matrix operations
void OpenGLGraphicsAPI::SetProjectionMatrix(const Matrix4x4& matrix) {
#ifndef PLATFORM_WINDOWS
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixf(&matrix.elements[0][0]);
#endif
}

void OpenGLGraphicsAPI::SetViewMatrix(const Matrix4x4& matrix) {
#ifndef PLATFORM_WINDOWS
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixf(&matrix.elements[0][0]);
#endif
}

void OpenGLGraphicsAPI::SetModelMatrix(const Matrix4x4& matrix) {
#ifndef PLATFORM_WINDOWS
    // Save current matrix mode
    GLint currentMode;
    glGetIntegerv(GL_MATRIX_MODE, &currentMode);
    
    // Set model matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(&matrix.elements[0][0]);
    
    // Restore previous matrix mode
    glMatrixMode(currentMode);
#endif
}

// Debug utilities
void OpenGLGraphicsAPI::DrawDebugLine(const Vector3& start, const Vector3& end, const Vector3& color) {
#ifndef PLATFORM_WINDOWS
    // Save current state
    GLboolean lighting;
    glGetBooleanv(GL_LIGHTING, &lighting);
    
    // Disable lighting for debug lines
    glDisable(GL_LIGHTING);
    
    // Set line color
    glColor3f(color.x, color.y, color.z);
    
    // Draw line
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
    
    // Restore lighting state
    if (lighting) {
        glEnable(GL_LIGHTING);
    }
#endif
}

// Framebuffer operations
void OpenGLGraphicsAPI::ReadPixels(int x, int y, int width, int height, unsigned char* pixels) {
    glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}

void OpenGLGraphicsAPI::GetViewport(int* viewport) {
    glGetIntegerv(GL_VIEWPORT, viewport);
}

void OpenGLGraphicsAPI::DrawDebugAxes() {
#ifndef PLATFORM_WINDOWS
    // X axis - Red
    DrawDebugLine(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(1, 0, 0));
    
    // Y axis - Green
    DrawDebugLine(Vector3(0, 0, 0), Vector3(0, 1, 0), Vector3(0, 1, 0));
    
    // Z axis - Blue
    DrawDebugLine(Vector3(0, 0, 0), Vector3(0, 0, 1), Vector3(0, 0, 1));
#endif
}

// Depth and culling
void OpenGLGraphicsAPI::SetDepthTest(bool enable) {
#ifndef PLATFORM_WINDOWS
    if (enable) {
        glEnable(GL_DEPTH_TEST);
    } else {
        glDisable(GL_DEPTH_TEST);
    }
#endif
}

void OpenGLGraphicsAPI::SetDepthFunc(int func) {
#ifndef PLATFORM_WINDOWS
    GLenum glFunc = GL_LESS;
    switch (func) {
        case 0: glFunc = GL_NEVER; break;
        case 1: glFunc = GL_LESS; break;
        case 2: glFunc = GL_EQUAL; break;
        case 3: glFunc = GL_LEQUAL; break;
        case 4: glFunc = GL_GREATER; break;
        case 5: glFunc = GL_NOTEQUAL; break;
        case 6: glFunc = GL_GEQUAL; break;
        case 7: glFunc = GL_ALWAYS; break;
    }
    glDepthFunc(glFunc);
#endif
}

void OpenGLGraphicsAPI::SetCullFace(bool enable) {
#ifndef PLATFORM_WINDOWS
    if (enable) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
#endif
}

void OpenGLGraphicsAPI::SetCullFaceMode(int mode) {
#ifndef PLATFORM_WINDOWS
    GLenum glMode = GL_BACK;
    switch (mode) {
        case 0: glMode = GL_FRONT; break;
        case 1: glMode = GL_BACK; break;
        case 2: glMode = GL_FRONT_AND_BACK; break;
    }
    glCullFace(glMode);
#endif
}

// Window management
bool OpenGLGraphicsAPI::CreateWindow(int width, int height, const char* title) {
#ifdef PLATFORM_WINDOWS
    // Windows-specific window creation code
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OpenGLWindowClass";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassEx(&wc)) {
        std::cout << "Failed to register window class" << std::endl;
        return false;
    }
    
    hWnd = CreateWindow(
        "OpenGLWindowClass", title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        NULL, NULL,
        GetModuleHandle(NULL), NULL
    );
    
    if (!hWnd) {
        std::cout << "Failed to create window" << std::endl;
        return false;
    }
    
    hDC = GetDC(hWnd);
    
    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    
    int format = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, format, &pfd);
    
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);
    
    windowOpen = true;
    return true;
#else
    // Linux-specific window creation code
    const char* displayName = getenv("DISPLAY");
    if (!displayName || strlen(displayName) == 0) {
        displayName = ":0"; // Default to :0 if DISPLAY is not set
    }
    
    std::cout << "Attempting to connect to X display: " << displayName << std::endl;
    display = XOpenDisplay(displayName);
    if (!display) {
        std::cout << "Failed to open X display: " << displayName << std::endl;
        return false;
    }
    
    Window root = DefaultRootWindow(display);
    
    // First get a framebuffer config using glXChooseFBConfig
    int fbAttribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, True,
        GLX_RED_SIZE, 8,
        GLX_GREEN_SIZE, 8,
        GLX_BLUE_SIZE, 8,
        GLX_ALPHA_SIZE, 8,
        GLX_DEPTH_SIZE, 24,
        None
    };
    
    int numFBConfigs = 0;
    GLXFBConfig* fbConfigs = glXChooseFBConfig(display, DefaultScreen(display), fbAttribs, &numFBConfigs);
    
    if (!fbConfigs || numFBConfigs == 0) {
        std::cout << "Failed to get framebuffer configs" << std::endl;
        return false;
    }
    
    // Get the visual from the framebuffer config
    XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbConfigs[0]);
    if (!vi) {
        std::cout << "No appropriate visual found" << std::endl;
        XFree(fbConfigs);
        return false;
    }
    
    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);
    
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask; // Added StructureNotifyMask for window events
    
    window = XCreateWindow(
        display, root,
        0, 0, width, height, 0,
        vi->depth, InputOutput,
        vi->visual, CWColormap | CWEventMask, &swa
    );
    
    // Set WM_DELETE_WINDOW atom to handle window close events properly
    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, window, &wmDeleteWindow, 1);
    
    XMapWindow(display, window);
    XStoreName(display, window, title);
    
    // Set up attributes for modern OpenGL context (3.3 core profile)
    int contextAttribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };
    
    // Get the function pointer for glXCreateContextAttribsARB
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 
        (PFNGLXCREATECONTEXTATTRIBSARBPROC)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    
    if (glXCreateContextAttribsARB) {
        // Try to create a modern OpenGL context
        context = glXCreateContextAttribsARB(display, fbConfigs[0], NULL, True, contextAttribs);
        
        if (!context) {
            std::cout << "Failed to create modern OpenGL context, falling back to legacy context" << std::endl;
            context = glXCreateContext(display, vi, NULL, GL_TRUE);
        } else {
            std::cout << "Created modern OpenGL context (3.3 core profile)" << std::endl;
        }
    } else {
        // Fall back to legacy context creation if the extension is not available
        std::cout << "glXCreateContextAttribsARB not available, using legacy context" << std::endl;
        context = glXCreateContext(display, vi, NULL, GL_TRUE);
    }
    
    if (!context) {
        std::cout << "Failed to create OpenGL context" << std::endl;
        XFree(fbConfigs);
        XFree(vi);
        return false;
    }
    
    glXMakeCurrent(display, window, context);
    
    // Clean up
    XFree(fbConfigs);
    
    // Initialize GLEW after context is created
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "GLEW initialization failed: " << glewGetErrorString(err) << std::endl;
        std::cout << "Failed to initialize OpenGL" << std::endl;
        return false;
    }
    
    // Check OpenGL version
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    const char* rendererStr = (const char*)glGetString(GL_RENDERER);
    const char* vendorStr = (const char*)glGetString(GL_VENDOR);
    
    std::cout << "OpenGL version: " << (versionStr ? versionStr : "Unknown") << std::endl;
    std::cout << "OpenGL renderer: " << (rendererStr ? rendererStr : "Unknown") << std::endl;
    std::cout << "OpenGL vendor: " << (vendorStr ? vendorStr : "Unknown") << std::endl;
    std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;
    
    // Verify we have at least OpenGL 3.3
    if (versionStr) {
        int major = 0, minor = 0;
        sscanf(versionStr, "%d.%d", &major, &minor);
        
        if (major < 3 || (major == 3 && minor < 3)) {
            std::cout << "Warning: OpenGL 3.3 or higher is required for all shaders to work correctly." << std::endl;
            std::cout << "Current version: " << major << "." << minor << std::endl;
        } else {
            std::cout << "OpenGL version " << major << "." << minor << " is compatible with shader requirements." << std::endl;
        }
    }
    
    windowOpen = true;
    return true;
#endif
}

void OpenGLGraphicsAPI::DestroyWindow() {
#ifdef PLATFORM_WINDOWS
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
    DestroyWindow(hWnd);
#else
    if (display) {
        std::cout << "DestroyWindow - Cleaning up X11 resources" << std::endl;
        if (context) {
            glXMakeCurrent(display, None, NULL);
            glXDestroyContext(display, context);
            context = NULL;
        }
        if (window) {
            XDestroyWindow(display, window);
            window = 0;
        }
        XCloseDisplay(display);
        display = NULL;
    } else {
        std::cout << "DestroyWindow - Display already null, nothing to clean up" << std::endl;
    }
#endif
    windowOpen = false;
}

void OpenGLGraphicsAPI::MakeContextCurrent() {
#ifdef PLATFORM_WINDOWS
    wglMakeCurrent(hDC, hRC);
#else
    if (display && window && context) {
        glXMakeCurrent(display, window, context);
    } else {
        std::cout << "MakeContextCurrent - ERROR: Cannot make context current, resources not initialized" 
                  << " (display=" << (display ? "valid" : "null")
                  << ", window=" << window
                  << ", context=" << (context ? "valid" : "null") << ")" << std::endl;
    }
#endif
}

bool OpenGLGraphicsAPI::IsWindowOpen() {
    return windowOpen;
}

void OpenGLGraphicsAPI::PollEvents() {
#ifdef PLATFORM_WINDOWS
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_QUIT) {
            windowOpen = false;
        }
    }
#else
    std::cout << "OpenGLGraphicsAPI::PollEvents - Starting X11 event polling" << std::endl;
    
    if (!display) {
        std::cout << "OpenGLGraphicsAPI::PollEvents - ERROR: Display is null, cannot poll events" << std::endl;
        windowOpen = false;
        return;
    }
    
    if (XConnectionNumber(display) < 0) {
        std::cout << "OpenGLGraphicsAPI::PollEvents - ERROR: X11 display connection is invalid" << std::endl;
        windowOpen = false;
        return;
    }
    
    int pending = 0;
    try {
        pending = XPending(display);
        std::cout << "OpenGLGraphicsAPI::PollEvents - " << pending << " pending X events" << std::endl;
    } catch (...) {
        std::cout << "OpenGLGraphicsAPI::PollEvents - ERROR: Exception while checking for pending events" << std::endl;
        windowOpen = false;
        return;
    }
    
    while (pending > 0) {
        XEvent event;
        XNextEvent(display, &event);
        
        // Handle different event types
        std::cout << "OpenGLGraphicsAPI::PollEvents - Processing event type: " << event.type << std::endl;
        switch (event.type) {
            case KeyPress:
                {
                    KeySym key = XLookupKeysym(&event.xkey, 0);
                    if (key == XK_Escape) {
                        windowOpen = false;
                    }
                }
                break;
                
            case ClientMessage:
                {
                    // Check if it's a window close event (WM_DELETE_WINDOW)
                    Atom wmDeleteWindow = XInternAtom(display, "WM_DELETE_WINDOW", False);
                    if ((Atom)event.xclient.data.l[0] == wmDeleteWindow) {
                        windowOpen = false;
                    }
                }
                break;
                
            case DestroyNotify:
                // Window was destroyed
                windowOpen = false;
                break;
        }
    }
#endif
}

// Platform-specific operations
void OpenGLGraphicsAPI::SwapBuffers() {
#ifdef PLATFORM_WINDOWS
    ::SwapBuffers(hDC);
#else
    if (display && window) {
        glXSwapBuffers(display, window);
    } else {
        std::cout << "SwapBuffers - ERROR: Cannot swap buffers, resources not initialized" 
                  << " (display=" << (display ? "valid" : "null")
                  << ", window=" << window << ")" << std::endl;
    }
#endif
}

// Default shader for models with missing shaders/textures
void OpenGLGraphicsAPI::UseDefaultRedShader() {
    // Simple shader program for red color
    static GLuint defaultRedShaderProgram = 0;
    
    // Create the shader program if it doesn't exist
    if (defaultRedShaderProgram == 0) {
        // Vertex shader source
        const char* vertexShaderSource = 
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "uniform mat4 model;\n"
            "uniform mat4 view;\n"
            "uniform mat4 projection;\n"
            "void main() {\n"
            "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
            "}\n";
        
        // Fragment shader source
        const char* fragmentShaderSource = 
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "void main() {\n"
            "    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color\n"
            "}\n";
        
        // Create and compile vertex shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        // Create and compile fragment shader
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        
        // Create shader program
        defaultRedShaderProgram = glCreateProgram();
        glAttachShader(defaultRedShaderProgram, vertexShader);
        glAttachShader(defaultRedShaderProgram, fragmentShader);
        glLinkProgram(defaultRedShaderProgram);
        
        // Delete shaders as they're linked into the program now
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    
    // Use the default red shader program
    glUseProgram(defaultRedShaderProgram);
}

// Note: Uniform setters are already defined above

void OpenGLGraphicsAPI::GetMousePosition(int& x, int& y) {
#ifdef PLATFORM_WINDOWS
    // Windows implementation would go here
    x = 0;
    y = 0;
#else
    if (display && window) {
        Window root_return, child_return;
        int root_x_return, root_y_return;
        unsigned int mask_return;
        
        XQueryPointer(display, window, &root_return, &child_return,
                      &root_x_return, &root_y_return, &x, &y, &mask_return);
    } else {
        x = 0;
        y = 0;
    }
#endif
}

bool OpenGLGraphicsAPI::IsMouseButtonPressed(int button) {
#ifdef PLATFORM_WINDOWS
    // Windows implementation would go here
    return false;
#else
    if (display && window) {
        Window root_return, child_return;
        int root_x_return, root_y_return, win_x_return, win_y_return;
        unsigned int mask_return;
        
        XQueryPointer(display, window, &root_return, &child_return,
                      &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return);
        
        unsigned int buttonMask = 0;
        switch (button) {
            case 0: buttonMask = Button1Mask; break; // Left button
            case 1: buttonMask = Button2Mask; break; // Middle button
            case 2: buttonMask = Button3Mask; break; // Right button
            default: return false;
        }
        
        return (mask_return & buttonMask) != 0;
    } else {
        return false;
    }
#endif
}
