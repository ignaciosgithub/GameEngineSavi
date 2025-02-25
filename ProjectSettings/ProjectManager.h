#ifndef PROJECT_MANAGER_H
#define PROJECT_MANAGER_H

#include "ProjectSettings.h"
#include <string>
#include <memory>
#include <fstream>

class ProjectManager {
private:
    // Singleton instance
    static std::unique_ptr<ProjectManager> instance;
    
    // Current project path
    std::string currentProjectPath;
    
    // Private constructor for singleton
    ProjectManager() {}
    
public:
    // Get singleton instance
    static ProjectManager& GetInstance();
    
    // Create a new project
    bool CreateProject(const std::string& name, const std::string& path);
    
    // Check if a project exists at the specified path
    bool ProjectExists(const std::string& path);
    
    // Open an existing project
    bool OpenProject(const std::string& path);
    
    // Save the current project
    bool SaveProject();
    
    // Close the current project
    void CloseProject();
    
    // Get the current project path
    std::string GetCurrentProjectPath() const;
};

#endif // PROJECT_MANAGER_H
