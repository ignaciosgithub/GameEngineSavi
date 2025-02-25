#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include "ProjectSettings.h"
#include <string>
#include <vector>

// Project manager class for handling project operations
class ProjectManager {
private:
    std::string currentProjectPath;
    std::vector<std::string> recentProjects;
    
public:
    ProjectManager();
    
    // Create a new project
    bool CreateProject(const std::string& projectName, const std::string& projectPath);
    
    // Open an existing project
    bool OpenProject(const std::string& projectPath);
    
    // Save the current project
    bool SaveProject();
    
    // Close the current project
    void CloseProject();
    
    // Get the current project path
    std::string GetCurrentProjectPath() const { return currentProjectPath; }
    
    // Get the list of recent projects
    std::vector<std::string> GetRecentProjects() const { return recentProjects; }
    
    // Add a project to the recent projects list
    void AddToRecentProjects(const std::string& projectPath);
    
    // Check if a project exists at the given path
    bool ProjectExists(const std::string& projectPath) const;
};

#endif // PROJECT_MANAGER_H
