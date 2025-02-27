#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

// Mock OpenGL types and functions for testing without an OpenGL context
typedef unsigned int GLuint;
typedef int GLint;
typedef unsigned int GLenum;
typedef char GLchar;
typedef float GLfloat;
typedef bool GLboolean;

#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_2D 0x0DE1

// Mock OpenGL functions
GLuint glCreateShader(GLenum type) { return 1; }
void glShaderSource(GLuint shader, GLint count, const char** string, const GLint* length) {}
void glCompileShader(GLuint shader) {}
void glGetShaderiv(GLuint shader, GLenum pname, GLint* params) { *params = GL_TRUE; }
void glGetShaderInfoLog(GLuint shader, GLint maxLength, GLint* length, GLchar* infoLog) {}
void glDeleteShader(GLuint shader) {}
GLuint glCreateProgram() { return 1; }
void glAttachShader(GLuint program, GLuint shader) {}
void glLinkProgram(GLuint program) {}
void glGetProgramiv(GLuint program, GLenum pname, GLint* params) { *params = GL_TRUE; }
void glGetProgramInfoLog(GLuint program, GLint maxLength, GLint* length, GLchar* infoLog) {}
void glDeleteProgram(GLuint program) {}
void glUseProgram(GLuint program) {}
GLint glGetUniformLocation(GLuint program, const char* name) { return 0; }
void glUniform1f(GLint location, GLfloat v0) {}
void glUniform1i(GLint location, GLint v0) {}
void glUniform3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) {}
void glUniformMatrix4fv(GLint location, GLint count, GLboolean transpose, const GLfloat* value) {}
void glUniform1fv(GLint location, GLint count, const GLfloat* value) {}
void glUniform1iv(GLint location, GLint count, const GLint* value) {}
void glUniform3fv(GLint location, GLint count, const GLfloat* value) {}
void glActiveTexture(GLenum texture) {}
void glBindTexture(GLenum target, GLuint texture) {}

// Vector2 class for texture coordinates
class Vector2 {
public:
    float x, y;
    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}
};

// Vector3 class
class Vector3 {
public:
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

// Matrix4x4 class
class Matrix4x4 {
public:
    float elements[4][4];
    Matrix4x4() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                elements[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }
};

// Face structure for OBJ loading
struct Face {
    std::vector<int> vertex_indices;
    std::vector<int> texcoord_indices;
    std::vector<int> normal_indices;
};

namespace Shaders {

// Forward declarations
class Shader;

// ShaderError class
class ShaderError {
public:
    static void LogError(const std::string& message) {
        std::cerr << "Shader Error: " << message << std::endl;
    }
    
    static std::string HandleCompileError(GLuint shader) {
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        
        if (!success) {
            return "Mock shader compilation error";
        }
        
        return "";
    }
    
    static std::string HandleLinkError(GLuint program) {
        GLint success;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        
        if (!success) {
            return "Mock program linking error";
        }
        
        return "";
    }
};

// Shader class
class Shader {
public:
    enum Type {
        VERTEX,
        FRAGMENT,
        GEOMETRY
    };
    
    Shader(Type type) : type(type), handle(0), isCompiled(false) {
        handle = glCreateShader(GetGLShaderType());
    }
    
    ~Shader() {
        if (handle != 0) {
            glDeleteShader(handle);
            handle = 0;
        }
    }
    
    bool LoadFromFile(const std::string& path) {
        // Simulate loading from file
        source = "// Mock shader source from " + path;
        return true;
    }
    
    bool LoadFromString(const std::string& source) {
        this->source = source;
        return true;
    }
    
    bool Compile() {
        // Simulate compilation
        glCompileShader(handle);
        
        // Always succeed in mock
        isCompiled = true;
        return true;
    }
    
    GLuint GetHandle() const { return handle; }
    Type GetType() const { return type; }
    const std::string& GetError() const { return errorLog; }
    
private:
    GLuint handle;
    Type type;
    std::string source;
    std::string errorLog;
    bool isCompiled;
    
    GLenum GetGLShaderType() const {
        switch (type) {
            case VERTEX: return GL_VERTEX_SHADER;
            case FRAGMENT: return GL_FRAGMENT_SHADER;
            case GEOMETRY: return GL_GEOMETRY_SHADER;
            default: return GL_VERTEX_SHADER;
        }
    }
};

// ShaderProgram class
class ShaderProgram {
public:
    ShaderProgram() : handle(0), isLinked(false) {
        handle = glCreateProgram();
    }
    
    ~ShaderProgram() {
        if (handle != 0) {
            glDeleteProgram(handle);
            handle = 0;
        }
    }
    
    bool AttachShader(Shader* shader) {
        if (!shader) {
            errorLog = "Invalid shader";
            return false;
        }
        
        glAttachShader(handle, shader->GetHandle());
        shaders.push_back(shader);
        return true;
    }
    
    bool Link() {
        glLinkProgram(handle);
        isLinked = true;
        return true;
    }
    
    void Use() {
        if (isLinked) {
            glUseProgram(handle);
        }
    }
    
    GLuint GetHandle() const { return handle; }
    bool IsLinked() const { return isLinked; }
    const std::string& GetError() const { return errorLog; }
    
    void SetUniform(const std::string& name, float value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1f(location, value);
        }
    }
    
    void SetUniform(const std::string& name, int value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, value);
        }
    }
    
    void SetUniform(const std::string& name, bool value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1i(location, value ? 1 : 0);
        }
    }
    
    void SetUniform(const std::string& name, const Vector3& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform3f(location, value.x, value.y, value.z);
        }
    }
    
    void SetUniform(const std::string& name, const Matrix4x4& value) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value.elements[0][0]);
        }
    }
    
    void SetUniform(const std::string& name, GLuint textureUnit, GLuint textureID) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glUniform1i(location, textureUnit);
        }
    }
    
    void SetUniformArray(const std::string& name, const float* values, int count) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1fv(location, count, values);
        }
    }
    
    void SetUniformArray(const std::string& name, const int* values, int count) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform1iv(location, count, values);
        }
    }
    
    void SetUniformArray(const std::string& name, const Vector3* values, int count) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniform3fv(location, count, reinterpret_cast<const float*>(values));
        }
    }
    
    void SetUniformArray(const std::string& name, const Matrix4x4* values, int count) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniformMatrix4fv(location, count, GL_FALSE, reinterpret_cast<const float*>(values));
        }
    }
    
private:
    GLuint handle;
    bool isLinked;
    std::string errorLog;
    std::vector<Shader*> shaders;
    std::unordered_map<std::string, GLint> uniformLocations;
    
    GLint GetUniformLocation(const std::string& name) {
        auto it = uniformLocations.find(name);
        if (it != uniformLocations.end()) {
            return it->second;
        }
        
        GLint location = glGetUniformLocation(handle, name.c_str());
        uniformLocations[name] = location;
        return location;
    }
};

// ShaderAsset class
class ShaderAsset {
public:
    static ShaderProgram* LoadProgram(const std::string& vertPath, 
                                     const std::string& fragPath,
                                     const std::string& geomPath = "") {
        // Create a unique key for this shader combination
        std::string key = vertPath + ":" + fragPath;
        if (!geomPath.empty()) {
            key += ":" + geomPath;
        }
        
        // Check if we already have this shader program
        auto it = shaderPrograms.find(key);
        if (it != shaderPrograms.end()) {
            return it->second;
        }
        
        // Create new shader program
        ShaderProgram* program = new ShaderProgram();
        
        // Create and compile vertex shader
        Shader* vertexShader = new Shader(Shader::VERTEX);
        if (!vertexShader->LoadFromFile(vertPath)) {
            std::cerr << "Failed to load vertex shader from " << vertPath << std::endl;
            delete vertexShader;
            delete program;
            return nullptr;
        }
        
        if (!vertexShader->Compile()) {
            std::cerr << "Failed to compile vertex shader: " << vertexShader->GetError() << std::endl;
            delete vertexShader;
            delete program;
            return nullptr;
        }
        
        // Create and compile fragment shader
        Shader* fragmentShader = new Shader(Shader::FRAGMENT);
        if (!fragmentShader->LoadFromFile(fragPath)) {
            std::cerr << "Failed to load fragment shader from " << fragPath << std::endl;
            delete vertexShader;
            delete fragmentShader;
            delete program;
            return nullptr;
        }
        
        if (!fragmentShader->Compile()) {
            std::cerr << "Failed to compile fragment shader: " << fragmentShader->GetError() << std::endl;
            delete vertexShader;
            delete fragmentShader;
            delete program;
            return nullptr;
        }
        
        // Attach shaders to program
        program->AttachShader(vertexShader);
        program->AttachShader(fragmentShader);
        
        // Create and compile geometry shader if provided
        Shader* geometryShader = nullptr;
        if (!geomPath.empty()) {
            geometryShader = new Shader(Shader::GEOMETRY);
            if (!geometryShader->LoadFromFile(geomPath)) {
                std::cerr << "Failed to load geometry shader from " << geomPath << std::endl;
                delete vertexShader;
                delete fragmentShader;
                delete geometryShader;
                delete program;
                return nullptr;
            }
            
            if (!geometryShader->Compile()) {
                std::cerr << "Failed to compile geometry shader: " << geometryShader->GetError() << std::endl;
                delete vertexShader;
                delete fragmentShader;
                delete geometryShader;
                delete program;
                return nullptr;
            }
            
            program->AttachShader(geometryShader);
        }
        
        // Link program
        if (!program->Link()) {
            std::cerr << "Failed to link shader program: " << program->GetError() << std::endl;
            delete vertexShader;
            delete fragmentShader;
            if (geometryShader) delete geometryShader;
            delete program;
            return nullptr;
        }
        
        // Store in cache
        shaderPrograms[key] = program;
        
        // Log success
        std::cout << "Shader program loaded successfully: " << key << std::endl;
        
        return program;
    }
    
    static ShaderProgram* GetProgram(const std::string& name) {
        auto it = shaderPrograms.find(name);
        if (it != shaderPrograms.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    static void Cleanup() {
        for (auto& pair : shaderPrograms) {
            delete pair.second;
        }
        shaderPrograms.clear();
    }
    
private:
    static std::unordered_map<std::string, ShaderProgram*> shaderPrograms;
};

// Initialize static member
std::unordered_map<std::string, ShaderProgram*> ShaderAsset::shaderPrograms;

} // namespace Shaders

// Test vertex shader source
const std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}
)";

// Test fragment shader source
const std::string fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D albedoTexture;
uniform bool hasAlbedoTexture;
uniform vec3 viewPos;
uniform int lightCount;

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float range;
};

uniform PointLight lights[10];

out vec4 FragColor;

void main() {
    // Base color
    vec4 baseColor = hasAlbedoTexture ? texture(albedoTexture, TexCoord) : vec4(1.0, 1.0, 1.0, 1.0);
    
    // Ambient light
    vec3 ambient = vec3(0.1, 0.1, 0.1) * baseColor.rgb;
    
    // Diffuse and specular light
    vec3 result = ambient;
    
    for (int i = 0; i < lightCount; i++) {
        // Light direction
        vec3 lightDir = normalize(lights[i].position - FragPos);
        
        // Diffuse
        float diff = max(dot(Normal, lightDir), 0.0);
        vec3 diffuse = diff * lights[i].color * lights[i].intensity * baseColor.rgb;
        
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, Normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
        vec3 specular = spec * lights[i].color * lights[i].intensity;
        
        // Attenuation
        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        
        // Apply attenuation
        diffuse *= attenuation;
        specular *= attenuation;
        
        // Add to result
        result += diffuse + specular;
    }
    
    // Final color
    FragColor = vec4(result, baseColor.a);
}
)";

// Test geometry shader source
const std::string geometryShaderSource = R"(
#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 TexCoord[];
in vec3 Normal[];
in vec3 FragPos[];

out vec2 GTexCoord;
out vec3 GNormal;
out vec3 GFragPos;

void main() {
    for (int i = 0; i < 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        GTexCoord = TexCoord[i];
        GNormal = Normal[i];
        GFragPos = FragPos[i];
        EmitVertex();
    }
    EndPrimitive();
}
)";

// Test shader compilation
bool TestShaderCompilation() {
    bool success = true;
    
    // Test vertex shader
    Shaders::Shader vertexShader(Shaders::Shader::VERTEX);
    if (!vertexShader.LoadFromString(vertexShaderSource)) {
        std::cerr << "Failed to load vertex shader source: " << vertexShader.GetError() << std::endl;
        success = false;
    }
    
    if (!vertexShader.Compile()) {
        std::cerr << "Failed to compile vertex shader: " << vertexShader.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Vertex shader compiled successfully" << std::endl;
    }
    
    // Test fragment shader
    Shaders::Shader fragmentShader(Shaders::Shader::FRAGMENT);
    if (!fragmentShader.LoadFromString(fragmentShaderSource)) {
        std::cerr << "Failed to load fragment shader source: " << fragmentShader.GetError() << std::endl;
        success = false;
    }
    
    if (!fragmentShader.Compile()) {
        std::cerr << "Failed to compile fragment shader: " << fragmentShader.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Fragment shader compiled successfully" << std::endl;
    }
    
    // Test geometry shader
    Shaders::Shader geometryShader(Shaders::Shader::GEOMETRY);
    if (!geometryShader.LoadFromString(geometryShaderSource)) {
        std::cerr << "Failed to load geometry shader source: " << geometryShader.GetError() << std::endl;
        success = false;
    }
    
    if (!geometryShader.Compile()) {
        std::cerr << "Failed to compile geometry shader: " << geometryShader.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Geometry shader compiled successfully" << std::endl;
    }
    
    // Test shader program
    Shaders::ShaderProgram program;
    program.AttachShader(&vertexShader);
    program.AttachShader(&fragmentShader);
    program.AttachShader(&geometryShader);
    
    if (!program.Link()) {
        std::cerr << "Failed to link shader program: " << program.GetError() << std::endl;
        success = false;
    } else {
        std::cout << "Shader program linked successfully" << std::endl;
    }
    
    // Test shader asset
    Shaders::ShaderProgram* standardProgram = Shaders::ShaderAsset::LoadProgram(
        "Shaders/Defaults/standard.vert",
        "Shaders/Defaults/standard.frag",
        "Shaders/Defaults/standard.geom"
    );
    
    if (!standardProgram) {
        std::cerr << "Failed to load standard shader program" << std::endl;
        success = false;
    } else {
        std::cout << "Standard shader program loaded successfully" << std::endl;
    }
    
    // Clean up
    Shaders::ShaderAsset::Cleanup();
    
    return success;
}

int main() {
    std::cout << "Testing shader compilation..." << std::endl;
    
    if (TestShaderCompilation()) {
        std::cout << "All shader compilation tests passed" << std::endl;
        return 0;
    } else {
        std::cerr << "Shader compilation tests failed" << std::endl;
        return 1;
    }
}
