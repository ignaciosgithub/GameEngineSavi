/**
 * FrameCapture_png.cpp
 * Implementation of PNG frame capture functionality using stb_image_write.h
 */

#include "FrameCapture_png.h"
#include "platform.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cmath>
#include "Graphics/Core/GraphicsAPIFactory.h"

// Include stb_image_write.h for PNG writing
#include "ThirdParty/stb/stb_image_write.h"

bool FrameCapture_PNG::CaptureViewportToFile(const std::string& filename, int width, int height) {
    // Get the graphics API
    auto graphics = GraphicsAPIFactory::GetInstance().GetGraphicsAPI();
    if (!graphics) {
        std::cerr << "Failed to get graphics API for frame capture" << std::endl;
        return false;
    }
    
    // Use default dimensions if not specified
    if (width <= 0 || height <= 0) {
        // Get the current viewport dimensions from the graphics API
        int viewport[4];
        graphics->GetViewport(viewport);
        width = viewport[2];
        height = viewport[3];
    }
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        std::cerr << "Invalid viewport dimensions: " << width << "x" << height << std::endl;
        // Use safe default values
        width = 800;
        height = 600;
    }
    
    std::cout << "Capturing viewport to file: " << filename << " (" << width << "x" << height << ")" << std::endl;
    
    // Read the pixels from the framebuffer using the graphics API
    std::vector<unsigned char> pixels(width * height * 4);
    graphics->ReadPixels(0, 0, width, height, pixels.data());
    
    // Flip the image vertically (OpenGL has origin at bottom-left, PNG at top-left)
    std::vector<unsigned char> flipped(width * height * 4);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int c = 0; c < 4; c++) {
                flipped[(y * width + x) * 4 + c] = pixels[((height - 1 - y) * width + x) * 4 + c];
            }
        }
    }
    
    // Write the PNG file using stb_image_write
    int result = stbi_write_png(
        filename.c_str(),
        width,
        height,
        4,  // RGBA components
        flipped.data(),
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

bool FrameCapture_PNG::SavePixelsToFile(const std::string& filename, const unsigned char* pixels, int width, int height) {
    std::cout << "Saving pixels to file: " << filename << " (" << width << "x" << height << ")" << std::endl;
    
    // Validate dimensions
    if (width <= 0 || height <= 0 || width > 4096 || height > 4096) {
        std::cerr << "Invalid image dimensions: " << width << "x" << height << std::endl;
        return false;
    }
    
    // Write the PNG file using stb_image_write
    // Note: stb_image_write.h handles opening and closing the file internally
    int result = stbi_write_png(
        filename.c_str(),
        width,
        height,
        4,  // RGBA components
        pixels,
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

bool FrameCapture_PNG::CreateSimulatedFrame(const std::string& filename, int width, int height, int frameNumber) {
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
    
    // Save the frame to file
    return SavePixelsToFile(filename, pixels.data(), width, height);
}
