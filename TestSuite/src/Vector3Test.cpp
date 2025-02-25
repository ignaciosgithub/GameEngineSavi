#include "../include/Test.h"
#include "../../Vector3.h"
#include <string>
#include <sstream>
#include <iomanip>

class Vector3Test : public Test {
public:
    Vector3Test() : Test("Vector3") {}
    
    void Run() override {
        LogTestStart();
        
        TestConstruction();
        TestAddition();
        TestSubtraction();
        TestMultiplication();
        TestDivision();
        TestNormalization();
        TestDistance();
        TestDotProduct();
        TestCrossProduct();
        
        LogTestEnd();
    }
    
private:
    // Helper function to convert Vector3 to string
    std::string VecToString(const Vector3& v) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(4) << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return ss.str();
    }
    
    void TestConstruction() {
        Vector3 v1;
        Vector3 v2(1.0f, 2.0f, 3.0f);
        
        LogResult("Default Constructor", VecToString(v1));
        LogResult("Parameterized Constructor", VecToString(v2));
    }
    
    void TestAddition() {
        Vector3 v1(1.0f, 2.0f, 3.0f);
        Vector3 v2(4.0f, 5.0f, 6.0f);
        Vector3 sum = v1 + v2;
        
        LogResult("v1", VecToString(v1));
        LogResult("v2", VecToString(v2));
        LogResult("v1 + v2", VecToString(sum));
    }
    
    void TestSubtraction() {
        Vector3 v1(1.0f, 2.0f, 3.0f);
        Vector3 v2(4.0f, 5.0f, 6.0f);
        Vector3 diff = v1 - v2;
        
        LogResult("v1", VecToString(v1));
        LogResult("v2", VecToString(v2));
        LogResult("v1 - v2", VecToString(diff));
    }
    
    void TestMultiplication() {
        Vector3 v1(1.0f, 2.0f, 3.0f);
        float scalar = 2.0f;
        Vector3 product = v1 * scalar;
        
        LogResult("v1", VecToString(v1));
        LogResult("scalar", std::to_string(scalar));
        LogResult("v1 * scalar", VecToString(product));
    }
    
    void TestDivision() {
        Vector3 v1(2.0f, 4.0f, 6.0f);
        float scalar = 2.0f;
        Vector3 quotient = v1 / scalar;
        
        LogResult("v1", VecToString(v1));
        LogResult("scalar", std::to_string(scalar));
        LogResult("v1 / scalar", VecToString(quotient));
    }
    
    void TestNormalization() {
        Vector3 v1(3.0f, 4.0f, 0.0f);
        float originalMagnitude = v1.magnitude();
        Vector3 normalized = v1.normalized();
        float normalizedMagnitude = normalized.magnitude();
        
        LogResult("Original Vector", VecToString(v1));
        LogResult("Original Magnitude", std::to_string(originalMagnitude));
        LogResult("Normalized Vector", VecToString(normalized));
        LogResult("Normalized Magnitude", std::to_string(normalizedMagnitude));
    }
    
    void TestDistance() {
        Vector3 v1(1.0f, 2.0f, 3.0f);
        Vector3 v2(4.0f, 6.0f, 8.0f);
        float distance = v1.distance(v2);
        
        LogResult("v1", VecToString(v1));
        LogResult("v2", VecToString(v2));
        LogResult("Distance", std::to_string(distance));
    }
    
    void TestDotProduct() {
        Vector3 v1(1.0f, 2.0f, 3.0f);
        Vector3 v2(4.0f, 5.0f, 6.0f);
        float dotProduct = v1.dot(v2);
        
        LogResult("v1", VecToString(v1));
        LogResult("v2", VecToString(v2));
        LogResult("Dot Product", std::to_string(dotProduct));
    }
    
    void TestCrossProduct() {
        Vector3 v1(1.0f, 0.0f, 0.0f);
        Vector3 v2(0.0f, 1.0f, 0.0f);
        Vector3 crossProduct = v1.cross(v2);
        
        LogResult("v1", VecToString(v1));
        LogResult("v2", VecToString(v2));
        LogResult("Cross Product", VecToString(crossProduct));
    }
};
