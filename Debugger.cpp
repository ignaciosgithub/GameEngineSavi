#include "Debugger.h"
#include "RedundancyDetector.h"
#include <iostream>

Debugger* Debugger::instance = nullptr;

Debugger::Debugger() : showErrorPanel(false), showRedundancyPanel(false), 
                              showImportErrorPanel(false) {
    // Initialize debugger
}

Debugger& Debugger::GetInstance() {
    if (!instance) {
        instance = new Debugger();
    }
    return *instance;
}

void Debugger::LogError(const std::string& message, const std::string& scriptName,
                       const std::string& functionName, int lineNumber) {
    errors.emplace_back(message, scriptName, functionName, lineNumber);
    
    // Always print to console for now
    std::cout << "Script Error in " << scriptName << "::" << functionName;
    if (lineNumber >= 0) {
        std::cout << " (line " << lineNumber << ")";
    }
    std::cout << ": " << message << std::endl;
    
    Update();
}

void Debugger::LogError(const std::string& message) {
    LogError(message, "Unknown", "Unknown", -1);
}

void Debugger::LogRedundancy(const std::string& fileName, const std::string& symbolName,
                           const std::string& type, int lineNumber) {
    redundancies.emplace_back(fileName, symbolName, type, lineNumber);
    
    // Always print to console for now
    std::cout << "Redundancy Warning: " << type << " '" << symbolName << "' in " << fileName;
    if (lineNumber >= 0) {
        std::cout << " (line " << lineNumber << ")";
    }
    std::cout << std::endl;
    
    Update();
}

void Debugger::LogImportError(const std::string& message, const std::string& filePath,
                           const std::string& importType) {
    importErrors.emplace_back(message, filePath, importType);
    
    // Always print to console for now
    std::cout << "Import Error: " << importType << " '" << filePath << "': " << message << std::endl;
    
    Update();
}

void Debugger::ClearErrors() {
    errors.clear();
    Update();
}

void Debugger::ClearRedundancies() {
    redundancies.clear();
    Update();
}

void Debugger::ClearImportErrors() {
    importErrors.clear();
    Update();
}

const std::vector<Debugger::ScriptError>& Debugger::GetErrors() const {
    return errors;
}

const std::vector<Debugger::RedundancyInfo>& Debugger::GetRedundancies() const {
    return redundancies;
}

const std::vector<Debugger::ImportError>& Debugger::GetImportErrors() const {
    return importErrors;
}

void Debugger::Update() {
    // In the future, this will update the GUI error panel
    // For now, we just print errors to the console
    if (showErrorPanel && !errors.empty()) {
        std::cout << "\n=== ERROR PANEL ===\n";
        for (const auto& error : errors) {
            std::cout << error.scriptName << "::" << error.functionName;
            if (error.lineNumber >= 0) {
                std::cout << " (line " << error.lineNumber << ")";
            }
            std::cout << ": " << error.message << std::endl;
        }
        std::cout << "==================\n";
    }
    
    // Display redundancy panel
    if (showRedundancyPanel && !redundancies.empty()) {
        std::cout << "\n=== REDUNDANCY PANEL ===\n";
        for (const auto& redundancy : redundancies) {
            std::cout << redundancy.type << " '" << redundancy.symbolName << "' in " << redundancy.fileName;
            if (redundancy.lineNumber >= 0) {
                std::cout << " (line " << redundancy.lineNumber << ")";
            }
            std::cout << std::endl;
        }
        std::cout << "========================\n";
    }
    
    // Display import error panel
    if (showImportErrorPanel && !importErrors.empty()) {
        std::cout << "\n=== IMPORT ERROR PANEL ===\n";
        for (const auto& error : importErrors) {
            std::cout << error.importType << " '" << error.filePath << "': " 
                      << error.message << std::endl;
        }
        std::cout << "========================\n";
    }
}

void Debugger::CheckForRedundancies() {
    std::cout << "Checking for redundancies in the codebase..." << std::endl;
    
    RedundancyDetector detector;
    detector.ProcessDirectory(".");
    
    auto redundantSymbols = detector.GetRedundantSymbols();
    
    for (const auto& pair : redundantSymbols) {
        std::string symbolName = pair.first;
        std::string type = pair.second[0].type;
        
        for (const auto& info : pair.second) {
            LogRedundancy(info.fileName, symbolName, type, info.lineNumber);
        }
    }
    
    std::cout << "Redundancy check complete. Found " << redundantSymbols.size() << " redundant symbols." << std::endl;
}
