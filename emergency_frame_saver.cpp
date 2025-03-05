/**
 * emergency_frame_saver.cpp
 * Simple program to create simulated frames for the GameEngineSavi engine.
 * 
 * This version uses direct file writing instead of OpenGL functions to avoid
 * linking issues.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>
#include <cmath>

// Define STB_IMAGE_WRITE_IMPLEMENTATION before including stb_image_write.h
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image_write.h"

// Function to create a simulated frame
bool CreateSimulatedFrame(const std::string& filename, int width, int height, int frameNumber) {
    std::cout << "Creating simulated frame: " << filename << " (" << width << "x" << height << ")" << std::endl;
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        std::cerr << "Invalid image dimensions: " << width << "x" << height << std::endl;
        return false;
    }
    
    // Create pixel data
    std::vector<unsigned char> pixels(width * height * 4);
    
    // Generate a simple colored image
    float rotationAngle = frameNumber * 36.0f; // 360 degrees / 10 frames = 36 degrees per frame
    float angleRad = rotationAngle * 3.14159f / 180.0f;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Normalize coordinates to [-1, 1]
            float nx = (float)x / width * 2.0f - 1.0f;
            float ny = (float)y / height * 2.0f - 1.0f;
            
            // Simple perspective projection
            float z = 0.0f;
            float distance = sqrt(nx*nx + ny*ny + z*z);
            
            // Apply rotation (simplified)
            float angle = atan2(ny, nx) + angleRad;
            float radius = sqrt(nx*nx + ny*ny);
            float rx = radius * cos(angle);
            float ry = radius * sin(angle);
            
            // Calculate lighting (simplified)
            float lightFactor = std::max(0.2f, 1.0f - distance);
            
            // Calculate color based on position and lighting
            int r, g, b;
            
            // Cube visualization (simplified)
            if (distance < 0.5f) {
                // Inside cube area - use different colors for different faces
                if (rx > 0 && ry > 0) {
                    r = static_cast<int>(255 * lightFactor);
                    g = static_cast<int>(0 * lightFactor);
                    b = static_cast<int>(0 * lightFactor);
                } else if (rx < 0 && ry > 0) {
                    r = static_cast<int>(0 * lightFactor);
                    g = static_cast<int>(255 * lightFactor);
                    b = static_cast<int>(0 * lightFactor);
                } else if (rx < 0 && ry < 0) {
                    r = static_cast<int>(0 * lightFactor);
                    g = static_cast<int>(0 * lightFactor);
                    b = static_cast<int>(255 * lightFactor);
                } else {
                    r = static_cast<int>(255 * lightFactor);
                    g = static_cast<int>(255 * lightFactor);
                    b = static_cast<int>(0 * lightFactor);
                }
            } else {
                // Background
                r = static_cast<int>(50 * lightFactor);
                g = static_cast<int>(50 * lightFactor);
                b = static_cast<int>(80 * lightFactor);
            }
            
            // Ensure color values are in valid range
            r = std::min(255, std::max(0, r));
            g = std::min(255, std::max(0, g));
            b = std::min(255, std::max(0, b));
            
            // Set pixel color (RGBA format)
            int index = (y * width + x) * 4;
            pixels[index] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255; // Alpha
        }
    }
    
    // Save the frame to file using stb_image_write
    int result = stbi_write_png(
        filename.c_str(),
        width,
        height,
        4,  // RGBA components
        pixels.data(),
        width * 4  // Stride in bytes
    );
    
    if (result == 0) {
        std::cerr << "Failed to write PNG file: " << filename << std::endl;
        return false;
    }
    
    // Verify the file was written correctly
    std::ifstream verifyFile(filename, std::ios::binary);
    if (!verifyFile.is_open()) {
        std::cerr << "Warning: Could not verify file was written: " << filename << std::endl;
        return true; // Still return true as stb_image_write reported success
    }
    
    verifyFile.seekg(0, std::ios::end);
    size_t fileSize = verifyFile.tellg();
    verifyFile.close(); // Explicitly close the verification file stream
    
    if (fileSize < 100) {
        std::cerr << "Warning: Generated file is suspiciously small: " << fileSize << " bytes" << std::endl;
    } else {
        std::cout << "Successfully wrote " << fileSize << " bytes to " << filename << std::endl;
    }
    
    return true;
}

int main(int argc, char** argv) {
    std::cout << "Starting emergency frame saver..." << std::endl;
    
    // Create frames directory
    system("mkdir -p frames");
    
    // Generate 10 simulated frames
    const int numFrames = 10;
    const int width = 800;
    const int height = 600;
    
    for (int i = 0; i < numFrames; i++) {
        std::stringstream ss;
        ss << "frames/frame" << i << ".png";
        std::string filename = ss.str();
        
        std::cout << "Creating frame " << i << "..." << std::endl;
        
        // Create simulated frame
        if (CreateSimulatedFrame(filename, width, height, i)) {
            std::cout << "Frame " << i << " created successfully." << std::endl;
        } else {
            std::cerr << "Failed to create frame " << i << "." << std::endl;
        }
        
        // Small delay to not overwhelm the system
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Emergency frame saving complete. " << numFrames << " frames created." << std::endl;
    
    return 0;
}
