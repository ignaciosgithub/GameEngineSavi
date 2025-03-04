#pragma once
#include <string>
#include <vector>
#include "ThirdParty/OpenGL/include/GL/platform_gl.h"

/**
 * @brief Utility class for capturing OpenGL frame buffer to PNG image files
 */
class FrameCapture_PNG {
public:
    /**
     * @brief Captures the current viewport to a PNG file
     * @param filename The output filename
     * @param width The width of the capture (0 for current viewport width)
     * @param height The height of the capture (0 for current viewport height)
     * @return True if the capture was successful
     */
    static bool CaptureViewportToFile(const std::string& filename, int width = 0, int height = 0);
    
    /**
     * @brief Reads the current frame buffer
     * @param width Output parameter for the width of the frame buffer
     * @param height Output parameter for the height of the frame buffer
     * @return The pixel data as a vector of unsigned chars (RGB format)
     */
    static std::vector<unsigned char> ReadFrameBuffer(int& width, int& height);
    
    /**
     * @brief Saves pixel data to a PNG file
     * @param filename The output filename
     * @param pixels The pixel data in RGBA format
     * @param width The width of the image
     * @param height The height of the image
     * @return True if the save was successful
     */
    static bool SavePixelsToFile(const std::string& filename, const unsigned char* pixels, int width, int height);
    
    /**
     * @brief Creates a simulated frame for testing
     * @param filename The output filename
     * @param width The width of the image
     * @param height The height of the image
     * @param frameNumber The frame number (used for varying the image)
     * @return True if the frame was created successfully
     */
    static bool CreateSimulatedFrame(const std::string& filename, int width, int height, int frameNumber);
};
