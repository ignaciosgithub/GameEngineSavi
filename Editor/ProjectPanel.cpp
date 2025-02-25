#include "ProjectPanel.h"
#include "Editor.h"
#include "../ProjectSettings/ProjectSettings.h"
#include "../platform.h"
#ifdef PLATFORM_LINUX
#include <GL/gl.h>
#elif defined(PLATFORM_WINDOWS)
#include <windows.h>
#include <GL/gl.h>
#endif
#include <iostream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

ProjectPanel::ProjectPanel(float x, float y, float w, float h, Editor* editor)
    : Panel(x, y, w, h), editor(editor), scrollOffset(0), isDragging(false) {
    
    // Set current directory to project directory or assets directory
    auto& settings = ProjectSettings::GetInstance();
    // Use project name as a fallback since GetProjectPath doesn't exist
    currentDirectory = "test_assets";
    
    // Update file list
    UpdateFileList();
}

void ProjectPanel::Draw() {
    // Draw panel background
    Panel::Draw();
    
    // Draw panel title
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 15);
    // Draw "Project" text (simplified for prototype)
    
    // Draw current directory
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(x + 5, y + 35);
    // Draw current directory text (simplified for prototype)
    
    // Draw directories
    int visibleItems = static_cast<int>((height - 50) / itemHeight);
    int startIndex = scrollOffset;
    int endIndex = std::min(startIndex + visibleItems, static_cast<int>(directories.size() + files.size()));
    
    for (int i = startIndex; i < endIndex; i++) {
        float itemY = y + 50 + (i - startIndex) * itemHeight;
        
        if (i < static_cast<int>(directories.size())) {
            // Draw directory
            glColor3f(0.7f, 0.7f, 0.0f);
            glBegin(GL_QUADS);
            glVertex2f(x + 5, itemY);
            glVertex2f(x + 15, itemY);
            glVertex2f(x + 15, itemY + 15);
            glVertex2f(x + 5, itemY + 15);
            glEnd();
            
            // Draw directory name
            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(x + 20, itemY + 12);
            // Draw directory name text (simplified for prototype)
        } else {
            // Draw file
            int fileIndex = i - directories.size();
            std::string fileName = files[fileIndex];
            std::string extension = fileName.substr(fileName.find_last_of(".") + 1);
            
            // Draw file icon based on extension
            if (extension == "obj") {
                // Draw OBJ icon
                glColor3f(0.0f, 0.7f, 0.0f);
            } else if (extension == "png" || extension == "jpg" || extension == "jpeg") {
                // Draw image icon
                glColor3f(0.0f, 0.0f, 0.7f);
            } else {
                // Draw generic file icon
                glColor3f(0.7f, 0.0f, 0.0f);
            }
            
            glBegin(GL_QUADS);
            glVertex2f(x + 5, itemY);
            glVertex2f(x + 15, itemY);
            glVertex2f(x + 15, itemY + 15);
            glVertex2f(x + 5, itemY + 15);
            glEnd();
            
            // Draw file name
            glColor3f(0.0f, 0.0f, 0.0f);
            glRasterPos2f(x + 20, itemY + 12);
            // Draw file name text (simplified for prototype)
        }
    }
    
    // Draw scroll bar if needed
    if (directories.size() + files.size() > visibleItems) {
        float scrollBarHeight = (visibleItems / static_cast<float>(directories.size() + files.size())) * (height - 50);
        float scrollBarY = y + 50 + (scrollOffset / static_cast<float>(directories.size() + files.size())) * (height - 50);
        
        glBegin(GL_QUADS);
        glColor3f(0.7f, 0.7f, 0.7f);
        glVertex2f(x + width - 10, scrollBarY);
        glVertex2f(x + width - 2, scrollBarY);
        glVertex2f(x + width - 2, scrollBarY + scrollBarHeight);
        glVertex2f(x + width - 10, scrollBarY + scrollBarHeight);
        glEnd();
    }
    
    // Draw dragged file if dragging
    if (isDragging) {
        // Get mouse position
        int mouseX, mouseY;
        // In a real implementation, we would get the mouse position from the input system
        // For now, we'll just use placeholder values
        mouseX = 400;
        mouseY = 300;
        
        // Draw file icon
        std::string extension = draggedFile.substr(draggedFile.find_last_of(".") + 1);
        
        // Draw file icon based on extension
        if (extension == "obj") {
            // Draw OBJ icon
            glColor3f(0.0f, 0.7f, 0.0f);
        } else if (extension == "png" || extension == "jpg" || extension == "jpeg") {
            // Draw image icon
            glColor3f(0.0f, 0.0f, 0.7f);
        } else {
            // Draw generic file icon
            glColor3f(0.7f, 0.0f, 0.0f);
        }
        
        glBegin(GL_QUADS);
        glVertex2f(mouseX, mouseY);
        glVertex2f(mouseX + 20, mouseY);
        glVertex2f(mouseX + 20, mouseY + 20);
        glVertex2f(mouseX, mouseY + 20);
        glEnd();
        
        // Draw file name
        glColor3f(0.0f, 0.0f, 0.0f);
        glRasterPos2f(mouseX + 25, mouseY + 15);
        // Draw file name text (simplified for prototype)
    }
}

bool ProjectPanel::HandleInput(int mouseX, int mouseY, bool clicked) {
    if (!Panel::HandleInput(mouseX, mouseY, clicked)) {
        return false;
    }
    
    // Check if mouse is over a directory or file
    if (mouseX >= x && mouseX <= x + width - 10 && mouseY >= y + 50) {
        int visibleItems = static_cast<int>((height - 50) / itemHeight);
        int startIndex = scrollOffset;
        int endIndex = std::min(startIndex + visibleItems, static_cast<int>(directories.size() + files.size()));
        
        for (int i = startIndex; i < endIndex; i++) {
            float itemY = y + 50 + (i - startIndex) * itemHeight;
            
            if (mouseY >= itemY && mouseY <= itemY + itemHeight) {
                if (clicked) {
                    if (i < static_cast<int>(directories.size())) {
                        // Navigate to directory
                        NavigateToDirectory(directories[i]);
                    } else {
                        // Start dragging file
                        int fileIndex = i - directories.size();
                        StartDragging(files[fileIndex]);
                    }
                }
                return true;
            }
        }
    }
    
    // Check if mouse is over scroll bar
    if (mouseX >= x + width - 10 && mouseX <= x + width && mouseY >= y + 50 && mouseY <= y + height) {
        // Handle scroll bar interaction
        // This would include dragging the scroll bar
        // For now, we'll just handle mouse wheel scrolling
        // in a real implementation
    }
    
    // Stop dragging if mouse is released
    if (!clicked && isDragging) {
        // Check if mouse is over scene view
        // In a real implementation, we would check if the mouse is over the scene view
        // For now, we'll just stop dragging
        StopDragging();
    }
    
    return true;
}

void ProjectPanel::UpdateFileList() {
    // Clear current lists
    directories.clear();
    files.clear();
    
    // Add parent directory
    if (currentDirectory != "test_assets") {
        directories.push_back("..");
    }
    
    // For now, just add some sample files for the prototype
    // In a real implementation, we would read the directory
    directories.push_back("Models");
    directories.push_back("Textures");
    directories.push_back("Scripts");
    
    files.push_back("cube.obj");
    files.push_back("sphere.obj");
    files.push_back("texture.png");
    files.push_back("normal_map.png");
    files.push_back("config.json");
    
    // Sort directories and files
    std::sort(directories.begin(), directories.end());
    std::sort(files.begin(), files.end());
}

void ProjectPanel::NavigateToDirectory(const std::string& directory) {
    if (directory == "..") {
        // Navigate to parent directory
        std::string path = currentDirectory;
        size_t lastSlash = path.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            currentDirectory = path.substr(0, lastSlash);
        }
    } else {
        // Navigate to subdirectory
        currentDirectory = currentDirectory + "/" + directory;
    }
    
    // Update file list
    UpdateFileList();
    
    std::cout << "Navigated to directory: " << currentDirectory << std::endl;
}

void ProjectPanel::StartDragging(const std::string& file) {
    isDragging = true;
    draggedFile = file;
    
    std::cout << "Started dragging file: " << draggedFile << std::endl;
}

void ProjectPanel::StopDragging() {
    if (isDragging) {
        // Handle drop
        // In a real implementation, we would check if the mouse is over the scene view
        // and handle the drop accordingly
        // For now, we'll just notify the editor
        if (editor) {
            editor->HandleDragAndDrop(currentDirectory + "/" + draggedFile);
        }
        
        isDragging = false;
        std::cout << "Stopped dragging file: " << draggedFile << std::endl;
    }
}

bool ProjectPanel::IsDragging() const {
    return isDragging;
}

std::string ProjectPanel::GetDraggedFile() const {
    return draggedFile;
}
