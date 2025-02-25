#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include "platform.h"
#include <string>
#include <memory>
#include <map>
#include <vector>

// Forward declarations
class ProjectSettings;

// Project settings singleton
class ProjectSettings {
private:
    static std::unique_ptr<ProjectSettings> instance;
    
    // Project data
    std::string projectName;
    std::string projectPath;
    std::string engineVersion;
    
    // Build settings
    struct BuildSettings {
        bool debugSymbols;
        bool optimization;
        std::vector<std::string> targetPlatforms;
        std::string outputDirectory;
    } buildSettings;
    
    // Engine settings
    struct EngineSettings {
        // Physics settings
        struct PhysicsSettings {
            float fixedTimeStep;
            float gravity;
            bool enableCollisions;
        } physics;
        
        // Rendering settings
        struct RenderSettings {
            int targetFPS;
            bool vsync;
            int msaa;
            bool shadows;
        } rendering;
    } engineSettings;
    
    // Asset paths
    std::map<std::string, std::string> assetPaths;
    
    // Private constructor for singleton
    ProjectSettings();
    
public:
    // Get singleton instance
    static ProjectSettings& GetInstance();
    
    // Load project settings from JSON file
    bool LoadFromFile(const std::string& filePath);
    
    // Save project settings to JSON file
    bool SaveToFile(const std::string& filePath);
    
    // Create a new project with default settings
    bool CreateNewProject(const std::string& projectName, const std::string& projectPath);
    
    // Getters and setters
    std::string GetProjectName() const { return projectName; }
    void SetProjectName(const std::string& name) { projectName = name; }
    
    std::string GetProjectPath() const { return projectPath; }
    void SetProjectPath(const std::string& path) { projectPath = path; }
    
    std::string GetEngineVersion() const { return engineVersion; }
    
    // Build settings
    bool GetDebugSymbols() const { return buildSettings.debugSymbols; }
    void SetDebugSymbols(bool enable) { buildSettings.debugSymbols = enable; }
    
    bool GetOptimization() const { return buildSettings.optimization; }
    void SetOptimization(bool enable) { buildSettings.optimization = enable; }
    
    std::vector<std::string> GetTargetPlatforms() const { return buildSettings.targetPlatforms; }
    void SetTargetPlatforms(const std::vector<std::string>& platforms) { buildSettings.targetPlatforms = platforms; }
    
    std::string GetOutputDirectory() const { return buildSettings.outputDirectory; }
    void SetOutputDirectory(const std::string& dir) { buildSettings.outputDirectory = dir; }
    
    // Physics settings
    float GetFixedTimeStep() const { return engineSettings.physics.fixedTimeStep; }
    void SetFixedTimeStep(float timeStep) { engineSettings.physics.fixedTimeStep = timeStep; }
    
    float GetGravity() const { return engineSettings.physics.gravity; }
    void SetGravity(float gravity) { engineSettings.physics.gravity = gravity; }
    
    bool GetEnableCollisions() const { return engineSettings.physics.enableCollisions; }
    void SetEnableCollisions(bool enable) { engineSettings.physics.enableCollisions = enable; }
    
    // Rendering settings
    int GetTargetFPS() const { return engineSettings.rendering.targetFPS; }
    void SetTargetFPS(int fps) { engineSettings.rendering.targetFPS = fps; }
    
    bool GetVSync() const { return engineSettings.rendering.vsync; }
    void SetVSync(bool enable) { engineSettings.rendering.vsync = enable; }
    
    int GetMSAA() const { return engineSettings.rendering.msaa; }
    void SetMSAA(int msaa) { engineSettings.rendering.msaa = msaa; }
    
    bool GetShadows() const { return engineSettings.rendering.shadows; }
    void SetShadows(bool enable) { engineSettings.rendering.shadows = enable; }
    
    // Asset paths
    std::string GetAssetPath(const std::string& assetType) const;
    void SetAssetPath(const std::string& assetType, const std::string& path);
};

#endif // PROJECT_SETTINGS_H
