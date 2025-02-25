#include "ProjectManager.h"
#include <iostream>

// Initialize static instance
std::unique_ptr<ProjectManager> ProjectManager::instance = nullptr;

// Get singleton instance
ProjectManager& ProjectManager::GetInstance() {
    if (!instance) {
        instance.reset(new ProjectManager());
    }
    return *instance;
}

// Create a new project
bool ProjectManager::CreateProject(const std::string& name, const std::string& path) {
    // Check if a project already exists at the specified path
    if (ProjectExists(path)) {
        std::cout << "A project already exists at " << path << std::endl;
        return false;
    }
    
    // Create a new project
    auto& settings = ProjectSettings::GetInstance();
    if (!settings.CreateNewProject(name, path)) {
        return false;
    }
    
    // Set the current project path
    currentProjectPath = path;
    
    std::cout << "Project created successfully at " << currentProjectPath << std::endl;
    return true;
}

// Check if a project exists at the specified path
bool ProjectManager::ProjectExists(const std::string& path) {
    std::ifstream file(path + "/project.json");
    return file.good();
}

// Open an existing project
bool ProjectManager::OpenProject(const std::string& path) {
    // Check if a project exists at the specified path
    if (!ProjectExists(path)) {
        std::cout << "No project found at " << path << std::endl;
        return false;
    }
    
    // Load the project settings
    auto& settings = ProjectSettings::GetInstance();
    if (!settings.LoadFromFile(path + "/project.json")) {
        return false;
    }
    
    // Set the current project path
    currentProjectPath = path;
    
    std::cout << "Project opened successfully from " << currentProjectPath << std::endl;
    return true;
}

// Save the current project
bool ProjectManager::SaveProject() {
    // Check if a project is currently open
    if (currentProjectPath.empty()) {
        std::cout << "No project is currently open" << std::endl;
        return false;
    }
    
    // Save the project settings
    auto& settings = ProjectSettings::GetInstance();
    if (!settings.SaveToFile(currentProjectPath + "/project.json")) {
        return false;
    }
    
    std::cout << "Project saved successfully to " << currentProjectPath << std::endl;
    return true;
}

// Close the current project
void ProjectManager::CloseProject() {
    // Check if a project is currently open
    if (currentProjectPath.empty()) {
        std::cout << "No project is currently open" << std::endl;
        return;
    }
    
    // Clear the current project path
    currentProjectPath = "";
    
    std::cout << "Project closed" << std::endl;
}

// Get the current project path
std::string ProjectManager::GetCurrentProjectPath() const {
    return currentProjectPath;
}
