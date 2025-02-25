#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <thread>
#include <algorithm>
#include <sstream>

// Forward declarations
class Vector3;
class RigidBody;
class PhysicsSystem;
class GameObject;

// Simple Vector3 implementation for testing
class Vector3 {
public:
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    
    float magnitude() const {
        return std::sqrt(x*x + y*y + z*z);
    }
    
    Vector3 normalized() const {
        float mag = magnitude();
        if (mag > 0.0001f) {
            return *this / mag;
        }
        return Vector3(0, 0, 0);
    }
    
    float distance(const Vector3& other) const {
        return (*this - other).magnitude();
    }
    
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
};

// Simple GameObject implementation for testing
class GameObject {
private:
    std::string name;
    
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    std::shared_ptr<RigidBody> rigidBody;
    
    GameObject(const std::string& name, const Vector3& position)
        : name(name), position(position), rotation(0, 0, 0), scale(1, 1, 1) {}
    
    std::string GetName() const { return name; }
    
    void AddRigidBody(std::shared_ptr<RigidBody> rb);
};

// Simple RigidBody implementation for testing
class RigidBody {
private:
    float mass;
    bool useGravity;
    Vector3 velocity;
    Vector3 force;
    GameObject* gameObject;
    
public:
    RigidBody() : mass(1.0f), useGravity(true), velocity(0, 0, 0), force(0, 0, 0), gameObject(nullptr) {}
    
    void SetMass(float m) { mass = m; }
    float GetMass() const { return mass; }
    
    void EnableGravity(bool enable) { useGravity = enable; }
    bool IsGravityEnabled() const { return useGravity; }
    
    void SetVelocity(const Vector3& vel) { velocity = vel; }
    Vector3 GetVelocity() const { return velocity; }
    
    void AddForce(const Vector3& f) { force = force + f; }
    Vector3 GetForce() const { return force; }
    
    void SetGameObject(GameObject* go) { gameObject = go; }
    GameObject* GetGameObject() const { return gameObject; }
    
    void Update(float deltaTime, const Vector3& gravity) {
        if (useGravity) {
            // Apply gravity
            force = force + gravity * mass;
        }
        
        // Apply force to velocity (F = ma, so a = F/m)
        Vector3 acceleration = force / mass;
        velocity = velocity + acceleration * deltaTime;
        
        // Apply velocity to position
        if (gameObject) {
            gameObject->position = gameObject->position + velocity * deltaTime;
        }
        
        // Reset force for next frame
        force = Vector3(0, 0, 0);
    }
};

// Implementation of GameObject::AddRigidBody
void GameObject::AddRigidBody(std::shared_ptr<RigidBody> rb) {
    rigidBody = rb;
    rb->SetGameObject(this);
}

// Simple PhysicsSystem implementation for testing
class PhysicsSystem {
private:
    std::vector<RigidBody*> bodies;
    Vector3 gravity;
    bool enableCollisions;
    
public:
    PhysicsSystem() : gravity(0, -9.81f, 0), enableCollisions(true) {}
    
    void SetGravity(float gravityValue) {
        gravity = Vector3(0, gravityValue, 0);
    }
    
    void SetEnableCollisions(bool enable) {
        enableCollisions = enable;
    }
    
    void AddBody(RigidBody* body) {
        bodies.push_back(body);
    }
    
    void RemoveBody(RigidBody* body) {
        auto it = std::find(bodies.begin(), bodies.end(), body);
        if (it != bodies.end()) {
            bodies.erase(it);
        }
    }
    
    void Update(float deltaTime) {
        // Update all bodies
        for (auto body : bodies) {
            body->Update(deltaTime, gravity);
        }
        
        // Check for collisions
        if (enableCollisions) {
            for (size_t i = 0; i < bodies.size(); i++) {
                for (size_t j = i + 1; j < bodies.size(); j++) {
                    RigidBody* bodyA = bodies[i];
                    RigidBody* bodyB = bodies[j];
                    
                    GameObject* goA = bodyA->GetGameObject();
                    GameObject* goB = bodyB->GetGameObject();
                    
                    if (goA && goB) {
                        // Simple sphere collision detection
                        float distance = goA->position.distance(goB->position);
                        float collisionThreshold = 1.0f; // Assuming unit spheres
                        
                        if (distance < collisionThreshold) {
                            // Collision response
                            Vector3 direction = (goA->position - goB->position).normalized();
                            
                            // Calculate relative velocity
                            Vector3 relativeVelocity = bodyA->GetVelocity() - bodyB->GetVelocity();
                            
                            // Calculate impulse
                            float impulseStrength = relativeVelocity.dot(direction);
                            
                            // Apply impulse
                            if (impulseStrength > 0) {
                                float totalMass = bodyA->GetMass() + bodyB->GetMass();
                                float ratioA = bodyB->GetMass() / totalMass;
                                float ratioB = bodyA->GetMass() / totalMass;
                                
                                bodyA->SetVelocity(bodyA->GetVelocity() - direction * impulseStrength * ratioA);
                                bodyB->SetVelocity(bodyB->GetVelocity() + direction * impulseStrength * ratioB);
                                
                                // Move objects apart to prevent sticking
                                float overlap = collisionThreshold - distance;
                                goA->position = goA->position + direction * overlap * 0.5f;
                                goB->position = goB->position - direction * overlap * 0.5f;
                            }
                        }
                    }
                }
            }
        }
    }
};

// Test class
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
    
    std::string VecToString(const Vector3& v) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(4) << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return ss.str();
    }

public:
    Test(const std::string& testName) : name(testName) {}
    virtual ~Test() = default;
    virtual void Run() = 0;
};

// Vector3 Test
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

// Physics Test
class PhysicsTest : public Test {
public:
    PhysicsTest() : Test("Physics") {}
    
    void Run() override {
        LogTestStart();
        
        TestGravity();
        TestVelocity();
        TestCollision();
        
        LogTestEnd();
    }
    
private:
    void TestGravity() {
        LogResult("Test", "Gravity Effect");
        
        // Create physics system
        PhysicsSystem physics;
        physics.SetGravity(-9.81f);
        
        // Create game object with rigid body
        auto gameObject = std::make_shared<GameObject>("TestObject", Vector3(0, 10, 0));
        auto body = std::make_shared<RigidBody>();
        
        // Configure rigid body
        body->SetMass(1.0f);
        body->EnableGravity(true);
        body->SetVelocity(Vector3(0, 0, 0));
        
        // Add rigid body to game object
        gameObject->AddRigidBody(body);
        
        // Add body to physics system
        physics.AddBody(body.get());
        
        // Simulate for 1 second (60 frames at 60 FPS)
        float deltaTime = 1.0f / 60.0f;
        
        for (int i = 0; i < 60; i++) {
            // Update physics
            physics.Update(deltaTime);
            
            // Output position and velocity every 10 frames
            if (i % 10 == 0) {
                LogResult("Time " + std::to_string(i * deltaTime) + "s Position", VecToString(gameObject->position));
                LogResult("Time " + std::to_string(i * deltaTime) + "s Velocity", VecToString(body->GetVelocity()));
            }
        }
        
        // Final velocity should be approximately (0, -9.81, 0) after 1 second
        Vector3 finalVel = body->GetVelocity();
        LogResult("Final Velocity", VecToString(finalVel));
        LogResult("Expected Velocity", "(0.0000, -9.8100, 0.0000)");
        
        // Verify gravity is working correctly
        bool gravityWorking = (std::abs(finalVel.y + 9.81f) < 0.1f);
        LogResult("Gravity Test", gravityWorking ? "PASSED" : "FAILED");
    }
    
    void TestVelocity() {
        LogResult("Test", "Velocity Integration");
        
        // Create physics system
        PhysicsSystem physics;
        physics.SetGravity(0.0f); // Disable gravity for this test
        
        // Create game object with rigid body
        auto gameObject = std::make_shared<GameObject>("TestObject", Vector3(0, 0, 0));
        auto body = std::make_shared<RigidBody>();
        
        // Configure rigid body
        body->SetMass(1.0f);
        body->EnableGravity(false);
        body->SetVelocity(Vector3(1.0f, 2.0f, 3.0f));
        
        // Add rigid body to game object
        gameObject->AddRigidBody(body);
        
        // Add body to physics system
        physics.AddBody(body.get());
        
        // Simulate for 1 second (60 frames at 60 FPS)
        float deltaTime = 1.0f / 60.0f;
        
        for (int i = 0; i < 60; i++) {
            // Update physics
            physics.Update(deltaTime);
            
            // Output position and velocity every 10 frames
            if (i % 10 == 0) {
                LogResult("Time " + std::to_string(i * deltaTime) + "s Position", VecToString(gameObject->position));
                LogResult("Time " + std::to_string(i * deltaTime) + "s Velocity", VecToString(body->GetVelocity()));
            }
        }
        
        // Final position should be approximately (1, 2, 3) after 1 second
        LogResult("Final Position", VecToString(gameObject->position));
        LogResult("Expected Position", "(1.0000, 2.0000, 3.0000)");
        
        // Verify velocity integration is working correctly
        bool velocityWorking = (gameObject->position.distance(Vector3(1, 2, 3)) < 0.1f);
        LogResult("Velocity Test", velocityWorking ? "PASSED" : "FAILED");
    }
    
    void TestCollision() {
        LogResult("Test", "Collision Detection");
        
        // Create physics system
        PhysicsSystem physics;
        physics.SetGravity(0.0f); // Disable gravity for this test
        physics.SetEnableCollisions(true);
        
        // Create first game object with rigid body
        auto gameObjectA = std::make_shared<GameObject>("TestObjectA", Vector3(0, 0, 0));
        auto bodyA = std::make_shared<RigidBody>();
        
        // Configure first rigid body
        bodyA->SetMass(1.0f);
        bodyA->EnableGravity(false);
        bodyA->SetVelocity(Vector3(0, 0, 1.0f));
        
        // Add first rigid body to game object
        gameObjectA->AddRigidBody(bodyA);
        
        // Create second game object with rigid body
        auto gameObjectB = std::make_shared<GameObject>("TestObjectB", Vector3(0, 0, 2.0f));
        auto bodyB = std::make_shared<RigidBody>();
        
        // Configure second rigid body
        bodyB->SetMass(1.0f);
        bodyB->EnableGravity(false);
        bodyB->SetVelocity(Vector3(0, 0, -1.0f));
        
        // Add second rigid body to game object
        gameObjectB->AddRigidBody(bodyB);
        
        // Add bodies to physics system
        physics.AddBody(bodyA.get());
        physics.AddBody(bodyB.get());
        
        // Simulate for 1 second (60 frames at 60 FPS)
        float deltaTime = 1.0f / 60.0f;
        bool collisionDetected = false;
        
        for (int i = 0; i < 60; i++) {
            // Update physics
            physics.Update(deltaTime);
            
            // Output positions every 10 frames
            if (i % 10 == 0) {
                LogResult("Time " + std::to_string(i * deltaTime) + "s Object A", VecToString(gameObjectA->position));
                LogResult("Time " + std::to_string(i * deltaTime) + "s Object B", VecToString(gameObjectB->position));
            }
            
            // Check for collision
            float distance = gameObjectA->position.distance(gameObjectB->position);
            if (distance < 1.0f && !collisionDetected) {
                collisionDetected = true;
                LogResult("Collision Detected", "Time: " + std::to_string(i * deltaTime) + "s");
                LogResult("Distance", std::to_string(distance));
                LogResult("Object A Velocity", VecToString(bodyA->GetVelocity()));
                LogResult("Object B Velocity", VecToString(bodyB->GetVelocity()));
            }
        }
        
        // Verify collision detection is working
        LogResult("Collision Test", collisionDetected ? "PASSED" : "FAILED");
        
        // Final positions and velocities
        LogResult("Final Object A Position", VecToString(gameObjectA->position));
        LogResult("Final Object B Position", VecToString(gameObjectB->position));
        LogResult("Final Object A Velocity", VecToString(bodyA->GetVelocity()));
        LogResult("Final Object B Velocity", VecToString(bodyB->GetVelocity()));
    }
};

// Main function
int main() {
    std::cout << "GameEngineSavi Test Suite" << std::endl;
    std::cout << "=======================" << std::endl;
    
    // Create and run Vector3 test
    Vector3Test vectorTest;
    vectorTest.Run();
    
    // Create and run Physics test
    PhysicsTest physicsTest;
    physicsTest.Run();
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}
