#include "NavMesh.h"
#include "GameObject.h"
#include <algorithm>
#include <limits>
#include <cmath>

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
    
    // Find the nearest nodes to the from and to positions
    size_t fromNodeIndex = FindNearestNode(from);
    size_t toNodeIndex = FindNearestNode(to);
    
    if (fromNodeIndex >= nodes.size() || toNodeIndex >= nodes.size()) {
        return false;
    }
    
    // Check if the angle between the orientation and the normal of the destination node is within the maximum allowed angle
    const NavNode& toNode = nodes[toNodeIndex];
    Vector3 normalizedOrientation = orientation.normalized();
    Vector3 normalizedNormal = toNode.normal.normalized();
    
    float dotProduct = normalizedOrientation.dot(normalizedNormal);
    // Clamp the dot product to the range [-1, 1] to avoid numerical issues
    dotProduct = std::max(-1.0f, std::min(1.0f, dotProduct));
    
    float angle = std::acos(dotProduct) * 180.0f / 3.14159f; // Convert to degrees
    
    return angle <= maxAngleDiff;
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
