/**
 * simple_text_controlled_editor.cpp
 * A simplified version of the text controlled editor that doesn't depend on the full engine.
 * This version demonstrates the text parser functionality without requiring all dependencies.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <sys/stat.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(dir) mkdir(dir)
#else
#include <unistd.h>
#define MKDIR(dir) mkdir(dir, 0755)
#endif

// Constants
const std::string COMMAND_FILE = "frames/commands.txt";
const std::string COMMAND_SEPARATOR = ",";
const std::string RUNNING_FILE = "frames/running.txt";
const std::string STATUS_FILE = "frames/status.txt";

// Function declarations
void createFramesDirectory();
bool checkCommandFile(std::string& key, float& duration);
void clearCommandFile();
void simulateKeyPress(const std::string& key, float duration);

// Create frames directory if it doesn't exist
void createFramesDirectory() {
    const char* framesDir = "frames";
    struct stat st = {0};
    
    if (stat(framesDir, &st) == -1) {
        if (MKDIR(framesDir) != 0) {
            std::cerr << "Error creating frames directory" << std::endl;
        } else {
            std::cout << "Created frames directory" << std::endl;
        }
    }
}

// Check command file for new commands
bool checkCommandFile(std::string& key, float& duration) {
    // Check if the file exists
    struct stat st = {0};
    if (stat(COMMAND_FILE.c_str(), &st) == -1) {
        // Create an empty command file if it doesn't exist
        std::ofstream commandFile(COMMAND_FILE);
        if (commandFile.is_open()) {
            commandFile.close();
            std::cout << "Created empty command file: " << COMMAND_FILE << std::endl;
        } else {
            std::cerr << "Failed to create command file: " << COMMAND_FILE << std::endl;
        }
        return false;
    }
    
    // Check if the file is empty
    if (st.st_size == 0) {
        return false;
    }
    
    // Read the command file
    std::ifstream commandFile(COMMAND_FILE);
    if (!commandFile.is_open()) {
        std::cerr << "Failed to open command file: " << COMMAND_FILE << std::endl;
        return false;
    }
    
    std::string line;
    if (std::getline(commandFile, line)) {
        // Parse the command
        size_t separatorPos = line.find(COMMAND_SEPARATOR);
        if (separatorPos != std::string::npos) {
            key = line.substr(0, separatorPos);
            std::string durationStr = line.substr(separatorPos + 1);
            
            try {
                duration = std::stof(durationStr);
                commandFile.close();
                
                // Clear the command file
                clearCommandFile();
                
                std::cout << "Read command: Key=" << key << ", Duration=" << duration << std::endl;
                return true;
            } catch (const std::exception& e) {
                std::cerr << "Failed to parse duration: " << durationStr << std::endl;
            }
        } else {
            std::cerr << "Invalid command format: " << line << std::endl;
        }
    }
    
    commandFile.close();
    return false;
}

// Clear the command file
void clearCommandFile() {
    std::ofstream commandFile(COMMAND_FILE, std::ofstream::trunc);
    if (commandFile.is_open()) {
        commandFile.close();
        std::cout << "Cleared command file: " << COMMAND_FILE << std::endl;
    } else {
        std::cerr << "Failed to clear command file: " << COMMAND_FILE << std::endl;
    }
}

// Simulate a key press (just print to console in this simplified version)
void simulateKeyPress(const std::string& key, float duration) {
    std::cout << "Simulating key press: " << key << " for " << duration << " seconds" << std::endl;
    
    // In a real implementation, this would use pydirectinput or similar
    // For this simplified version, we just sleep for the duration
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(duration * 1000)));
    
    std::cout << "Released key: " << key << " after " << duration << " seconds" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "Starting Simple Text Controlled Editor..." << std::endl;
    
    // Create frames directory
    createFramesDirectory();
    
    // Create an empty command file if it doesn't exist
    std::ofstream commandFile(COMMAND_FILE);
    if (commandFile.is_open()) {
        commandFile.close();
        std::cout << "Created empty command file: " << COMMAND_FILE << std::endl;
    } else {
        std::cerr << "WARNING: Could not create command file" << std::endl;
    }
    
    // Create a signal file to check for exit requests
    std::ofstream exitSignalFile(RUNNING_FILE);
    if (exitSignalFile.is_open()) {
        exitSignalFile << "running" << std::endl;
        exitSignalFile.close();
    }
    
    // Signal that initialization is complete
    std::ofstream statusFile(STATUS_FILE);
    if (statusFile.is_open()) {
        statusFile << "initialized" << std::endl;
        statusFile.close();
        std::cout << "Created status file to indicate initialization is complete" << std::endl;
    } else {
        std::cerr << "WARNING: Could not create status file" << std::endl;
    }
    
    std::cout << "Starting continuous command processing. To stop, delete frames/running.txt" << std::endl;
    std::cout << "To control the editor, write commands to frames/commands.txt in the format KEY,DURATION" << std::endl;
    std::cout << "Example: A,1 (press A key for 1 second)" << std::endl;
    
    // Variables for key press simulation
    std::string currentKey = "";
    float keyPressDuration = 0.0f;
    bool keyIsPressed = false;
    
    // Main loop
    bool running = true;
    while (running) {
        // Check if we should exit (if running.txt file is deleted)
        std::ifstream checkRunning(RUNNING_FILE);
        if (!checkRunning.good()) {
            std::cout << "Exit signal detected (frames/running.txt not found). Stopping." << std::endl;
            running = false;
            break;
        }
        checkRunning.close();
        
        // Check for new commands if no key is currently being pressed
        if (!keyIsPressed) {
            std::string key;
            float duration;
            if (checkCommandFile(key, duration)) {
                currentKey = key;
                keyPressDuration = duration;
                keyIsPressed = true;
                
                // Simulate the key press
                simulateKeyPress(currentKey, keyPressDuration);
                
                // Reset key press state
                keyIsPressed = false;
            }
        }
        
        // Small delay to not overwhelm the system
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Simple text controlled editor complete." << std::endl;
    
    return 0;
}
