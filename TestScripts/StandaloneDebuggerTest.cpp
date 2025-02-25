#include "../Debugger.h"
#include <iostream>
#include <vector>
#include <stdexcept>

// Simple function that will throw an exception
void ThrowException() {
    throw std::runtime_error("Test exception");
}

// Function that will cause an out of bounds access
void OutOfBoundsTest() {
    std::vector<int> v;
    v.at(10); // This will throw std::out_of_range
}

int main() {
    std::cout << "Standalone Debugger Test Program" << std::endl;
    std::cout << "===============================" << std::endl;
    
    // Initialize the debugger
    auto& debugger = Debugger::GetInstance();
    
    std::cout << "\nTesting standard exception:" << std::endl;
    bool result1 = debugger.TryExecute([&]() {
        ThrowException();
    }, "TestScript", "ThrowException");
    std::cout << "Exception caught: " << (result1 ? "No" : "Yes") << std::endl;
    
    std::cout << "\nTesting out of bounds exception:" << std::endl;
    bool result2 = debugger.TryExecute([&]() {
        OutOfBoundsTest();
    }, "TestScript", "OutOfBoundsTest");
    std::cout << "Exception caught: " << (result2 ? "No" : "Yes") << std::endl;
    
    // Test custom error logging
    debugger.LogError("Custom error message", "CustomScript", "CustomFunction", 42);
    
    // Display all errors
    std::cout << "\nAll errors logged:" << std::endl;
    const auto& errors = debugger.GetErrors();
    for (size_t i = 0; i < errors.size(); i++) {
        const auto& error = errors[i];
        std::cout << i+1 << ". Error in " << error.scriptName << "::" << error.functionName;
        if (error.lineNumber >= 0) {
            std::cout << " (line " << error.lineNumber << ")";
        }
        std::cout << ": " << error.message << std::endl;
    }
    
    std::cout << "\nTest completed successfully!" << std::endl;
    return 0;
}
