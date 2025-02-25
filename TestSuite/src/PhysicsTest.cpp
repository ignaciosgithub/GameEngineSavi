#include "../include/Test.h"
#include "../../PhysicsSystem.h"
#include "../../RigidBody.h"
#include "../../Vector3.h"
#include "../../GameObject.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>

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
    // Helper function to convert Vector3 to string
    std::string VecToString(const Vector3& v) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(4) << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return ss.str();
    }
    
    void TestGravity() {
        LogResult("Test", "Gravity Effect");
        
        // Create physics system
        PhysicsSystem physics;
        physics.SetGravity(-9.81f);
        
        // Create game object with rigid body
        auto gameObject = std::unique_ptr<GameObject>(new GameObject(
            "TestObject", 
            Vector3(0, 0, 0), 
            Vector3(0, 0, 0), 
            Vector3(1, 1, 1), 
            std::vector<PointLight>()
        ));
        
        // Create rigid body
        auto body = std::shared_ptr<RigidBody>(new RigidBody());
        body->SetMass(1.0f);
        body->EnableGravity(true);
        body->SetVelocity(Vector3(0, 0, 0));
        
        // Add rigid body to game object
        gameObject->AddComponent(body);
        
        // Add body to physics system
        physics.AddBody(body.get());
        
        // Simulate for 1 second (60 frames at 60 FPS)
        float deltaTime = 1.0f / 60.0f;
        
        for (int i = 0; i < 60; i++) {
            // Update physics
            physics.Update(deltaTime);
            
            // Output velocity every 10 frames
            if (i % 10 == 0) {
                Vector3 vel = body->GetVelocity();
                LogResult("Time " + std::to_string(i * deltaTime) + "s Velocity", VecToString(vel));
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
        auto gameObject = std::unique_ptr<GameObject>(new GameObject(
            "TestObject", 
            Vector3(0, 0, 0), 
            Vector3(0, 0, 0), 
            Vector3(1, 1, 1), 
            std::vector<PointLight>()
        ));
        
        // Create rigid body
        auto body = std::shared_ptr<RigidBody>(new RigidBody());
        body->SetMass(1.0f);
        body->EnableGravity(false);
        body->SetVelocity(Vector3(1.0f, 2.0f, 3.0f));
        
        // Add rigid body to game object
        gameObject->AddComponent(body);
        
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
        auto gameObjectA = std::unique_ptr<GameObject>(new GameObject(
            "TestObjectA", 
            Vector3(0, 2.0f, 0), 
            Vector3(0, 0, 0), 
            Vector3(1, 1, 1), 
            std::vector<PointLight>()
        ));
        
        // Create second game object with rigid body
        auto gameObjectB = std::unique_ptr<GameObject>(new GameObject(
            "TestObjectB", 
            Vector3(0, 0, 0), 
            Vector3(0, 0, 0), 
            Vector3(1, 1, 1), 
            std::vector<PointLight>()
        ));
        
        // Create rigid bodies
        auto bodyA = std::shared_ptr<RigidBody>(new RigidBody());
        bodyA->SetMass(1.0f);
        bodyA->EnableGravity(false);
        bodyA->SetVelocity(Vector3(0, -1.0f, 0));
        
        auto bodyB = std::shared_ptr<RigidBody>(new RigidBody());
        bodyB->SetMass(1.0f);
        bodyB->EnableGravity(false);
        bodyB->SetVelocity(Vector3(0, 0, 0));
        
        // Add rigid bodies to game objects
        gameObjectA->AddComponent(bodyA);
        gameObjectB->AddComponent(bodyB);
        
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
                LogResult("Time " + std::to_string(i * deltaTime) + "s Body A", VecToString(gameObjectA->position));
                LogResult("Time " + std::to_string(i * deltaTime) + "s Body B", VecToString(gameObjectB->position));
            }
            
            // Check for collision
            if (gameObjectA->position.distance(gameObjectB->position) < 1.0f) {
                collisionDetected = true;
                LogResult("Collision Detected", "Time: " + std::to_string(i * deltaTime) + "s");
                break;
            }
        }
        
        // Verify collision detection is working
        LogResult("Collision Test", collisionDetected ? "PASSED" : "FAILED");
    }
};
