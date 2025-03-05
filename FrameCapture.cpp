#include "FrameCapture.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "ThirdParty/OpenGL/include/GL/platform_gl.h"

// Capture the current viewport to a file
bool FrameCapture::CaptureViewportToFile(const std::string& filename, int width, int height) {
    // Get the current viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Use the current viewport dimensions if not specified
    if (width <= 0) {
        width = viewport[2];
    }
    if (height <= 0) {
        height = viewport[3];
    }
    
    // Set the viewport to the desired dimensions
    glViewport(0, 0, width, height);
    
    // Read the pixels from the framebuffer
    std::vector<unsigned char> pixels(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    
    // Restore the original viewport
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    
    // Write the pixels to a file
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return false;
    }
    
    // Write the header
    file << "P6\n" << width << " " << height << "\n255\n";
    
    // Write the pixels (convert RGBA to RGB)
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;
            file.write(reinterpret_cast<const char*>(&pixels[index]), 3);
        }
    }
    
    file.close();
    
    return true;
}

// Read the current frame buffer
std::vector<unsigned char> FrameCapture::ReadFrameBuffer(int& width, int& height) {
    // Get the current viewport
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    
    // Use the current viewport dimensions
    width = viewport[2];
    height = viewport[3];
    
    // Read the pixels from the framebuffer
    std::vector<unsigned char> pixels(width * height * 4);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    
    // Check for errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << error << std::endl;
    }
    
    return pixels;
}
