#include <iostream>
#include "GameObject.h"
#include "RigidBody.h"
#include "Vector3.h"

// Simple physics update function for demo purposes
void UpdatePhysics(RigidBody* rb, float deltaTime) {
    if (rb) {
        rb->Update(deltaTime);
    }
}

int main() {
    std::cout << "Torque Demo" << std::endl;
    
    // Create a cube game object
    GameObject* cube = new GameObject("Cube", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
    
    // Add a rigid body to the cube
    RigidBody* cubeRigidBody = new RigidBody();
    cubeRigidBody->SetGameObject(cube);
    cubeRigidBody->mass = 1.0f;
    cubeRigidBody->useGravity = false; // Disable gravity for this demo
    cubeRigidBody->drag = 0.0f;        // No linear drag for this demo
    cubeRigidBody->angularDrag = 0.0f; // No angular drag for this demo
    
    // Create a second cube game object
    GameObject* cube2 = new GameObject("Cube2", Vector3(3, 0, 0), Vector3(0, 0, 0), Vector3(2, 0.5, 0.5));
    
    // Add a rigid body to the second cube
    RigidBody* cube2RigidBody = new RigidBody();
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
        UpdatePhysics(cubeRigidBody, deltaTime);
        UpdatePhysics(cube2RigidBody, deltaTime);
        
        // Print cube positions and rotations
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
        
        // Increment time
        totalTime += deltaTime;
    }
    
    // Clean up
    delete cube;
    delete cubeRigidBody;
    delete cube2;
    delete cube2RigidBody;
    
    std::cout << "Torque Demo completed" << std::endl;
    
    return 0;
}
