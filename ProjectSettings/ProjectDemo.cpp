#include "ProjectSettings.h"
#include "ProjectManager.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  std::cout << "Project Management System Demo" << std::endl;
  std::cout << "==============================" << std::endl;
  
  // Check if a project path was provided
  std::string projectPath = (argc > 1) ? argv[1] : "TestProject";
  
  // Get the project manager instance
  auto& manager = ProjectManager::GetInstance();
  
  // Create a new project if it doesn't exist
  std::cout << "Creating a new project..." << std::endl;
  if (manager.CreateProject("Test Project", projectPath)) {
    std::cout << "Project created successfully!" << std::endl;
  } else {
    std::cout << "Failed to create project." << std::endl;
  }
  
  // Open the project
  manager.OpenProject(projectPath);
  
  // Get the project settings
  auto& settings = ProjectSettings::GetInstance();
  
  // Modify some settings
  settings.SetProjectName("Modified Test Project");
  settings.SetTargetFPS(120);
  settings.SetVSync(false);
  
  // Save the project
  std::cout << "Saving project settings..." << std::endl;
  if (manager.SaveProject()) {
    std::cout << "Project saved successfully!" << std::endl;
  } else {
    std::cout << "Failed to save project." << std::endl;
  }
  
  // Close the project
  std::cout << "Closing project..." << std::endl;
  manager.CloseProject();
  
  // Reopen the project to verify changes
  std::cout << "Opening project..." << std::endl;
  if (manager.OpenProject(projectPath)) {
    std::cout << "Project opened successfully!" << std::endl;
  } else {
    std::cout << "Failed to open project." << std::endl;
  }
  
  // Display the project settings
  std::cout << "Project settings:" << std::endl;
  std::cout << "  Project name: " << settings.GetProjectName() << std::endl;
  std::cout << "  Target FPS: " << settings.GetTargetFPS() << std::endl;
  std::cout << "  VSync: " << (settings.GetVSync() ? "Enabled" : "Disabled") << std::endl;
  std::cout << "  Fixed time step: " << settings.GetFixedTimeStep() << std::endl;
  std::cout << "  Gravity: " << settings.GetGravity() << std::endl;
  
  std::cout << "Demo completed successfully!" << std::endl;
  return 0;
}
