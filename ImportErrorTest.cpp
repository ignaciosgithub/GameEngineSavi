#include "Debugger.h"
#include <iostream>
#include <stdexcept>

int main() {
    std::cout << "Testing import error handling..." << std::endl;
    
    // Enable import error panel
    Debugger::GetInstance().ShowImportErrorPanel(true);
    
    // Manually test import error handling
    std::cout << "\nTesting manual import error handling:" << std::endl;
    
    // Test with a valid file path
    bool validResult = Debugger::GetInstance().TryImport([&]() -> bool {
        std::cout << "Processing valid file..." << std::endl;
        return true;
    }, "valid/path/file.txt", "test");
    std::cout << "Valid import result: " << (validResult ? "Success" : "Failure") << std::endl;
    
    // Test with an invalid file path
    bool invalidResult = Debugger::GetInstance().TryImport([&]() -> bool {
        std::cout << "Processing invalid file..." << std::endl;
        throw std::runtime_error("Could not open file");
        return false;
    }, "invalid/path/nonexistent.txt", "test");
    std::cout << "Invalid import result: " << (invalidResult ? "Success" : "Failure") << std::endl;
    
    // Test with a different error type
    bool formatErrorResult = Debugger::GetInstance().TryImport([&]() -> bool {
        std::cout << "Processing file with format error..." << std::endl;
        throw std::runtime_error("Invalid file format");
        return false;
    }, "invalid/format/file.txt", "test");
    std::cout << "Format error result: " << (formatErrorResult ? "Success" : "Failure") << std::endl;
    
    // Display import errors
    std::cout << "\nImport errors:" << std::endl;
    const auto& importErrors = Debugger::GetInstance().GetImportErrors();
    std::cout << "Total import errors: " << importErrors.size() << std::endl;
    
    // Force update to display errors
    Debugger::GetInstance().Update();
    
    return 0;
}
