#include "ProjectSettings/ProjectManager.h"
#include "ProjectSettings/ProjectSettings.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    std::cout << "Project Management System Demo" << std::endl;
    std::cout << "==============================" << std::endl;
    
    // Create a project manager
    ProjectManager projectManager;
    
    // Create a new project
    std::string projectName = "Test Project";
    std::string projectPath = "./TestProject";
    
    std::cout << "Creating a new project..." << std::endl;
    if (projectManager.CreateProject(projectName, projectPath)) {
        std::cout << "Project created successfully!" << std::endl;
    } else {
        std::cout << "Failed to create project." << std::endl;
        return 1;
    }
    
    // Modify some settings
    ProjectSettings& settings = ProjectSettings::GetInstance();
    settings.SetTargetFPS(120);
    settings.SetVSync(false);
    settings.SetGravity(9.8f);
    
    // Save the project
    std::cout << "Saving project settings..." << std::endl;
    if (projectManager.SaveProject()) {
        std::cout << "Project saved successfully!" << std::endl;
    } else {
        std::cout << "Failed to save project." << std::endl;
        return 1;
    }
    
    // Close the project
    std::cout << "Closing project..." << std::endl;
    projectManager.CloseProject();
    
    // Open the project again
    std::cout << "Opening project..." << std::endl;
    if (projectManager.OpenProject(projectPath)) {
        std::cout << "Project opened successfully!" << std::endl;
    } else {
        std::cout << "Failed to open project." << std::endl;
        return 1;
    }
    
    // Display project settings
    std::cout << "Project settings:" << std::endl;
    std::cout << "  Project name: " << settings.GetProjectName() << std::endl;
    std::cout << "  Target FPS: " << settings.GetTargetFPS() << std::endl;
    std::cout << "  VSync: " << (settings.GetVSync() ? "Enabled" : "Disabled") << std::endl;
    std::cout << "  Gravity: " << settings.GetGravity() << std::endl;
    
    std::cout << "Demo completed successfully!" << std::endl;
    return 0;
}
