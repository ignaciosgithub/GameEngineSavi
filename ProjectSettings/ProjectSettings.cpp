#include "ProjectSettings.h"
#include "../ThirdParty/json/json.hpp"
#include <fstream>
#include <iostream>
#include <sys/stat.h>

using json = nlohmann::json;

// Initialize static instance
std::unique_ptr<ProjectSettings> ProjectSettings::instance = nullptr;

// Get singleton instance
ProjectSettings& ProjectSettings::GetInstance() {
    if (!instance) {
        instance.reset(new ProjectSettings());
    }
    return *instance;
}

// Constructor with default values
ProjectSettings::ProjectSettings() 
    : projectName("New Project"), 
      engineVersion("1.0.0") {
    
    // Default build settings
    buildSettings.debugSymbols = true;
    buildSettings.optimization = false;
    buildSettings.targetPlatforms = {"Windows", "Linux"};
    buildSettings.outputDirectory = "Build";
    
    // Default physics settings
    engineSettings.physics.fixedTimeStep = 1.0f / 60.0f;
    engineSettings.physics.gravity = -9.81f;
    engineSettings.physics.enableCollisions = true;
    
    // Default rendering settings
    engineSettings.rendering.targetFPS = 60;
    engineSettings.rendering.vsync = true;
    engineSettings.rendering.msaa = 4;
    engineSettings.rendering.shadows = true;
    
    // Default network settings
    engineSettings.network.enableNetworking = false;
    engineSettings.network.defaultServerAddress = "127.0.0.1";
    engineSettings.network.defaultServerPort = 7777;
    engineSettings.network.enablePacketLogging = false;
    engineSettings.network.simulatedLatency = 0.0f;
    engineSettings.network.simulatedPacketLoss = 0.0f;
    engineSettings.network.preferP2P = false;
    
    // Default audio settings
    engineSettings.audio.masterVolume = 1.0f;
    engineSettings.audio.sampleRate = 44100;
    engineSettings.audio.channels = 2;
    engineSettings.audio.enableAudio = true;
    
    // Default asset paths
    assetPaths["models"] = "Assets/Models";
    assetPaths["textures"] = "Assets/Textures";
    assetPaths["sounds"] = "Assets/Sounds";
    assetPaths["scripts"] = "Assets/Scripts";
}

// Load project settings from JSON file
bool ProjectSettings::LoadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return false;
    }
    
    try {
        json j;
        file >> j;
        
        projectName = j["projectName"];
        engineVersion = j["engineVersion"];
        
        // Load build settings
        buildSettings.debugSymbols = j["buildSettings"]["debugSymbols"];
        buildSettings.optimization = j["buildSettings"]["optimization"];
        buildSettings.targetPlatforms = j["buildSettings"]["targetPlatforms"].get<std::vector<std::string>>();
        buildSettings.outputDirectory = j["buildSettings"]["outputDirectory"];
        
        // Load physics settings
        engineSettings.physics.fixedTimeStep = j["engineSettings"]["physics"]["fixedTimeStep"];
        engineSettings.physics.gravity = j["engineSettings"]["physics"]["gravity"];
        engineSettings.physics.enableCollisions = j["engineSettings"]["physics"]["enableCollisions"];
        
        // Load rendering settings
        engineSettings.rendering.targetFPS = j["engineSettings"]["rendering"]["targetFPS"];
        engineSettings.rendering.vsync = j["engineSettings"]["rendering"]["vsync"];
        engineSettings.rendering.msaa = j["engineSettings"]["rendering"]["msaa"];
        engineSettings.rendering.shadows = j["engineSettings"]["rendering"]["shadows"];
        
        // Load network settings if they exist
        if (j["engineSettings"].contains("network")) {
            engineSettings.network.enableNetworking = j["engineSettings"]["network"]["enableNetworking"];
            engineSettings.network.defaultServerAddress = j["engineSettings"]["network"]["defaultServerAddress"];
            engineSettings.network.defaultServerPort = j["engineSettings"]["network"]["defaultServerPort"];
            engineSettings.network.enablePacketLogging = j["engineSettings"]["network"]["enablePacketLogging"];
            engineSettings.network.simulatedLatency = j["engineSettings"]["network"]["simulatedLatency"];
            engineSettings.network.simulatedPacketLoss = j["engineSettings"]["network"]["simulatedPacketLoss"];
            engineSettings.network.preferP2P = j["engineSettings"]["network"]["preferP2P"];
        }
        
        // Load audio settings if they exist
        if (j["engineSettings"].contains("audio")) {
            engineSettings.audio.masterVolume = j["engineSettings"]["audio"]["masterVolume"];
            engineSettings.audio.sampleRate = j["engineSettings"]["audio"]["sampleRate"];
            engineSettings.audio.channels = j["engineSettings"]["audio"]["channels"];
            engineSettings.audio.enableAudio = j["engineSettings"]["audio"]["enableAudio"];
        }
        
        // Load asset paths
        auto assetPathsJson = j["assetPaths"];
        for (auto it = assetPathsJson.begin(); it != assetPathsJson.end(); ++it) {
            assetPaths[it.key()] = it.value();
        }
        
        std::cout << "Successfully loaded project settings from " << filePath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading project settings: " << e.what() << std::endl;
        return false;
    }
}

// Save project settings to JSON file
bool ProjectSettings::SaveToFile(const std::string& filePath) {
    try {
        // Create JSON object
        json j;
        
        // Project data
        j["projectName"] = projectName;
        j["engineVersion"] = engineVersion;
        
        // Build settings
        j["buildSettings"]["debugSymbols"] = buildSettings.debugSymbols;
        j["buildSettings"]["optimization"] = buildSettings.optimization;
        j["buildSettings"]["targetPlatforms"] = buildSettings.targetPlatforms;
        j["buildSettings"]["outputDirectory"] = buildSettings.outputDirectory;
        
        // Physics settings
        j["engineSettings"]["physics"]["fixedTimeStep"] = engineSettings.physics.fixedTimeStep;
        j["engineSettings"]["physics"]["gravity"] = engineSettings.physics.gravity;
        j["engineSettings"]["physics"]["enableCollisions"] = engineSettings.physics.enableCollisions;
        
        // Rendering settings
        j["engineSettings"]["rendering"]["targetFPS"] = engineSettings.rendering.targetFPS;
        j["engineSettings"]["rendering"]["vsync"] = engineSettings.rendering.vsync;
        j["engineSettings"]["rendering"]["msaa"] = engineSettings.rendering.msaa;
        j["engineSettings"]["rendering"]["shadows"] = engineSettings.rendering.shadows;
        
        // Network settings
        j["engineSettings"]["network"]["enableNetworking"] = engineSettings.network.enableNetworking;
        j["engineSettings"]["network"]["defaultServerAddress"] = engineSettings.network.defaultServerAddress;
        j["engineSettings"]["network"]["defaultServerPort"] = engineSettings.network.defaultServerPort;
        j["engineSettings"]["network"]["enablePacketLogging"] = engineSettings.network.enablePacketLogging;
        j["engineSettings"]["network"]["simulatedLatency"] = engineSettings.network.simulatedLatency;
        j["engineSettings"]["network"]["simulatedPacketLoss"] = engineSettings.network.simulatedPacketLoss;
        j["engineSettings"]["network"]["preferP2P"] = engineSettings.network.preferP2P;
        
        // Audio settings
        j["engineSettings"]["audio"]["masterVolume"] = engineSettings.audio.masterVolume;
        j["engineSettings"]["audio"]["sampleRate"] = engineSettings.audio.sampleRate;
        j["engineSettings"]["audio"]["channels"] = engineSettings.audio.channels;
        j["engineSettings"]["audio"]["enableAudio"] = engineSettings.audio.enableAudio;
        
        // Asset paths
        for (auto it = assetPaths.begin(); it != assetPaths.end(); ++it) {
            j["assetPaths"][it->first] = it->second;
        }
        
        // Create directory if it doesn't exist
        std::string directory = filePath.substr(0, filePath.find_last_of("/\\"));
        CreateDirectories(directory);
        
        // Write to file
        std::ofstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filePath << " for writing" << std::endl;
            return false;
        }
        
        file << std::setw(4) << j << std::endl;
        std::cout << "Successfully saved project settings to " << filePath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving project settings: " << e.what() << std::endl;
        return false;
    }
}

// Create a new project with default settings
bool ProjectSettings::CreateNewProject(const std::string& name, const std::string& path) {
    projectName = name;
    projectPath = path;
    
    // Create project directory structure
    try {
        CreateDirectories(path);
        CreateDirectories(path + "/Assets");
        CreateDirectories(path + "/Assets/Models");
        CreateDirectories(path + "/Assets/Textures");
        CreateDirectories(path + "/Assets/Sounds");
        CreateDirectories(path + "/Assets/Scripts");
        CreateDirectories(path + "/Build");
        
        std::cout << "Created project directory structure at " << path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating project directories: " << e.what() << std::endl;
        return false;
    }
    
    // Save default settings to project file
    return SaveToFile(path + "/project.json");
}

// Platform-specific directory creation
bool ProjectSettings::CreateDirectories(const std::string& path) {
    #ifdef PLATFORM_WINDOWS
    // Windows implementation
    std::string current;
    size_t pos = 0;
    
    while ((pos = path.find_first_of("/\\", pos)) != std::string::npos) {
        current = path.substr(0, pos);
        if (!current.empty()) {
            if (mkdir(current.c_str()) != 0 && errno != EEXIST) {
                return false;
            }
        }
        pos++;
    }
    
    return mkdir(path.c_str()) == 0 || errno == EEXIST;
    #else
    // Linux/Unix implementation
    std::string current;
    size_t pos = 0;
    
    while ((pos = path.find_first_of("/", pos)) != std::string::npos) {
        current = path.substr(0, pos);
        if (!current.empty()) {
            if (mkdir(current.c_str(), 0755) != 0 && errno != EEXIST) {
                return false;
            }
        }
        pos++;
    }
    
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
    #endif
}

// Getters and setters
std::string ProjectSettings::GetProjectName() const {
    return projectName;
}

void ProjectSettings::SetProjectName(const std::string& name) {
    projectName = name;
}

std::string ProjectSettings::GetEngineVersion() const {
    return engineVersion;
}

void ProjectSettings::SetEngineVersion(const std::string& version) {
    engineVersion = version;
}

bool ProjectSettings::GetDebugSymbols() const {
    return buildSettings.debugSymbols;
}

void ProjectSettings::SetDebugSymbols(bool enabled) {
    buildSettings.debugSymbols = enabled;
}

bool ProjectSettings::GetOptimization() const {
    return buildSettings.optimization;
}

void ProjectSettings::SetOptimization(bool enabled) {
    buildSettings.optimization = enabled;
}

std::vector<std::string> ProjectSettings::GetTargetPlatforms() const {
    return buildSettings.targetPlatforms;
}

void ProjectSettings::SetTargetPlatforms(const std::vector<std::string>& platforms) {
    buildSettings.targetPlatforms = platforms;
}

std::string ProjectSettings::GetOutputDirectory() const {
    return buildSettings.outputDirectory;
}

void ProjectSettings::SetOutputDirectory(const std::string& directory) {
    buildSettings.outputDirectory = directory;
}

float ProjectSettings::GetFixedTimeStep() const {
    return engineSettings.physics.fixedTimeStep;
}

void ProjectSettings::SetFixedTimeStep(float timeStep) {
    engineSettings.physics.fixedTimeStep = timeStep;
}

float ProjectSettings::GetGravity() const {
    return engineSettings.physics.gravity;
}

void ProjectSettings::SetGravity(float gravity) {
    engineSettings.physics.gravity = gravity;
}

bool ProjectSettings::GetEnableCollisions() const {
    return engineSettings.physics.enableCollisions;
}

void ProjectSettings::SetEnableCollisions(bool enabled) {
    engineSettings.physics.enableCollisions = enabled;
}

int ProjectSettings::GetTargetFPS() const {
    return engineSettings.rendering.targetFPS;
}

void ProjectSettings::SetTargetFPS(int fps) {
    engineSettings.rendering.targetFPS = fps;
}

bool ProjectSettings::GetVSync() const {
    return engineSettings.rendering.vsync;
}

void ProjectSettings::SetVSync(bool enabled) {
    engineSettings.rendering.vsync = enabled;
}

int ProjectSettings::GetMSAA() const {
    return engineSettings.rendering.msaa;
}

void ProjectSettings::SetMSAA(int msaa) {
    engineSettings.rendering.msaa = msaa;
}

bool ProjectSettings::GetShadows() const {
    return engineSettings.rendering.shadows;
}

void ProjectSettings::SetShadows(bool enabled) {
    engineSettings.rendering.shadows = enabled;
}

// Audio settings getters and setters
float ProjectSettings::GetMasterVolume() const {
    return engineSettings.audio.masterVolume;
}

void ProjectSettings::SetMasterVolume(float volume) {
    // Clamp volume between 0 and 1
    engineSettings.audio.masterVolume = (volume < 0.0f) ? 0.0f : ((volume > 1.0f) ? 1.0f : volume);
}

int ProjectSettings::GetSampleRate() const {
    return engineSettings.audio.sampleRate;
}

void ProjectSettings::SetSampleRate(int sampleRate) {
    engineSettings.audio.sampleRate = sampleRate;
}

int ProjectSettings::GetAudioChannels() const {
    return engineSettings.audio.channels;
}

void ProjectSettings::SetAudioChannels(int channels) {
    engineSettings.audio.channels = channels;
}

bool ProjectSettings::GetEnableAudio() const {
    return engineSettings.audio.enableAudio;
}

void ProjectSettings::SetEnableAudio(bool enabled) {
    engineSettings.audio.enableAudio = enabled;
}

std::string ProjectSettings::GetAssetPath(const std::string& type) const {
    auto it = assetPaths.find(type);
    if (it != assetPaths.end()) {
        return it->second;
    }
    return "";
}

void ProjectSettings::SetAssetPath(const std::string& type, const std::string& path) {
    assetPaths[type] = path;
}
