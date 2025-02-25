#ifndef TEST_H
#define TEST_H

#include <string>
#include <iostream>
#include <chrono>
#include <iomanip>

class Test {
protected:
    std::string name;
    
    void LogTestStart() {
        std::cout << "\n=== " << name << " Test Start ===\n" << std::endl;
    }
    
    void LogTestEnd() {
        std::cout << "\n=== " << name << " Test Complete ===\n" << std::endl;
    }
    
    void LogResult(const std::string& component, const std::string& value) {
        std::cout << std::setw(20) << component << ": " << value << std::endl;
    }

public:
    Test(const std::string& testName) : name(testName) {}
    virtual ~Test() = default;
    virtual void Run() = 0;
};

#endif // TEST_H
