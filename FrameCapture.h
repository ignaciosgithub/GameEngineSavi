#pragma once
#include <string>
#include <vector>
#include "gl_types.h"

/**
 * @brief Utility class for capturing OpenGL frame buffer to image files
 * 
 * This class provides functionality to capture the current OpenGL viewport
 * and save it as an image file. It's primarily used for taking screenshots
 * of the editor interface and scene view.
 */
class FrameCapture {
public:
    /**
     * @brief Captures the current viewport and saves it to a file
     * 
     * @param filename Path to save the image file
     * @return true if the capture and save was successful
     * @return false if there was an error
     */
    static bool CaptureViewportToFile(const std::string& filename);
    
    /**
     * @brief Captures the current viewport with specified dimensions
     * 
     * @param filename Path to save the image file
     * @param width Width of the capture area
     * @param height Height of the capture area
     * @return true if the capture and save was successful
     * @return false if there was an error
     */
    static bool CaptureViewportToFile(const std::string& filename, int width, int height);

private:
    /**
     * @brief Reads pixel data from the OpenGL frame buffer
     * 
     * @param width Output parameter that will contain the width of the captured area
     * @param height Output parameter that will contain the height of the captured area
     * @return std::vector<unsigned char> Raw pixel data in RGBA format
     */
    static std::vector<unsigned char> ReadFrameBuffer(int& width, int& height);
    
    /**
     * @brief Saves raw pixel data as a PNG file
     * 
     * @param filename Path to save the PNG file
     * @param data Raw pixel data in RGBA format
     * @param width Width of the image
     * @param height Height of the image
     * @return true if the save was successful
     * @return false if there was an error
     */
    static bool SavePNG(const std::string& filename, const std::vector<unsigned char>& data, 
                       int width, int height);
                       
    /**
     * @brief Flips the image vertically
     * 
     * OpenGL frame buffer has (0,0) at the bottom left, but most image formats
     * have (0,0) at the top left, so we need to flip the image vertically.
     * 
     * @param data Raw pixel data to flip
     * @param width Width of the image
     * @param height Height of the image
     * @return std::vector<unsigned char> Flipped pixel data
     */
    static std::vector<unsigned char> FlipVertically(const std::vector<unsigned char>& data, 
                                                   int width, int height);
};
