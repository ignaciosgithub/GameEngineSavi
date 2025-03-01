#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include "../platform.h"
#include <string>
#include <vector>
#include <map>
#include <memory>

class ProjectSettings {
private:
    // Singleton instance
    static std::unique_ptr<ProjectSettings> instance;
    
    // Project data
    std::string projectName;
    std::string engineVersion;
    std::string projectPath;
    
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
        struct RenderingSettings {
            int targetFPS;
            bool vsync;
            int msaa;
            bool shadows;
        } rendering;
        
        // Network settings
        struct NetworkSettings {
            bool enableNetworking;
            std::string defaultServerAddress;
            int defaultServerPort;
            bool enablePacketLogging;
            float simulatedLatency;
            float simulatedPacketLoss;
            bool preferP2P;
        } network;
        
        // Audio settings
        struct AudioSettings {
            float masterVolume;
            int sampleRate;
            int channels;
            bool enableAudio;
        } audio;
        
        // Navigation settings
        struct NavigationSettings {
            float navMeshRefreshRate;
            float maxAngleDiff;
            float maxDist;
        } navigation;
    } engineSettings;
    
    // Asset paths
    std::map<std::string, std::string> assetPaths;
    
    // Private constructor for singleton
    ProjectSettings();
    
    // Platform-specific directory creation
    bool CreateDirectories(const std::string& path);
    
public:
    // Get singleton instance
    static ProjectSettings& GetInstance();
    
    // Load project settings from JSON file
    bool LoadFromFile(const std::string& filePath);
    
    // Save project settings to JSON file
    bool SaveToFile(const std::string& filePath);
    
    // Create a new project with default settings
    bool CreateNewProject(const std::string& name, const std::string& path);
    
    // Getters and setters
    std::string GetProjectName() const;
    void SetProjectName(const std::string& name);
    
    std::string GetEngineVersion() const;
    void SetEngineVersion(const std::string& version);
    
    bool GetDebugSymbols() const;
    void SetDebugSymbols(bool enabled);
    
    bool GetOptimization() const;
    void SetOptimization(bool enabled);
    
    std::vector<std::string> GetTargetPlatforms() const;
    void SetTargetPlatforms(const std::vector<std::string>& platforms);
    
    std::string GetOutputDirectory() const;
    void SetOutputDirectory(const std::string& directory);
    
    float GetFixedTimeStep() const;
    void SetFixedTimeStep(float timeStep);
    
    float GetGravity() const;
    void SetGravity(float gravity);
    
    bool GetEnableCollisions() const;
    void SetEnableCollisions(bool enabled);
    
    int GetTargetFPS() const;
    void SetTargetFPS(int fps);
    
    bool GetVSync() const;
    void SetVSync(bool enabled);
    
    int GetMSAA() const;
    void SetMSAA(int msaa);
    
    bool GetShadows() const;
    void SetShadows(bool enabled);
    
    // Audio settings getters and setters
    float GetMasterVolume() const;
    void SetMasterVolume(float volume);
    
    int GetSampleRate() const;
    void SetSampleRate(int sampleRate);
    
    int GetAudioChannels() const;
    void SetAudioChannels(int channels);
    
    bool GetEnableAudio() const;
    void SetEnableAudio(bool enabled);
    
    // Navigation settings getters and setters
    float GetNavMeshRefreshRate() const;
    void SetNavMeshRefreshRate(float rate);
    
    float GetMaxAngleDiff() const;
    void SetMaxAngleDiff(float angle);
    
    float GetMaxDist() const;
    void SetMaxDist(float distance);
    
    std::string GetAssetPath(const std::string& type) const;
    void SetAssetPath(const std::string& type, const std::string& path);
};

#endif // PROJECT_SETTINGS_H
