#ifndef NAV_MESH_H
#define NAV_MESH_H

#include "Vector3.h"
#include <vector>
#include <memory>

// Forward declarations
class GameObject;

// Structure to represent a navigation mesh node
struct NavNode {
    Vector3 position;
    std::vector<size_t> connections;
    float height;
    Vector3 normal;
    
    NavNode(const Vector3& pos, float h, const Vector3& norm)
        : position(pos), height(h), normal(norm) {}
};

class NavMesh {
private:
    std::vector<NavNode> nodes;
    std::vector<GameObject*> obstacles;
    
public:
    NavMesh();
    ~NavMesh();
    
    // Add a node to the navigation mesh
    void AddNode(const Vector3& position, float height, const Vector3& normal);
    
    // Connect two nodes
    void ConnectNodes(size_t nodeA, size_t nodeB);
    
    // Add an obstacle to the navigation mesh
    void AddObstacle(GameObject* obstacle);
    
    // Remove an obstacle from the navigation mesh
    void RemoveObstacle(GameObject* obstacle);
    
    // Get all nodes in the navigation mesh
    const std::vector<NavNode>& GetNodes() const;
    
    // Get all obstacles in the navigation mesh
    const std::vector<GameObject*>& GetObstacles() const;
    
    // Check if a position is accessible from a given position with a given orientation
    bool IsAccessible(const Vector3& from, const Vector3& to, const Vector3& orientation, float maxAngleDiff, float maxDist) const;
    
    // Find the nearest node to a given position
    size_t FindNearestNode(const Vector3& position) const;
    
    // Clear the navigation mesh
    void Clear();
};

#endif // NAV_MESH_H
