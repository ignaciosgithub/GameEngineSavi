#ifndef PROJECT_PANEL_H
#define PROJECT_PANEL_H

#include "../GUI/GUI.h"
#include <vector>
#include <string>
#include <experimental/filesystem>

// Forward declarations
class Editor;

// Project panel that displays project files
class ProjectPanel : public Panel {
private:
    Editor* editor;
    std::string currentDirectory;
    std::vector<std::string> files;
    std::vector<std::string> directories;
    
    // Scroll position
    int scrollOffset;
    
    // Item height
    const int itemHeight = 20;
    
    // Drag and drop
    bool isDragging;
    std::string draggedFile;
    
public:
    ProjectPanel(float x, float y, float w, float h, Editor* editor);
    
    // Draw the panel
    void Draw() override;
    
    // Handle input
    bool HandleInput(int mouseX, int mouseY, bool clicked) override;
    
    // Update file list
    void UpdateFileList();
    
    // Navigate to directory
    void NavigateToDirectory(const std::string& directory);
    
    // Start dragging a file
    void StartDragging(const std::string& file);
    
    // Stop dragging
    void StopDragging();
    
    // Is dragging
    bool IsDragging() const;
    
    // Get dragged file
    std::string GetDraggedFile() const;
};

#endif // PROJECT_PANEL_H
