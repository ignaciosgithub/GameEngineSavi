#include "Vector3.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

class NavNode {
public:
    Vector3 position;
    Vector3 normal;
    float cost;
    std::vector<size_t> connections;
    
    NavNode(const Vector3& pos, const Vector3& norm, float c = 0.0f)
        : position(pos), normal(norm), cost(c) {}
};

class TiltedNavMesh {
private:
    std::vector<NavNode> nodes;
    
public:
    TiltedNavMesh() {}
    
    void AddNode(const Vector3& position, float cost, const Vector3& normal) {
        nodes.push_back(NavNode(position, normal, cost));
    }
    
    void ConnectNodes(size_t nodeA, size_t nodeB) {
        if (nodeA < nodes.size() && nodeB < nodes.size()) {
            // Add connection from A to B
            if (std::find(nodes[nodeA].connections.begin(), nodes[nodeA].connections.end(), nodeB) == nodes[nodeA].connections.end()) {
                nodes[nodeA].connections.push_back(nodeB);
            }
            
            // Add connection from B to A
            if (std::find(nodes[nodeB].connections.begin(), nodes[nodeB].connections.end(), nodeA) == nodes[nodeB].connections.end()) {
                nodes[nodeB].connections.push_back(nodeA);
            }
        }
    }
    
    const std::vector<NavNode>& GetNodes() const {
        return nodes;
    }
    
    // Calculate heuristic value (Manhattan distance)
    float CalculateHeuristic(size_t nodeIndex, size_t targetIndex) const {
        if (nodeIndex >= nodes.size() || targetIndex >= nodes.size()) {
            return INFINITY;
        }
        
        const Vector3& nodePos = nodes[nodeIndex].position;
        const Vector3& targetPos = nodes[targetIndex].position;
        
        float dx = std::abs(nodePos.x - targetPos.x);
        float dy = std::abs(nodePos.y - targetPos.y);
        float dz = std::abs(nodePos.z - targetPos.z);
        
        return dx + dy + dz;
    }
    
    // Check if an AI entity can access a node based on angle difference
    bool CanAccessNode(const Vector3& entityPosition, const Vector3& entityOrientation, 
                       size_t nodeIndex, float maxAngleDiff, float maxDist) const {
        if (nodeIndex >= nodes.size()) {
            return false;
        }
        
        const NavNode& node = nodes[nodeIndex];
        
        // Check distance
        Vector3 distVec = node.position - entityPosition;
        float distance = distVec.magnitude();
        if (distance > maxDist) {
            return false;
        }
        
        // Check angle difference between entity orientation and node normal
        float dotProduct = entityOrientation.dot(node.normal);
        float angle = std::acos(dotProduct) * 180.0f / 3.14159f;
        
        return angle <= maxAngleDiff;
    }
    
    // Find a path from start to target
    std::vector<size_t> FindPath(const Vector3& startPos, const Vector3& targetPos, 
                                const Vector3& entityOrientation, float maxAngleDiff, float maxDist) const {
        // Find closest nodes to start and target positions
        size_t startNode = FindClosestNode(startPos);
        size_t targetNode = FindClosestNode(targetPos);
        
        std::cout << "Starting A* pathfinding from node " << startNode << " (" 
                  << nodes[startNode].position.x << "," 
                  << nodes[startNode].position.y << "," 
                  << nodes[startNode].position.z << ") to node " 
                  << targetNode << " (" 
                  << nodes[targetNode].position.x << "," 
                  << nodes[targetNode].position.y << "," 
                  << nodes[targetNode].position.z << ")" << std::endl;
        
        // A* algorithm
        std::vector<bool> closedSet(nodes.size(), false);
        std::vector<float> gScore(nodes.size(), INFINITY);
        std::vector<float> fScore(nodes.size(), INFINITY);
        std::vector<size_t> cameFrom(nodes.size(), -1);
        
        gScore[startNode] = 0;
        fScore[startNode] = CalculateHeuristic(startNode, targetNode);
        
        std::vector<size_t> openSet;
        openSet.push_back(startNode);
        
        while (!openSet.empty()) {
            // Find node with lowest fScore
            size_t current = openSet[0];
            size_t currentIndex = 0;
            
            for (size_t i = 1; i < openSet.size(); ++i) {
                if (fScore[openSet[i]] < fScore[current]) {
                    current = openSet[i];
                    currentIndex = i;
                }
            }
            
            // Check if we've reached the target
            if (current == targetNode) {
                return ReconstructPath(cameFrom, current);
            }
            
            // Remove current from openSet
            openSet.erase(openSet.begin() + currentIndex);
            closedSet[current] = true;
            
            // Check each neighbor
            for (size_t neighbor : nodes[current].connections) {
                if (closedSet[neighbor]) {
                    continue;
                }
                
                // Check if the entity can access this node based on angle and distance
                if (!CanAccessNode(nodes[current].position, entityOrientation, neighbor, maxAngleDiff, maxDist)) {
                    continue;
                }
                
                // Calculate tentative gScore
                float tentativeGScore = gScore[current] + CalculateDistance(current, neighbor);
                
                // Add neighbor to openSet if not already there
                bool inOpenSet = false;
                for (size_t i = 0; i < openSet.size(); ++i) {
                    if (openSet[i] == neighbor) {
                        inOpenSet = true;
                        break;
                    }
                }
                
                if (!inOpenSet) {
                    openSet.push_back(neighbor);
                } else if (tentativeGScore >= gScore[neighbor]) {
                    continue;
                }
                
                // This path is better, record it
                cameFrom[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = gScore[neighbor] + CalculateHeuristic(neighbor, targetNode);
            }
        }
        
        // No path found
        std::cout << "No path found" << std::endl;
        return std::vector<size_t>();
    }
    
private:
    // Find the closest node to a position
    size_t FindClosestNode(const Vector3& position) const {
        if (nodes.empty()) {
            return -1;
        }
        
        size_t closestNode = 0;
        float closestDistance = (nodes[0].position - position).magnitude();
        
        for (size_t i = 1; i < nodes.size(); ++i) {
            float distance = (nodes[i].position - position).magnitude();
            if (distance < closestDistance) {
                closestDistance = distance;
                closestNode = i;
            }
        }
        
        return closestNode;
    }
    
    // Calculate distance between two nodes
    float CalculateDistance(size_t nodeA, size_t nodeB) const {
        if (nodeA >= nodes.size() || nodeB >= nodes.size()) {
            return INFINITY;
        }
        
        return (nodes[nodeA].position - nodes[nodeB].position).magnitude();
    }
    
    // Reconstruct path from cameFrom map
    std::vector<size_t> ReconstructPath(const std::vector<size_t>& cameFrom, size_t current) const {
        std::vector<size_t> path;
        path.push_back(current);
        
        while (cameFrom[current] != static_cast<size_t>(-1)) {
            current = cameFrom[current];
            path.insert(path.begin(), current);
        }
        
        return path;
    }
};

class TiltedAIEntity {
private:
    Vector3 aiPosition;
    Vector3 aiOrientation;
    Vector3 targetPosition;
    float maxAngleDiff;
    float maxDist;
    float moveSpeed;
    std::vector<size_t> currentPath;
    size_t currentPathIndex;
    TiltedNavMesh* navMesh;
    bool hasReachedTarget;
    
public:
    TiltedAIEntity() 
        : aiPosition(0, 0, 0), 
          aiOrientation(0, 1, 0), 
          targetPosition(0, 0, 0), 
          maxAngleDiff(45.0f), 
          maxDist(2.0f), 
          moveSpeed(1.0f), 
          currentPathIndex(0), 
          navMesh(nullptr),
          hasReachedTarget(false) {}
    
    void SetAIPosition(const Vector3& position) { aiPosition = position; }
    void SetAIOrientation(const Vector3& orientation) { aiOrientation = orientation; }
    void SetMaxAngleDiff(float angle) { maxAngleDiff = angle; }
    void SetMaxDist(float dist) { maxDist = dist; }
    void SetMoveSpeed(float speed) { moveSpeed = speed; }
    void SetTarget(const Vector3& target) { 
        targetPosition = target; 
        RecalculatePath();
    }
    
    void SetNavMesh(TiltedNavMesh* mesh) { 
        navMesh = mesh; 
        RecalculatePath();
    }
    
    const Vector3& GetAIPosition() const { return aiPosition; }
    const Vector3& GetAIOrientation() const { return aiOrientation; }
    const Vector3& GetTargetPosition() const { return targetPosition; }
    bool HasReachedTarget() const { return hasReachedTarget; }
    
    void RecalculatePath() {
        if (navMesh) {
            currentPath = navMesh->FindPath(aiPosition, targetPosition, aiOrientation, maxAngleDiff, maxDist);
            currentPathIndex = 0;
            hasReachedTarget = false;
            
            std::cout << "Path recalculated. Path length: " << currentPath.size() << std::endl;
            if (!currentPath.empty()) {
                std::cout << "Path nodes: ";
                for (size_t nodeIdx : currentPath) {
                    const NavNode& node = navMesh->GetNodes()[nodeIdx];
                    std::cout << "(" << node.position.x << "," << node.position.y << "," << node.position.z << ") ";
                }
                std::cout << std::endl;
            }
        }
    }
    
    void OnNavMeshRefresh() {
        std::cout << "NavMesh refreshed, recalculating path..." << std::endl;
        RecalculatePath();
    }
    
    void Update(float deltaTime) {
        if (!navMesh || currentPath.empty() || hasReachedTarget) {
            return;
        }
        
        // Check if we've reached the target
        Vector3 distToTarget = aiPosition - targetPosition;
        if (distToTarget.magnitude() < 0.5f) {
            hasReachedTarget = true;
            std::cout << "Target reached at position: (" 
                      << aiPosition.x << ", " 
                      << aiPosition.y << ", " 
                      << aiPosition.z << ")" << std::endl;
            return;
        }
        
        // Get current target node
        if (currentPathIndex >= currentPath.size()) {
            return;
        }
        
        size_t nodeIndex = currentPath[currentPathIndex];
        const NavNode& targetNode = navMesh->GetNodes()[nodeIndex];
        
        // Calculate direction to target node
        Vector3 direction = targetNode.position - aiPosition;
        float distance = direction.magnitude();
        
        // If close enough to current node, move to next node
        if (distance < 0.1f) {
            currentPathIndex++;
            
            if (currentPathIndex >= currentPath.size()) {
                // We've reached the end of the path
                return;
            }
            
            // Get the next node
            nodeIndex = currentPath[currentPathIndex];
            const NavNode& nextNode = navMesh->GetNodes()[nodeIndex];
            
            // Calculate direction to next node
            direction = nextNode.position - aiPosition;
            distance = direction.magnitude();
        }
        
        // Normalize direction
        if (distance > 0) {
            direction = direction / distance;
        }
        
        // Update orientation to match the normal of the current node
        aiOrientation = targetNode.normal;
        
        // Move towards target node
        float moveDistance = moveSpeed * deltaTime;
        if (moveDistance > distance) {
            moveDistance = distance;
        }
        
        // Update position
        aiPosition = aiPosition + direction * moveDistance;
        
        // Calculate heuristic value for the current position
        float heuristic = CalculateHeuristic(aiPosition, targetPosition);
        std::cout << "Heuristic value: " << heuristic << std::endl;
    }
    
    // Calculate heuristic value (Manhattan distance)
    float CalculateHeuristic(const Vector3& current, const Vector3& target) const {
        float dx = std::abs(current.x - target.x);
        float dy = std::abs(current.y - target.y);
        float dz = std::abs(current.z - target.z);
        return dx + dy + dz;
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
    // Create a tilted navigation mesh
    TiltedNavMesh navMesh;
    
    // Create a tilted plane (30 degree angle)
    const int gridSize = 10;
    const float spacing = 1.0f;
    const float angle = 30.0f * 3.14159f / 180.0f; // 30 degrees in radians
    
    std::cout << "Creating tilted navigation mesh with " << gridSize * gridSize << " nodes..." << std::endl;
    
    // Calculate the normal vector for the tilted plane
    Vector3 planeNormal(std::sin(angle), std::cos(angle), 0);
    planeNormal = planeNormal.normalized();
    
    // Add nodes to the navigation mesh
    for (int x = 0; x < gridSize; ++x) {
        for (int z = 0; z < gridSize; ++z) {
            // Calculate position on the tilted plane
            float y = x * std::sin(angle); // Height increases with x
            Vector3 position(x * spacing, y, z * spacing);
            navMesh.AddNode(position, 0, planeNormal);
        }
    }
    
    // Create point obstacles
    GameObject obstacle1("Obstacle1");
    obstacle1.position = Vector3(3, 3 * std::sin(angle), 3);
    obstacle1.size = Vector3(1, 1, 1);
    
    GameObject obstacle2("Obstacle2");
    obstacle2.position = Vector3(3, 3 * std::sin(angle), 4);
    obstacle2.size = Vector3(1, 1, 1);
    
    std::cout << "Created point obstacles at (" 
              << obstacle1.position.x << "," << obstacle1.position.y << "," << obstacle1.position.z << ") and ("
              << obstacle2.position.x << "," << obstacle2.position.y << "," << obstacle2.position.z << ")" << std::endl;
    
    // Get all nodes for connection
    const std::vector<NavNode>& nodes = navMesh.GetNodes();
    
    // Connect adjacent nodes
    for (size_t i = 0; i < nodes.size(); ++i) {
        const Vector3& currentPos = nodes[i].position;
        
        for (size_t j = i + 1; j < nodes.size(); ++j) {
            const Vector3& neighborPos = nodes[j].position;
            
            // Calculate 2D distance (ignoring y)
            float dx = std::abs(currentPos.x - neighborPos.x);
            float dz = std::abs(currentPos.z - neighborPos.z);
            
            // Connect orthogonal neighbors (up, down, left, right)
            if ((dx <= spacing + 0.01f && dz <= 0.01f) || (dx <= 0.01f && dz <= spacing + 0.01f)) {
                // Skip connections that go through obstacles
                bool throughObstacle = false;
                
                // Check if the connection passes through obstacle1
                if ((currentPos.x == obstacle1.position.x && neighborPos.x == obstacle1.position.x &&
                     ((currentPos.z < obstacle1.position.z && neighborPos.z > obstacle1.position.z) ||
                      (currentPos.z > obstacle1.position.z && neighborPos.z < obstacle1.position.z))) ||
                    (currentPos.z == obstacle1.position.z && neighborPos.z == obstacle1.position.z &&
                     ((currentPos.x < obstacle1.position.x && neighborPos.x > obstacle1.position.x) ||
                      (currentPos.x > obstacle1.position.x && neighborPos.x < obstacle1.position.x)))) {
                    throughObstacle = true;
                }
                
                // Check if the connection passes through obstacle2
                if ((currentPos.x == obstacle2.position.x && neighborPos.x == obstacle2.position.x &&
                     ((currentPos.z < obstacle2.position.z && neighborPos.z > obstacle2.position.z) ||
                      (currentPos.z > obstacle2.position.z && neighborPos.z < obstacle2.position.z))) ||
                    (currentPos.z == obstacle2.position.z && neighborPos.z == obstacle2.position.z &&
                     ((currentPos.x < obstacle2.position.x && neighborPos.x > obstacle2.position.x) ||
                      (currentPos.x > obstacle2.position.x && neighborPos.x < obstacle2.position.x)))) {
                    throughObstacle = true;
                }
                
                if (!throughObstacle) {
                    navMesh.ConnectNodes(i, j);
                }
            }
            
            // Connect diagonal neighbors
            if (dx <= spacing + 0.01f && dz <= spacing + 0.01f && dx > 0.01f && dz > 0.01f) {
                // Skip connections that go through obstacles
                bool throughObstacle = false;
                
                // Simple check for diagonal connections through obstacles
                if ((std::abs(currentPos.x - obstacle1.position.x) < 0.01f && std::abs(neighborPos.z - obstacle1.position.z) < 0.01f) ||
                    (std::abs(currentPos.z - obstacle1.position.z) < 0.01f && std::abs(neighborPos.x - obstacle1.position.x) < 0.01f) ||
                    (std::abs(currentPos.x - obstacle2.position.x) < 0.01f && std::abs(neighborPos.z - obstacle2.position.z) < 0.01f) ||
                    (std::abs(currentPos.z - obstacle2.position.z) < 0.01f && std::abs(neighborPos.x - obstacle2.position.x) < 0.01f)) {
                    throughObstacle = true;
                }
                
                if (!throughObstacle) {
                    navMesh.ConnectNodes(i, j);
                }
            }
        }
    }
    
    // Create an AI entity
    TiltedAIEntity aiEntity;
    aiEntity.SetAIPosition(Vector3(1, 1 * std::sin(angle), 1));
    aiEntity.SetAIOrientation(planeNormal);
    aiEntity.SetMaxAngleDiff(45.0f);
    aiEntity.SetMaxDist(2.0f);
    aiEntity.SetMoveSpeed(1.0f);
    aiEntity.SetNavMesh(&navMesh);
    
    // Set the target position
    Vector3 targetPosition(8, 8 * std::sin(angle), 8);
    aiEntity.SetTarget(targetPosition);
    
    // Main simulation loop
    float simulationTime = 0.0f;
    float deltaTime = 0.1f;
    const float maxSimulationTime = 60.0f; // Run for 60 seconds
    
    std::cout << "Starting A* pathfinding demo on tilted surface..." << std::endl;
    std::cout << "AI Entity starting position: (" << aiEntity.GetAIPosition().x << ", " 
              << aiEntity.GetAIPosition().y << ", " << aiEntity.GetAIPosition().z << ")" << std::endl;
    std::cout << "Target position: (" << targetPosition.x << ", " 
              << targetPosition.y << ", " << targetPosition.z << ")" << std::endl;
    std::cout << "Surface normal: (" << planeNormal.x << ", " << planeNormal.y << ", " << planeNormal.z << ")" << std::endl;
    
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
    
    std::cout << "Tilted surface A* pathfinding demo completed." << std::endl;
    
    return 0;
}
