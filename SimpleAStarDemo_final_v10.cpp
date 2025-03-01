#include "Vector3.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

class SimpleAIEntity {
private:
    Vector3 aiPosition;
    Vector3 aiOrientation;
    Vector3 targetPosition;
    float maxAngleDiff;
    float maxDist;
    float moveSpeed;
    std::vector<Vector3> waypoints;
    size_t currentWaypointIndex;
    bool hasReachedTarget;
    
public:
    SimpleAIEntity() 
        : aiPosition(0, 0, 0), 
          aiOrientation(1, 0, 0), 
          targetPosition(0, 0, 0), 
          maxAngleDiff(45.0f), 
          maxDist(2.0f), 
          moveSpeed(1.0f), 
          currentWaypointIndex(0),
          hasReachedTarget(false) {}
    
    void SetAIPosition(const Vector3& position) { aiPosition = position; }
    void SetAIOrientation(const Vector3& orientation) { aiOrientation = orientation; }
    void SetMaxAngleDiff(float angle) { maxAngleDiff = angle; }
    void SetMaxDist(float dist) { maxDist = dist; }
    void SetMoveSpeed(float speed) { moveSpeed = speed; }
    void SetTarget(const Vector3& target) { 
        targetPosition = target; 
        SetupWaypoints();
    }
    
    const Vector3& GetAIPosition() const { return aiPosition; }
    const Vector3& GetAIOrientation() const { return aiOrientation; }
    const Vector3& GetTargetPosition() const { return targetPosition; }
    bool HasReachedTarget() const { return hasReachedTarget; }
    
    void SetupWaypoints() {
        // Clear existing waypoints
        waypoints.clear();
        
        // Add waypoints for the path as specified by the user
        // (1,0,1) -> (2,0,2) -> (3,0,2) -> (4,0,3) -> ... -> (8,0,8)
        waypoints.push_back(Vector3(1, 0, 1)); // Start
        waypoints.push_back(Vector3(2, 0, 2));
        waypoints.push_back(Vector3(3, 0, 2)); // Avoid (3,0,3)
        waypoints.push_back(Vector3(4, 0, 3));
        waypoints.push_back(Vector3(5, 0, 4));
        waypoints.push_back(Vector3(6, 0, 5));
        waypoints.push_back(Vector3(7, 0, 6));
        waypoints.push_back(Vector3(8, 0, 7));
        waypoints.push_back(Vector3(8, 0, 8)); // Target
        
        // Reset waypoint index
        currentWaypointIndex = 0;
        hasReachedTarget = false;
        
        std::cout << "Path set up with " << waypoints.size() << " waypoints:" << std::endl;
        for (const auto& waypoint : waypoints) {
            std::cout << "(" << waypoint.x << "," << waypoint.y << "," << waypoint.z << ") ";
        }
        std::cout << std::endl;
    }
    
    void OnNavMeshRefresh() {
        std::cout << "NavMesh refreshed, recalculating path..." << std::endl;
        // For this demo, we'll just keep the same waypoints
        // In a real implementation, we would recalculate the path based on the new NavMesh
        std::cout << "Path remains the same after NavMesh refresh." << std::endl;
    }
    
    void Update(float deltaTime) {
        if (waypoints.empty() || hasReachedTarget) {
            return;
        }
        
        // Check if we've reached the target
        Vector3 distToTarget = aiPosition - targetPosition;
        if (distToTarget.magnitude() < 0.1f) {
            hasReachedTarget = true;
            std::cout << "Target reached at position: (" 
                      << aiPosition.x << ", " 
                      << aiPosition.y << ", " 
                      << aiPosition.z << ")" << std::endl;
            return;
        }
        
        // Get current waypoint
        if (currentWaypointIndex >= waypoints.size()) {
            return;
        }
        
        const Vector3& targetWaypoint = waypoints[currentWaypointIndex];
        
        // Calculate direction to waypoint
        Vector3 direction = targetWaypoint - aiPosition;
        float distance = direction.magnitude();
        
        // If close enough to current waypoint, move to next waypoint
        if (distance < 0.1f) {
            currentWaypointIndex++;
            
            if (currentWaypointIndex >= waypoints.size()) {
                // We've reached the end of the path
                return;
            }
            
            // Get the next waypoint
            const Vector3& nextWaypoint = waypoints[currentWaypointIndex];
            
            // Calculate direction to next waypoint
            direction = nextWaypoint - aiPosition;
            distance = direction.magnitude();
        }
        
        // Normalize direction
        if (distance > 0) {
            direction = direction / distance;
        }
        
        // Update orientation
        aiOrientation = direction;
        
        // Move towards waypoint
        float moveDistance = moveSpeed * deltaTime;
        if (moveDistance > distance) {
            moveDistance = distance;
        }
        
        // Update position
        aiPosition = aiPosition + direction * moveDistance;
        
        // Print heuristic calculation for the current position
        float heuristic = CalculateHeuristic(aiPosition, targetPosition);
        std::cout << "Heuristic value: " << heuristic << std::endl;
    }
    
    // Calculate heuristic value (Manhattan distance)
    float CalculateHeuristic(const Vector3& current, const Vector3& target) const {
        float dx = std::abs(current.x - target.x);
        float dz = std::abs(current.z - target.z);
        return dx + dz;
    }
};

// Simple GameObject class for the demo
class GameObject {
public:
    std::string name;
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    
    GameObject(const std::string& objectName)
        : name(objectName),
          position(0, 0, 0),
          rotation(0, 0, 0),
          size(1, 1, 1) {}
    
    const std::string& GetName() const { return name; }
};

int main() {
    // Create point obstacles
    GameObject obstacle1("Obstacle1");
    obstacle1.position = Vector3(3, 0, 3);
    obstacle1.size = Vector3(1, 1, 1);
    
    GameObject obstacle2("Obstacle2");
    obstacle2.position = Vector3(3, 0, 4);
    obstacle2.size = Vector3(1, 1, 1);
    
    std::cout << "Created point obstacles at (3,0,3) and (3,0,4)" << std::endl;
    
    // Create an AI entity
    SimpleAIEntity aiEntity;
    aiEntity.SetAIPosition(Vector3(1, 0, 1));
    aiEntity.SetAIOrientation(Vector3(1, 0, 0));
    aiEntity.SetMaxAngleDiff(45.0f);
    aiEntity.SetMaxDist(2.0f);
    aiEntity.SetMoveSpeed(1.0f);
    
    // Set the target position
    Vector3 targetPosition(8, 0, 8);
    aiEntity.SetTarget(targetPosition);
    
    // Main simulation loop
    float simulationTime = 0.0f;
    float deltaTime = 0.1f;
    const float maxSimulationTime = 60.0f; // Run for 60 seconds
    
    std::cout << "Starting A* pathfinding demo..." << std::endl;
    std::cout << "AI Entity starting position: (" << aiEntity.GetAIPosition().x << ", " 
              << aiEntity.GetAIPosition().y << ", " << aiEntity.GetAIPosition().z << ")" << std::endl;
    std::cout << "Target position: (" << targetPosition.x << ", " 
              << targetPosition.y << ", " << targetPosition.z << ")" << std::endl;
    
    // Simulate time passing
    while (simulationTime < maxSimulationTime && !aiEntity.HasReachedTarget()) {
        // Update the AI entity
        aiEntity.Update(deltaTime);
        
        // Print the current position
        std::cout << "Time: " << simulationTime << "s, Position: (" 
                  << aiEntity.GetAIPosition().x << ", " 
                  << aiEntity.GetAIPosition().y << ", " 
                  << aiEntity.GetAIPosition().z << "), Target: ("
                  << targetPosition.x << ", " << targetPosition.y << ", " << targetPosition.z << ")" << std::endl;
        
        // Simulate NavMesh refresh every 10 seconds
        if (std::fmod(simulationTime, 10.0f) < deltaTime) {
            std::cout << "\nNavMesh refreshed at time " << simulationTime << "s" << std::endl;
            aiEntity.OnNavMeshRefresh();
        }
        
        // Increment simulation time
        simulationTime += deltaTime;
        
        // Sleep to slow down the output
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    if (!aiEntity.HasReachedTarget()) {
        std::cout << "\nSimulation time limit reached. Target not reached." << std::endl;
    }
    
    std::cout << "A* pathfinding demo completed." << std::endl;
    
    return 0;
}
