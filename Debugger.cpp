#include "Debugger.h"
#include "RedundancyDetector.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Initialize the singleton instance
Debugger* Debugger::instance = nullptr;

// Constructor
Debugger::Debugger() : showErrorPanel(false), showRedundancyPanel(false), showImportErrorPanel(false) {
    // Initialize the debugger
}

// Get the singleton instance
Debugger& Debugger::GetInstance() {
    if (!instance) {
        instance = new Debugger();
    }
    return *instance;
}

// Log an error with full context
void Debugger::LogError(const std::string& message, const std::string& scriptName,
                       const std::string& functionName, int lineNumber) {
    errors.push_back(ScriptError(message, scriptName, functionName, lineNumber));
    std::cerr << "[ERROR] " << message << " in " << scriptName << "::" << functionName << " at line " << lineNumber << std::endl;
}

// Log a simple error message
void Debugger::LogError(const std::string& message) {
    errors.push_back(ScriptError(message, "", "", -1));
    std::cerr << "[ERROR] " << message << std::endl;
}

// Method to log redundant declarations
void Debugger::LogRedundancy(const std::string& fileName, const std::string& symbolName,
                            const std::string& type, int lineNumber) {
    redundancies.push_back(RedundancyInfo(fileName, symbolName, type, lineNumber));
    std::cout << "[REDUNDANCY] " << symbolName << " (" << type << ") in " << fileName << " at line " << lineNumber << std::endl;
}

// Method to log import errors
void Debugger::LogImportError(const std::string& message, const std::string& filePath,
                             const std::string& importType) {
    importErrors.push_back(ImportError(message, filePath, importType));
    std::cerr << "[IMPORT ERROR] " << message << " for " << importType << " at " << filePath << std::endl;
}

// Clear all logged errors
void Debugger::ClearErrors() {
    errors.clear();
}

// Clear all logged redundancies
void Debugger::ClearRedundancies() {
    redundancies.clear();
}

// Clear all logged import errors
void Debugger::ClearImportErrors() {
    importErrors.clear();
}

// Get all current errors
const std::vector<Debugger::ScriptError>& Debugger::GetErrors() const {
    return errors;
}

// Get all current redundancies
const std::vector<Debugger::RedundancyInfo>& Debugger::GetRedundancies() const {
    return redundancies;
}

// Get all current import errors
const std::vector<Debugger::ImportError>& Debugger::GetImportErrors() const {
    return importErrors;
}

// Update error display
void Debugger::Update() {
    // Stub implementation
}

// New method to check for redundant declarations in the codebase
void Debugger::CheckForRedundancies() {
    std::cout << "Checking for redundancies..." << std::endl;
    
    // Create a redundancy detector
    RedundancyDetector detector;
    
    // Process the current directory
    detector.ProcessDirectory(".");
    
    // Get redundant symbols
    std::string redundantSymbols = detector.GetRedundantSymbols();
    
    // Parse the redundant symbols
    std::istringstream iss(redundantSymbols);
    std::string line;
    
    // Simplified implementation that doesn't rely on pair.first and pair.second
    while (std::getline(iss, line)) {
        // In a real implementation, we would parse the line and log redundancies
        // For now, just log a generic redundancy
        LogRedundancy("unknown.cpp", line, "unknown", 0);
    }
}
