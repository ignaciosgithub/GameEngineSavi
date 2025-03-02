#include <iostream>
#include <vector>
#include "Vector3.h"

// Simple GameObject class for demo purposes
class SimpleGameObject {
public:
    std::string name;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    
    SimpleGameObject(const std::string& n, const Vector3& pos, const Vector3& rot, const Vector3& scl)
        : name(n), position(pos), rotation(rot), scale(scl) {}
    
    std::string GetName() const { return name; }
    Vector3 GetPosition() const { return position; }
    Vector3 GetRotation() const { return rotation; }
    Vector3 GetScale() const { return scale; }
    
    void SetPosition(const Vector3& pos) { position = pos; }
    void SetRotation(const Vector3& rot) { rotation = rot; }
};

// Simple RigidBody class for demo purposes
class SimpleRigidBody {
public:
    float mass;
    bool useGravity;
    bool isKinematic;
    float drag;
    float angularDrag;
    Vector3 velocity;
    Vector3 angularVelocity;
    Vector3 force;
    Vector3 torque;
    Vector3 inertiaTensor;
    SimpleGameObject* gameObject;
    
    SimpleRigidBody()
        : mass(1.0f)
        , useGravity(true)
        , isKinematic(false)
        , drag(0.0f)
        , angularDrag(0.05f)
        , velocity(0, 0, 0)
        , angularVelocity(0, 0, 0)
        , force(0, 0, 0)
        , torque(0, 0, 0)
        , inertiaTensor(1.0f, 1.0f, 1.0f)
        , gameObject(nullptr) {
    }
    
    void SetGameObject(SimpleGameObject* obj) {
        gameObject = obj;
        CalculateInertiaTensor();
    }
    
    void AddForce(const Vector3& newForce) {
        force += newForce;
    }
    
    void AddTorque(const Vector3& newTorque) {
        torque += newTorque;
    }
    
    void AddForceAtPosition(const Vector3& newForce, const Vector3& position) {
        // Add force
        force += newForce;
        
        // Calculate torque
        if (gameObject) {
            Vector3 relativePos = position - gameObject->GetPosition();
            Vector3 resultingTorque = relativePos.cross(newForce);
            torque += resultingTorque;
        }
    }
    
    void AddRelativeTorque(const Vector3& newTorque) {
        // Simple implementation - in a real engine, this would transform from local to world space
        torque += newTorque;
    }
    
    void Update(float deltaTime) {
        if (isKinematic || !gameObject) {
            ClearForces();
            return;
        }
        
        // Apply forces to velocity
        if (mass > 0) {
            velocity += force * (1.0f / mass) * deltaTime;
            
            // Apply drag
            if (drag > 0) {
                float dragFactor = 1.0f - drag * deltaTime;
                dragFactor = std::max(0.0f, dragFactor);
                velocity *= dragFactor;
            }
            
            // Apply angular forces using inertia tensor
            Vector3 angularAcceleration;
            angularAcceleration.x = torque.x / inertiaTensor.x;
            angularAcceleration.y = torque.y / inertiaTensor.y;
            angularAcceleration.z = torque.z / inertiaTensor.z;
            
            angularVelocity += angularAcceleration * deltaTime;
            
            // Apply angular drag
            if (angularDrag > 0) {
                float angularDragFactor = 1.0f - angularDrag * deltaTime;
                angularDragFactor = std::max(0.0f, angularDragFactor);
                angularVelocity *= angularDragFactor;
            }
        }
        
        // Update position based on velocity
        gameObject->SetPosition(gameObject->GetPosition() + velocity * deltaTime);
        
        // Update rotation based on angular velocity
        gameObject->SetRotation(gameObject->GetRotation() + angularVelocity * deltaTime);
        
        // Reset forces and torques
        ClearForces();
    }
    
    void ClearForces() {
        force = Vector3(0, 0, 0);
        torque = Vector3(0, 0, 0);
    }
    
    void CalculateInertiaTensor() {
        // Validate mass
        if (mass <= 0) {
            mass = 1.0f;  // Prevent division by zero
        }

        // Default to unit sphere if no game object
        if (!gameObject) {
            float sphereInertia = (2.0f/5.0f) * mass;
            inertiaTensor = Vector3(sphereInertia, sphereInertia, sphereInertia);
            return;
        }

        // Get object scale, prevent zero dimensions
        Vector3 size = gameObject->GetScale();
        size.x = std::max(0.01f, size.x);
        size.y = std::max(0.01f, size.y);
        size.z = std::max(0.01f, size.z);

        // Box inertia tensor calculation
        float factor = 1.0f / 12.0f;
        float x2 = size.x * size.x;
        float y2 = size.y * size.y;
        float z2 = size.z * size.z;

        inertiaTensor.x = factor * mass * (y2 + z2);
        inertiaTensor.y = factor * mass * (x2 + z2);
        inertiaTensor.z = factor * mass * (x2 + y2);
    }
};

int main() {
    std::cout << "Simple Torque Demo" << std::endl;
    
    // Create a cube game object
    SimpleGameObject* cube = new SimpleGameObject("Cube", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
    
    // Add a rigid body to the cube
    SimpleRigidBody* cubeRigidBody = new SimpleRigidBody();
    cubeRigidBody->SetGameObject(cube);
    cubeRigidBody->mass = 1.0f;
    cubeRigidBody->useGravity = false; // Disable gravity for this demo
    cubeRigidBody->drag = 0.0f;        // No linear drag for this demo
    cubeRigidBody->angularDrag = 0.0f; // No angular drag for this demo
    
    // Create a second cube game object
    SimpleGameObject* cube2 = new SimpleGameObject("Cube2", Vector3(3, 0, 0), Vector3(0, 0, 0), Vector3(2, 0.5, 0.5));
    
    // Add a rigid body to the second cube
    SimpleRigidBody* cube2RigidBody = new SimpleRigidBody();
    cube2RigidBody->SetGameObject(cube2);
    cube2RigidBody->mass = 1.0f;
    cube2RigidBody->useGravity = false; // Disable gravity for this demo
    cube2RigidBody->drag = 0.0f;        // No linear drag for this demo
    cube2RigidBody->angularDrag = 0.0f; // No angular drag for this demo
    
    // Apply force at a point to create torque
    Vector3 force(0, 10, 0);
    Vector3 applicationPoint(0.5, 0, 0); // Apply force at the edge of the cube
    
    std::cout << "Applying force " << force.x << ", " << force.y << ", " << force.z 
              << " at point " << applicationPoint.x << ", " << applicationPoint.y << ", " << applicationPoint.z  
              << " to " << cube->GetName() << std::endl;
    
    cubeRigidBody->AddForceAtPosition(force, applicationPoint);
    
    // Apply relative torque to the second cube
    Vector3 relativeTorque(0, 0, 5);
    
    std::cout << "Applying relative torque " << relativeTorque.x << ", " << relativeTorque.y << ", " << relativeTorque.z 
              << " to " << cube2->GetName() << std::endl;
    
    cube2RigidBody->AddRelativeTorque(relativeTorque);
    
    // Print initial inertia tensor values
    std::cout << "Cube Inertia Tensor: " 
              << cubeRigidBody->inertiaTensor.x << ", " 
              << cubeRigidBody->inertiaTensor.y << ", " 
              << cubeRigidBody->inertiaTensor.z << std::endl;
    
    std::cout << "Cube2 Inertia Tensor: " 
              << cube2RigidBody->inertiaTensor.x << ", " 
              << cube2RigidBody->inertiaTensor.y << ", " 
              << cube2RigidBody->inertiaTensor.z << std::endl;
    
    std::cout << std::endl;
    
    // Simulate for a few seconds
    float totalTime = 0.0f;
    float deltaTime = 1.0f / 60.0f; // 60 FPS
    
    while (totalTime < 5.0f) {
        // Update physics for each rigid body
        cubeRigidBody->Update(deltaTime);
        cube2RigidBody->Update(deltaTime);
        
        // Print cube positions and rotations every 0.5 seconds
        if (static_cast<int>(totalTime * 2) != static_cast<int>((totalTime + deltaTime) * 2)) {
            std::cout << "Time: " << totalTime << "s" << std::endl;
            
            std::cout << cube->GetName() << " Position: " 
                    << cube->GetPosition().x << ", " 
                    << cube->GetPosition().y << ", " 
                    << cube->GetPosition().z << std::endl;
            
            std::cout << cube->GetName() << " Rotation: " 
                    << cube->GetRotation().x << ", " 
                    << cube->GetRotation().y << ", " 
                    << cube->GetRotation().z << std::endl;
            
            std::cout << cube2->GetName() << " Position: " 
                    << cube2->GetPosition().x << ", " 
                    << cube2->GetPosition().y << ", " 
                    << cube2->GetPosition().z << std::endl;
            
            std::cout << cube2->GetName() << " Rotation: " 
                    << cube2->GetRotation().x << ", " 
                    << cube2->GetRotation().y << ", " 
                    << cube2->GetRotation().z << std::endl;
            
            std::cout << std::endl;
        }
        
        // Increment time
        totalTime += deltaTime;
    }
    
    // Clean up
    delete cube;
    delete cubeRigidBody;
    delete cube2;
    delete cube2RigidBody;
    
    std::cout << "Simple Torque Demo completed" << std::endl;
    
    return 0;
}
