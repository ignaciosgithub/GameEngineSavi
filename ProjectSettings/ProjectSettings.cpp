#include "ProjectSettings.h"
#include <fstream>
#include <iostream>

// Initialize static instance
std::unique_ptr<ProjectSettings> ProjectSettings::instance = nullptr;

// Private constructor
ProjectSettings::ProjectSettings() : 
    projectName("New Project"),
    projectPath(""),
    engineVersion("1.0.0")
{
    // Default build settings
    buildSettings.debugSymbols = true;
    buildSettings.optimization = false;
    buildSettings.targetPlatforms = {"Windows", "Linux"};
    buildSettings.outputDirectory = "Build";
    
    // Default physics settings
    engineSettings.physics.fixedTimeStep = 1.0f / 60.0f;
    engineSettings.physics.gravity = 9.81f;
    engineSettings.physics.enableCollisions = true;
    
    // Default rendering settings
    engineSettings.rendering.targetFPS = 60;
    engineSettings.rendering.vsync = true;
    engineSettings.rendering.msaa = 4;
    engineSettings.rendering.shadows = true;
    
    // Default asset paths
    assetPaths["models"] = "Assets/Models";
    assetPaths["textures"] = "Assets/Textures";
    assetPaths["sounds"] = "Assets/Sounds";
    assetPaths["scripts"] = "Assets/Scripts";
}

// Get singleton instance
ProjectSettings& ProjectSettings::GetInstance() {
    if (!instance) {
        instance.reset(new ProjectSettings());
    }
    return *instance;
}

// Load project settings from JSON file
bool ProjectSettings::LoadFromFile(const std::string& filePath) {
    // In a real implementation, we would use a JSON library to parse the file
    // For now, we'll just simulate loading from a file
    std::cout << "Loading project settings from " << filePath << std::endl;
    
    // TODO: Implement actual JSON parsing
    // For now, just return true to simulate successful loading
    return true;
}

// Save project settings to JSON file
bool ProjectSettings::SaveToFile(const std::string& filePath) {
    // In a real implementation, we would use a JSON library to generate the file
    // For now, we'll just simulate saving to a file
    std::cout << "Saving project settings to " << filePath << std::endl;
    
    // TODO: Implement actual JSON generation
    // For now, just return true to simulate successful saving
    return true;
}

// Create a new project with default settings
bool ProjectSettings::CreateNewProject(const std::string& name, const std::string& path) {
    projectName = name;
    projectPath = path;
    
    // Create project directory structure
    // In a real implementation, we would create the necessary directories
    // For now, we'll just simulate creating a new project
    std::cout << "Creating new project " << name << " at " << path << std::endl;
    
    // Save default settings to project file
    return SaveToFile(path + "/project.json");
}

// Get asset path
std::string ProjectSettings::GetAssetPath(const std::string& assetType) const {
    auto it = assetPaths.find(assetType);
    if (it != assetPaths.end()) {
        return it->second;
    }
    return "";
}

// Set asset path
void ProjectSettings::SetAssetPath(const std::string& assetType, const std::string& path) {
    assetPaths[assetType] = path;
}
