#include "FrameCapture.h"
#include <iostream>
#include <fstream>
#include <cstring>

// Define STB_IMAGE_WRITE_IMPLEMENTATION before including stb_image_write.h
// to create the implementation in this file
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ThirdParty/stb/stb_image_write.h"

bool FrameCapture::CaptureViewportToFile(const std::string& filename) {
    int width, height;
    auto pixels = ReadFrameBuffer(width, height);
    if (pixels.empty()) {
        std::cerr << "Failed to read frame buffer" << std::endl;
        return false;
    }
    
    // Flip the image vertically since OpenGL has (0,0) at bottom left
    auto flippedPixels = FlipVertically(pixels, width, height);
    
    return SavePNG(filename, flippedPixels, width, height);
}

bool FrameCapture::CaptureViewportToFile(const std::string& filename, int width, int height) {
    // Save current viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Set viewport to requested dimensions
    glViewport(0, 0, width, height);
    
    // Read pixels
    std::vector<unsigned char> pixels(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    
    // Restore original viewport
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    
    if (pixels.empty()) {
        std::cerr << "Failed to read frame buffer with custom dimensions" << std::endl;
        return false;
    }
    
    // Flip the image vertically
    auto flippedPixels = FlipVertically(pixels, width, height);
    
    return SavePNG(filename, flippedPixels, width, height);
}

std::vector<unsigned char> FrameCapture::ReadFrameBuffer(int& width, int& height) {
    // Get current viewport dimensions
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    width = viewport[2];
    height = viewport[3];
    
    // Allocate memory for pixel data (RGBA format)
    std::vector<unsigned char> pixels(width * height * 4);
    
    // Read pixels from the framebuffer
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    
    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error in ReadFrameBuffer: " << error << std::endl;
        return std::vector<unsigned char>();
    }
    
    return pixels;
}

bool FrameCapture::SavePNG(const std::string& filename, 
                          const std::vector<unsigned char>& data,
                          int width, int height) {
    // Create directory if it doesn't exist
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        std::string directory = filename.substr(0, lastSlash);
        
        #ifdef PLATFORM_WINDOWS
        std::string command = "mkdir \"" + directory + "\" 2> nul";
        #else
        std::string command = "mkdir -p \"" + directory + "\"";
        #endif
        
        system(command.c_str());
    }
    
    // Save the image using stb_image_write
    int result = stbi_write_png(filename.c_str(), width, height, 4, data.data(), width * 4);
    
    if (result == 0) {
        std::cerr << "Failed to save PNG file: " << filename << std::endl;
        return false;
    }
    
    std::cout << "Screenshot saved to: " << filename << std::endl;
    return true;
}

std::vector<unsigned char> FrameCapture::FlipVertically(const std::vector<unsigned char>& data, 
                                                      int width, int height) {
    std::vector<unsigned char> flipped(data.size());
    
    // Flip the image vertically (OpenGL has (0,0) at bottom left)
    for (int y = 0; y < height; y++) {
        // Copy each row to the flipped position
        std::memcpy(
            flipped.data() + (height - 1 - y) * width * 4,
            data.data() + y * width * 4,
            width * 4
        );
    }
    
    return flipped;
}
