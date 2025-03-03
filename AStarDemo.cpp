#include <iostream>
#include "NavMesh.h"
#include "NavMeshManager.h"
#include "AIEntity.h"
#include "Vector3.h"
#include <vector>
#include <chrono>
#include <thread>

int main() {
    std::cout << "A* Pathfinding Demo" << std::endl;
    
    // Create a nav mesh
    NavMesh navMesh;
    
    // Add nodes to the nav mesh (create a simple grid)
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            navMesh.AddNode(Vector3(x, 0, z));
        }
    }
    
    // Connect nodes (create edges)
    for (int x = 0; x < 10; x++) {
        for (int z = 0; z < 10; z++) {
            // Connect to neighbors
            if (x > 0) navMesh.ConnectNodes(Vector3(x, 0, z), Vector3(x-1, 0, z));
            if (x < 9) navMesh.ConnectNodes(Vector3(x, 0, z), Vector3(x+1, 0, z));
            if (z > 0) navMesh.ConnectNodes(Vector3(x, 0, z), Vector3(x, 0, z-1));
            if (z < 9) navMesh.ConnectNodes(Vector3(x, 0, z), Vector3(x, 0, z+1));
        }
    }
    
    // Add an obstacle (block some connections)
    for (int x = 2; x <= 7; x++) {
        for (int z = 2; z <= 7; z++) {
            if (x == 2 || x == 7 || z == 2 || z == 7) {
                // Create obstacle boundary
                navMesh.RemoveNode(Vector3(x, 0, z));
            }
        }
    }
    
    // Initialize NavMeshManager
    NavMeshManager navMeshManager;
    navMeshManager.SetNavMesh(&navMesh);
    
    // Create AI entity
    AIEntity aiEntity;
    aiEntity.SetPosition(Vector3(1, 0, 1));
    aiEntity.SetMaxAngleDiff(45.0f);  // 45 degrees max angle difference
    aiEntity.SetMaxDistance(1.5f);    // Can climb up to 1.5 units
    
    // Set target position
    Vector3 targetPosition(8, 0, 8);
    
    // Find path
    std::cout << "Finding path from " << aiEntity.GetPosition().x << "," 
              << aiEntity.GetPosition().y << "," << aiEntity.GetPosition().z 
              << " to " << targetPosition.x << "," << targetPosition.y 
              << "," << targetPosition.z << std::endl;
    
    std::vector<Vector3> path = navMeshManager.FindPath(aiEntity.GetPosition(), targetPosition, aiEntity);
    
    if (path.empty()) {
        std::cout << "No path found!" << std::endl;
    } else {
        std::cout << "Path found with " << path.size() << " nodes:" << std::endl;
        
        // Print path
        for (const auto& node : path) {
            std::cout << "  -> " << node.x << "," << node.y << "," << node.z << std::endl;
        }
        
        // Simulate entity following path
        std::cout << "\nSimulating entity movement:" << std::endl;
        for (const auto& node : path) {
            // Move entity to next node
            aiEntity.SetPosition(node);
            std::cout << "Entity at position: " << node.x << "," << node.y << "," << node.z << std::endl;
            
            // Simulate time passing
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        std::cout << "Entity reached target position!" << std::endl;
    }
    
    return 0;
}
