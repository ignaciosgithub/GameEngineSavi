#include "NavMesh.h"
#include "GameObject.h"
#include <algorithm>
#include <limits>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

NavMesh::NavMesh() {
}

NavMesh::~NavMesh() {
    Clear();
}

void NavMesh::AddNode(const Vector3& position, float height, const Vector3& normal) {
    nodes.emplace_back(position, height, normal);
}

void NavMesh::ConnectNodes(size_t nodeA, size_t nodeB) {
    if (nodeA < nodes.size() && nodeB < nodes.size()) {
        // Add connection if it doesn't already exist
        if (std::find(nodes[nodeA].connections.begin(), nodes[nodeA].connections.end(), nodeB) == nodes[nodeA].connections.end()) {
            nodes[nodeA].connections.push_back(nodeB);
        }
        
        // Add reverse connection if it doesn't already exist
        if (std::find(nodes[nodeB].connections.begin(), nodes[nodeB].connections.end(), nodeA) == nodes[nodeB].connections.end()) {
            nodes[nodeB].connections.push_back(nodeA);
        }
    }
}

void NavMesh::AddObstacle(GameObject* obstacle) {
    if (obstacle && std::find(obstacles.begin(), obstacles.end(), obstacle) == obstacles.end()) {
        obstacles.push_back(obstacle);
    }
}

void NavMesh::RemoveObstacle(GameObject* obstacle) {
    obstacles.erase(std::remove(obstacles.begin(), obstacles.end(), obstacle), obstacles.end());
}

const std::vector<NavNode>& NavMesh::GetNodes() const {
    return nodes;
}

const std::vector<GameObject*>& NavMesh::GetObstacles() const {
    return obstacles;
}

bool NavMesh::IsAccessible(const Vector3& from, const Vector3& to, const Vector3& orientation, float maxAngleDiff, float maxDist) const {
    // Check if the distance is within the maximum allowed distance
    float distance = (to - from).magnitude();
    if (distance > maxDist) {
        return false;
    }
    
    // Calculate direction vector from 'from' to 'to'
    Vector3 direction = (to - from).normalized();
    
    // Check if the angle between the orientation and the direction is within the maximum allowed angle
    float dotProduct = orientation.normalized().dot(direction);
    // Clamp the dot product to the range [-1, 1] to avoid numerical issues
    dotProduct = std::max(-1.0f, std::min(1.0f, dotProduct));
    
    float angle = std::acos(dotProduct) * 180.0f / 3.14159f; // Convert to degrees
    if (angle > maxAngleDiff) {
        return false;
    }
    
    // Check if the path between from and to intersects with any obstacles
    for (const auto& obstacle : obstacles) {
        if (obstacle) {
            // Get obstacle bounds
            Vector3 obstaclePos = obstacle->position;
            Vector3 obstacleSize = obstacle->size;
            
            float minX = obstaclePos.x - obstacleSize.x / 2;
            float maxX = obstaclePos.x + obstacleSize.x / 2;
            float minY = obstaclePos.y - obstacleSize.y / 2;
            float maxY = obstaclePos.y + obstacleSize.y / 2;
            float minZ = obstaclePos.z - obstacleSize.z / 2;
            float maxZ = obstaclePos.z + obstacleSize.z / 2;
            
            // Check if either endpoint is inside the obstacle
            if ((from.x >= minX && from.x <= maxX && from.y >= minY && from.y <= maxY && from.z >= minZ && from.z <= maxZ) ||
                (to.x >= minX && to.x <= maxX && to.y >= minY && to.y <= maxY && to.z >= minZ && to.z <= maxZ)) {
                return false;
            }
            
            // Simple line-box intersection test for axis-aligned obstacles
            // This is a simplified approach and may not catch all edge cases
            
            // Check for intersection with each face of the obstacle
            // X-axis aligned faces
            if (direction.x != 0) {
                float t1 = (minX - from.x) / direction.x;
                float t2 = (maxX - from.x) / direction.x;
                
                float tMin = std::min(t1, t2);
                float tMax = std::max(t1, t2);
                
                if (tMin > 0 && tMin < distance) {
                    float y = from.y + direction.y * tMin;
                    float z = from.z + direction.z * tMin;
                    
                    if (y >= minY && y <= maxY && z >= minZ && z <= maxZ) {
                        return false;
                    }
                }
                
                if (tMax > 0 && tMax < distance) {
                    float y = from.y + direction.y * tMax;
                    float z = from.z + direction.z * tMax;
                    
                    if (y >= minY && y <= maxY && z >= minZ && z <= maxZ) {
                        return false;
                    }
                }
            }
            
            // Z-axis aligned faces
            if (direction.z != 0) {
                float t1 = (minZ - from.z) / direction.z;
                float t2 = (maxZ - from.z) / direction.z;
                
                float tMin = std::min(t1, t2);
                float tMax = std::max(t1, t2);
                
                if (tMin > 0 && tMin < distance) {
                    float x = from.x + direction.x * tMin;
                    float y = from.y + direction.y * tMin;
                    
                    if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
                        return false;
                    }
                }
                
                if (tMax > 0 && tMax < distance) {
                    float x = from.x + direction.x * tMax;
                    float y = from.y + direction.y * tMax;
                    
                    if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
                        return false;
                    }
                }
            }
        }
    }
    
    // Ensure the path stays on the ground plane (y=0) as much as possible
    // Allow small vertical movements based on the terrain, but prefer horizontal movement
    if (std::abs(to.y - from.y) > 0.5f) {
        // If vertical movement is significant, check if it's necessary
        // For example, if there's an obstacle in the way
        bool necessaryVerticalMovement = false;
        
        for (const auto& obstacle : obstacles) {
            if (obstacle) {
                Vector3 obstaclePos = obstacle->position;
                Vector3 obstacleSize = obstacle->size;
                
                // Check if the obstacle is in the way of a horizontal path
                Vector3 horizontalDirection = Vector3(direction.x, 0, direction.z).normalized();
                Vector3 horizontalTo = from + horizontalDirection * distance;
                
                float minX = obstaclePos.x - obstacleSize.x / 2;
                float maxX = obstaclePos.x + obstacleSize.x / 2;
                float minZ = obstaclePos.z - obstacleSize.z / 2;
                float maxZ = obstaclePos.z + obstacleSize.z / 2;
                
                // Check if horizontal path intersects with obstacle
                if ((from.x >= minX && from.x <= maxX && from.z >= minZ && from.z <= maxZ) ||
                    (horizontalTo.x >= minX && horizontalTo.x <= maxX && horizontalTo.z >= minZ && horizontalTo.z <= maxZ)) {
                    necessaryVerticalMovement = true;
                    break;
                }
            }
        }
        
        // If vertical movement isn't necessary, prefer horizontal movement
        if (!necessaryVerticalMovement) {
            return false;
        }
    }
    
    return true;
}

size_t NavMesh::FindNearestNode(const Vector3& position) const {
    size_t nearestIndex = 0;
    float nearestDistance = std::numeric_limits<float>::max();
    
    for (size_t i = 0; i < nodes.size(); ++i) {
        float distance = (nodes[i].position - position).magnitude();
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestIndex = i;
        }
    }
    
    return nearestIndex;
}

void NavMesh::Clear() {
    nodes.clear();
    obstacles.clear();
}

std::vector<size_t> NavMesh::FindPath(const Vector3& start, const Vector3& target, const Vector3& orientation, float maxAngleDiff, float maxDist) const {
    // Find nearest nodes to start and target
    size_t startNodeIndex = FindNearestNode(start);
    size_t targetNodeIndex = FindNearestNode(target);
    
    // Check if start or target nodes are valid
    if (startNodeIndex >= nodes.size() || targetNodeIndex >= nodes.size()) {
        std::cout << "Invalid start or target node indices" << std::endl;
        return std::vector<size_t>();
    }
    
    // Check if start and target are the same node
    if (startNodeIndex == targetNodeIndex) {
        return std::vector<size_t>{startNodeIndex};
    }
    
    // Use a priority queue for the open set to efficiently get the node with lowest f-score
    // The queue contains pairs of (f-score, node index)
    std::priority_queue<std::pair<float, size_t>, std::vector<std::pair<float, size_t>>, std::greater<std::pair<float, size_t>>> openSet;
    std::unordered_set<size_t> openSetIndices; // To quickly check if a node is in the open set
    std::unordered_set<size_t> closedSet;
    
    // Initialize cost maps
    std::vector<float> gScore(nodes.size(), std::numeric_limits<float>::max());
    std::vector<float> fScore(nodes.size(), std::numeric_limits<float>::max());
    
    // Initialize parent map for path reconstruction
    std::vector<size_t> parent(nodes.size(), std::numeric_limits<size_t>::max());
    
    // Set initial costs
    gScore[startNodeIndex] = 0.0f;
    fScore[startNodeIndex] = CalculateHeuristic(startNodeIndex, targetNodeIndex);
    
    // Add start node to open set
    openSet.push(std::make_pair(fScore[startNodeIndex], startNodeIndex));
    openSetIndices.insert(startNodeIndex);
    
    // Debug info
    std::cout << "Starting A* pathfinding from node " << startNodeIndex 
              << " (" << nodes[startNodeIndex].position.x << "," 
              << nodes[startNodeIndex].position.y << "," 
              << nodes[startNodeIndex].position.z << ") to node " 
              << targetNodeIndex << " (" << nodes[targetNodeIndex].position.x << "," 
              << nodes[targetNodeIndex].position.y << "," 
              << nodes[targetNodeIndex].position.z << ")" << std::endl;
    
    while (!openSet.empty()) {
        // Get node with lowest f-score from the priority queue
        size_t current = openSet.top().second;
        openSet.pop();
        openSetIndices.erase(current);
        
        // Check if we reached the target
        if (current == targetNodeIndex) {
            // Reconstruct path
            std::vector<size_t> path;
            size_t currentNode = current;
            
            while (currentNode != startNodeIndex) {
                path.push_back(currentNode);
                currentNode = parent[currentNode];
            }
            
            path.push_back(startNodeIndex);
            std::reverse(path.begin(), path.end());
            
            // Debug info
            std::cout << "Path found with " << path.size() << " nodes" << std::endl;
            return path;
        }
        
        // Add current to closedSet
        closedSet.insert(current);
        
        // Check all neighbors
        for (size_t neighbor : nodes[current].connections) {
            // Skip if neighbor is in closedSet
            if (closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }
            
            // Check if the neighbor is accessible
            if (!IsAccessible(nodes[current].position, nodes[neighbor].position, orientation, maxAngleDiff, maxDist)) {
                continue;
            }
            
            // Calculate tentative gScore
            float tentativeGScore = gScore[current] + nodes[current].position.distance(nodes[neighbor].position);
            
            // Vertical movement is handled by constraints in the nav mesh
            // float verticalPenalty = std::abs(nodes[neighbor].position.y - nodes[current].position.y) * 10.0f;
            // tentativeGScore += verticalPenalty;
            
            // Add a penalty for nodes near obstacles to encourage paths that stay away from obstacles
            for (const auto& obstacle : obstacles) {
                if (obstacle) {
                    Vector3 obstaclePos = obstacle->position;
                    Vector3 obstacleSize = obstacle->size;
                    
                    // Calculate distance to obstacle
                    Vector3 nodeToObstacle = nodes[neighbor].position - obstaclePos;
                    float distX = std::abs(nodeToObstacle.x) - obstacleSize.x / 2;
                    float distZ = std::abs(nodeToObstacle.z) - obstacleSize.z / 2;
                    
                    // If node is close to obstacle, add penalty
                    if (distX < 1.0f && distZ < 1.0f) {
                        float obstaclePenalty = (1.0f - std::min(distX, distZ)) * 5.0f;
                        tentativeGScore += obstaclePenalty;
                    }
                }
            }
            
            // Prefer paths that follow the user's suggested route: (1,0,1), (2,0,1)....(8,0,1).....(8,0,8)
            // This means preferring horizontal movement along x-axis first, then along z-axis
            const Vector3& currentPos = nodes[current].position;
            const Vector3& neighborPos = nodes[neighbor].position;
            const Vector3& targetPos = nodes[targetNodeIndex].position;
            
            // If we're not yet at the target x-coordinate, prefer moving along x-axis
            if (currentPos.x < targetPos.x && neighborPos.x > currentPos.x && neighborPos.z == currentPos.z) {
                tentativeGScore -= 0.5f; // Bonus for moving along x-axis
            }
            // If we're at or past the target x-coordinate, prefer moving along z-axis
            else if (currentPos.x >= targetPos.x && neighborPos.z > currentPos.z && neighborPos.x == currentPos.x) {
                tentativeGScore -= 0.5f; // Bonus for moving along z-axis
            }
            
            // If this path is better than any previous one
            if (tentativeGScore < gScore[neighbor]) {
                // This path is better, record it
                parent[neighbor] = current;
                gScore[neighbor] = tentativeGScore;
                fScore[neighbor] = gScore[neighbor] + CalculateHeuristic(neighbor, targetNodeIndex);
                
                // Add to open set if not already there
                if (openSetIndices.find(neighbor) == openSetIndices.end()) {
                    openSet.push(std::make_pair(fScore[neighbor], neighbor));
                    openSetIndices.insert(neighbor);
                }
                // Note: If the node is already in the open set, its priority in the queue won't be updated
                // This is a simplification. A more accurate implementation would remove and re-add the node
                // with the new priority, but this works well enough for most cases.
            }
        }
    }
    
    // No path found
    std::cout << "No path found" << std::endl;
    return std::vector<size_t>();
}

float NavMesh::CalculateHeuristic(size_t nodeIndex, size_t targetNodeIndex) const {
    // Use a modified Manhattan distance as heuristic to encourage grid-like movement
    // This will prefer paths that move along the x-axis first, then along the z-axis
    const Vector3& nodePos = nodes[nodeIndex].position;
    const Vector3& targetPos = nodes[targetNodeIndex].position;
    
    // Calculate Manhattan distance
    float dx = std::abs(nodePos.x - targetPos.x);
    float dy = std::abs(nodePos.y - targetPos.y);
    float dz = std::abs(nodePos.z - targetPos.z);
    
    // Vertical movement is handled by constraints in the nav mesh
    // float verticalPenalty = dy * 2.0f;
    
    // Encourage x-axis movement first, then z-axis movement
    float heuristic = dx + dz;
    
    // If we're not yet at the target x-coordinate, add a small bonus for x-axis movement
    if (nodePos.x < targetPos.x) {
        heuristic -= 0.1f * dx;
    }
    // If we're at or past the target x-coordinate, add a small bonus for z-axis movement
    else {
        heuristic -= 0.1f * dz;
    }
    
    return heuristic;
}
