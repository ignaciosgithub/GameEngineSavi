#include <vector>
#include <memory>
#include <iostream>
#include "../include/Test.h"
#include "Vector3Test.cpp"
#include "PhysicsTest.cpp"

// C++11 implementation of make_unique (since we're targeting C++11)
namespace std {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

int main() {
    std::cout << "GameEngineSavi Test Suite" << std::endl;
    std::cout << "=======================" << std::endl;
    
    // Create vector of tests
    std::vector<std::unique_ptr<Test>> tests;
    
    // Add tests to vector
    tests.push_back(std::make_unique<Vector3Test>());
    tests.push_back(std::make_unique<PhysicsTest>());
    
    // Run all tests
    for (const auto& test : tests) {
        test->Run();
    }
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}
