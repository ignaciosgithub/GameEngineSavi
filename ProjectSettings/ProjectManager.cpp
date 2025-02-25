#include "ProjectManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>

ProjectManager::ProjectManager() : currentProjectPath("") {
    // Load recent projects from a file
    // In a real implementation, we would load this from a file
    // For now, we'll just initialize with an empty list
}

// Create a new project
bool ProjectManager::CreateProject(const std::string& projectName, const std::string& projectPath) {
    // Check if a project already exists at the given path
    if (ProjectExists(projectPath)) {
        std::cout << "A project already exists at " << projectPath << std::endl;
        return false;
    }
    
    // Create a new project using the ProjectSettings singleton
    bool success = ProjectSettings::GetInstance().CreateNewProject(projectName, projectPath);
    
    if (success) {
        // Set the current project path
        currentProjectPath = projectPath;
        
        // Add to recent projects
        AddToRecentProjects(projectPath);
        
        std::cout << "Project created successfully at " << projectPath << std::endl;
    }
    
    return success;
}

// Open an existing project
bool ProjectManager::OpenProject(const std::string& projectPath) {
    // Check if the project exists
    if (!ProjectExists(projectPath)) {
        std::cout << "No project found at " << projectPath << std::endl;
        return false;
    }
    
    // Load the project settings
    bool success = ProjectSettings::GetInstance().LoadFromFile(projectPath + "/project.json");
    
    if (success) {
        // Set the current project path
        currentProjectPath = projectPath;
        
        // Add to recent projects
        AddToRecentProjects(projectPath);
        
        std::cout << "Project opened successfully from " << projectPath << std::endl;
    }
    
    return success;
}

// Save the current project
bool ProjectManager::SaveProject() {
    // Check if a project is currently open
    if (currentProjectPath.empty()) {
        std::cout << "No project is currently open" << std::endl;
        return false;
    }
    
    // Save the project settings
    bool success = ProjectSettings::GetInstance().SaveToFile(currentProjectPath + "/project.json");
    
    if (success) {
        std::cout << "Project saved successfully to " << currentProjectPath << std::endl;
    }
    
    return success;
}

// Close the current project
void ProjectManager::CloseProject() {
    // Check if a project is currently open
    if (currentProjectPath.empty()) {
        std::cout << "No project is currently open" << std::endl;
        return;
    }
    
    // Reset the current project path
    currentProjectPath = "";
    
    std::cout << "Project closed" << std::endl;
}

// Add a project to the recent projects list
void ProjectManager::AddToRecentProjects(const std::string& projectPath) {
    // Check if the project is already in the list
    auto it = std::find(recentProjects.begin(), recentProjects.end(), projectPath);
    
    // If it's already in the list, remove it so we can add it to the front
    if (it != recentProjects.end()) {
        recentProjects.erase(it);
    }
    
    // Add the project to the front of the list
    recentProjects.insert(recentProjects.begin(), projectPath);
    
    // Limit the list to 10 projects
    if (recentProjects.size() > 10) {
        recentProjects.resize(10);
    }
    
    // In a real implementation, we would save the list to a file
}

// Check if a project exists at the given path
bool ProjectManager::ProjectExists(const std::string& projectPath) const {
    // Check if the project.json file exists
    std::ifstream file(projectPath + "/project.json");
    return file.good();
}
