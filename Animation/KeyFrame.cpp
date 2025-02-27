#include "KeyFrame.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace Animation {

bool KeyFrame::LoadFromOBJ() {
    // Open the OBJ file
    std::ifstream file(objFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << objFilePath << std::endl;
        return false;
    }
    
    // Clear existing vertex positions
    vertexPositions.clear();
    
    // Parse the OBJ file
    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Parse vertex positions (lines starting with 'v')
        if (line[0] == 'v' && line[1] == ' ') {
            std::istringstream iss(line.substr(2));
            float x, y, z;
            if (iss >> x >> y >> z) {
                vertexPositions.push_back(x);
                vertexPositions.push_back(y);
                vertexPositions.push_back(z);
            }
        }
    }
    
    // Check if we loaded any vertices
    if (vertexPositions.empty()) {
        std::cerr << "No vertices found in OBJ file: " << objFilePath << std::endl;
        return false;
    }
    
    return true;
}

} // namespace Animation
