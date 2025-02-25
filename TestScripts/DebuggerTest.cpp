#include "../Scene.h"
#include "../GameObject.h"
#include "../MonoBehaviourLike.h"
#include "../Debugger.h"
#include "ErrorTestScript.h"
#include <iostream>
#include <memory>

int main() {
    std::cout << "Debugger Test Program" << std::endl;
    std::cout << "====================" << std::endl;
    
    // Create a scene
    Scene scene;
    
    // Create a game object with the error test script
    auto gameObject = std::unique_ptr<GameObject>(new GameObject(
        "ErrorTestObject", 
        Vector3(0, 0, 0), 
        Vector3(0, 0, 0), 
        Vector3(1, 1, 1), 
        std::vector<PointLight>()
    ));
    
    // Add the error test script to the game object
    auto errorScript = std::shared_ptr<ErrorTestScript>(new ErrorTestScript());
    gameObject->AddComponent(errorScript);
    
    // Add the game object to the scene
    scene.AddGameObject(std::move(gameObject));
    
    // Initialize the debugger
    auto& debugger = Debugger::GetInstance();
    
    std::cout << "\nTesting null pointer error:" << std::endl;
    debugger.TryExecute([&]() {
        errorScript->TestNullPointerError();
    }, "ErrorTestObject", "TestNullPointerError");
    
    std::cout << "\nTesting out of bounds error:" << std::endl;
    debugger.TryExecute([&]() {
        errorScript->TestOutOfBoundsError();
    }, "ErrorTestObject", "TestOutOfBoundsError");
    
    std::cout << "\nTesting division by zero error:" << std::endl;
    debugger.TryExecute([&]() {
        errorScript->TestDivisionByZeroError();
    }, "ErrorTestObject", "TestDivisionByZeroError");
    
    std::cout << "\nTesting custom error:" << std::endl;
    debugger.TryExecute([&]() {
        errorScript->TestCustomError();
    }, "ErrorTestObject", "TestCustomError");
    
    // Display all errors
    std::cout << "\nAll errors logged:" << std::endl;
    for (const auto& error : debugger.GetErrors()) {
        std::cout << "Error in " << error.scriptName << "::" << error.functionName;
        if (error.lineNumber >= 0) {
            std::cout << " (line " << error.lineNumber << ")";
        }
        std::cout << ": " << error.message << std::endl;
    }
    
    std::cout << "\nTest completed successfully!" << std::endl;
    return 0;
}
