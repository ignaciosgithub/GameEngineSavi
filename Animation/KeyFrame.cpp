#include "KeyFrame.h"
#include "../Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

bool KeyFrame::LoadFromOBJ() {
    // If no OBJ file path is specified, return false
    if (objFilePath.empty()) {
        return false;
    }
    
    // Open the OBJ file
    std::ifstream file(objFilePath);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << objFilePath << std::endl;
        return false;
    }
    
    // Clear existing vertex positions
    vertexPositions.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        // Only process vertex positions (lines starting with 'v')
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            
            // Add vertex position to the vector
            vertexPositions.push_back(x);
            vertexPositions.push_back(y);
            vertexPositions.push_back(z);
        }
    }
    
    file.close();
    return !vertexPositions.empty();
}
