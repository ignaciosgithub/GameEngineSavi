#ifndef ERROR_TEST_SCRIPT_H
#define ERROR_TEST_SCRIPT_H

#include "../MonoBehaviourLike.h"
#include "../Vector3.h"
#include <iostream>
#include <vector>
#include <memory>

// Test script that intentionally causes different types of errors
class ErrorTestScript : public MonoBehaviourLike {
private:
    int* nullPointer = nullptr;
    std::vector<int> emptyVector;
    
public:
    // Test null pointer dereference
    void TestNullPointerError() {
        std::cout << "Testing null pointer error..." << std::endl;
        *nullPointer = 42; // This will cause a null pointer dereference
    }
    
    // Test out of bounds access
    void TestOutOfBoundsError() {
        std::cout << "Testing out of bounds error..." << std::endl;
        emptyVector.at(10); // This will throw std::out_of_range
    }
    
    // Test division by zero
    void TestDivisionByZeroError() {
        std::cout << "Testing division by zero error..." << std::endl;
        int a = 10;
        int b = 0;
        int c = a / b; // This will cause a division by zero
        std::cout << "Result: " << c << std::endl; // This line will not be reached
    }
    
    // Test custom error
    void TestCustomError() {
        std::cout << "Testing custom error..." << std::endl;
        throw std::runtime_error("This is a custom error message");
    }
    
    // MonoBehaviourLike lifecycle methods
    void Awake() override {
        std::cout << "ErrorTestScript::Awake()" << std::endl;
    }
    
    void Start() override {
        std::cout << "ErrorTestScript::Start()" << std::endl;
        // Uncomment one of these to test different errors during Start
        // TestNullPointerError();
        // TestOutOfBoundsError();
        // TestDivisionByZeroError();
        // TestCustomError();
    }
    
    void Update() override {
        std::cout << "ErrorTestScript::Update()" << std::endl;
        // Uncomment one of these to test different errors during Update
        // TestNullPointerError();
        // TestOutOfBoundsError();
        // TestDivisionByZeroError();
        // TestCustomError();
    }
    
    void OnDestroy() override {
        std::cout << "ErrorTestScript::OnDestroy()" << std::endl;
        // Uncomment one of these to test different errors during OnDestroy
        // TestNullPointerError();
        // TestOutOfBoundsError();
        // TestDivisionByZeroError();
        // TestCustomError();
    }
};

#endif // ERROR_TEST_SCRIPT_H
