#include "PhysicsSystem.h"
#include <iostream>

PhysicsSystem::PhysicsSystem() {
    std::cout << "PhysicsSystem initialized" << std::endl;
}

PhysicsSystem::~PhysicsSystem() {
    std::cout << "PhysicsSystem destroyed" << std::endl;
}

void PhysicsSystem::Update(float deltaTime) {
    // Stub implementation
}

void PhysicsSystem::SetGravity(float gravity) {
    // Stub implementation
}

float PhysicsSystem::GetGravity() const {
    return -9.81f; // Stub implementation
}
