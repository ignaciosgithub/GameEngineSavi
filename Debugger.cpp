#include "Debugger.h"
#include <iostream>
#include <algorithm>

// Initialize static members
Debugger* Debugger::instance = nullptr;
std::vector<Debugger::ScriptError> Debugger::errors;
std::vector<Debugger::RedundancyInfo> Debugger::redundancies;
std::vector<Debugger::ImportError> Debugger::importErrors;

// Get the singleton instance
Debugger& Debugger::GetInstance() {
    if (instance == nullptr) {
        instance = new Debugger();
    }
    return *instance;
}

// Log a script error with detailed information
void Debugger::LogError(const std::string& message, const std::string& scriptName,
                       const std::string& functionName, int lineNumber) {
    errors.push_back(ScriptError(message, scriptName, functionName, lineNumber));
    std::cout << "[ERROR] " << message << " in " << scriptName << "::" << functionName << " at line " << lineNumber << std::endl;
}

// Log a simple error message
void Debugger::LogError(const std::string& message) {
    errors.push_back(ScriptError(message, "", "", -1));
    std::cout << "[ERROR] " << message << std::endl;
}

// Method to log redundant declarations
void Debugger::LogRedundancy(const std::string& fileName, const std::string& symbolName,
                           const std::string& type, int lineNumber) {
    redundancies.push_back(RedundancyInfo(fileName, symbolName, type, lineNumber));
    std::cout << "[WARNING] Redundant " << type << " '" << symbolName << "' in " << fileName << " at line " << lineNumber << std::endl;
}

// Method to log import errors
void Debugger::LogImportError(const std::string& message, const std::string& filePath,
                             const std::string& importType) {
    importErrors.push_back(ImportError(message, filePath, importType));
    std::cout << "[IMPORT ERROR] " << message << " for " << importType << " at " << filePath << std::endl;
}

// Clear all logged errors
void Debugger::ClearErrors() {
    errors.clear();
}

// Get all logged errors
const std::vector<Debugger::ScriptError>& Debugger::GetErrors() {
    return errors;
}

// Get all redundant declarations
const std::vector<Debugger::RedundancyInfo>& Debugger::GetRedundancies() {
    return redundancies;
}

// Get all import errors
const std::vector<Debugger::ImportError>& Debugger::GetImportErrors() {
    return importErrors;
}

// Check if there are any errors
bool Debugger::HasErrors() {
    return !errors.empty();
}

// Check if there are any redundant declarations
bool Debugger::HasRedundancies() {
    return !redundancies.empty();
}

// Check if there are any import errors
bool Debugger::HasImportErrors() {
    return !importErrors.empty();
}

// Print all errors to the console
void Debugger::PrintErrors() {
    if (errors.empty()) {
        std::cout << "No errors to report." << std::endl;
        return;
    }

    std::cout << "=== ERROR REPORT ===" << std::endl;
    for (const auto& error : errors) {
        std::cout << "Error: " << error.message;
        if (!error.scriptName.empty()) {
            std::cout << " in " << error.scriptName;
            if (!error.functionName.empty()) {
                std::cout << "::" << error.functionName;
            }
            if (error.lineNumber >= 0) {
                std::cout << " at line " << error.lineNumber;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "===================" << std::endl;
}

// Print all redundant declarations to the console
void Debugger::PrintRedundancies() {
    if (redundancies.empty()) {
        std::cout << "No redundant declarations to report." << std::endl;
        return;
    }

    std::cout << "=== REDUNDANT DECLARATIONS REPORT ===" << std::endl;
    for (const auto& decl : redundancies) {
        std::cout << "Redundant " << decl.type << " '" << decl.symbolName << "' in " << decl.fileName;
        if (decl.lineNumber >= 0) {
            std::cout << " at line " << decl.lineNumber;
        }
        std::cout << std::endl;
    }
    std::cout << "=====================================" << std::endl;
}

// Print all import errors to the console
void Debugger::PrintImportErrors() {
    if (importErrors.empty()) {
        std::cout << "No import errors to report." << std::endl;
        return;
    }

    std::cout << "=== IMPORT ERROR REPORT ===" << std::endl;
    for (const auto& error : importErrors) {
        std::cout << "Import Error: " << error.message << " for " << error.importType << " at " << error.filePath << std::endl;
    }
    std::cout << "==========================" << std::endl;
}

// Constructor
Debugger::Debugger() : showErrorPanel(false), showRedundancyPanel(false), showImportErrorPanel(false) {
    // Initialize any resources needed
}

// Update error display
void Debugger::Update() {
    // Implementation will be added later
}

// Check for redundancies in the codebase
void Debugger::CheckForRedundancies() {
    // Implementation will be added later
}

// Clear all logged redundancies
void Debugger::ClearRedundancies() {
    redundancies.clear();
}

// Clear all logged import errors
void Debugger::ClearImportErrors() {
    importErrors.clear();
}
