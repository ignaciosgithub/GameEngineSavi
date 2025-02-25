#include "Debugger.h"
#include <iostream>

Debugger* Debugger::instance = nullptr;

Debugger::Debugger() : showErrorPanel(false) {
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

void Debugger::ClearErrors() {
    errors.clear();
    Update();
}

const std::vector<Debugger::ScriptError>& Debugger::GetErrors() const {
    return errors;
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
}
