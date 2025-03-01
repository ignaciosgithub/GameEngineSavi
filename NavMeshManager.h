#ifndef NAV_MESH_MANAGER_H
#define NAV_MESH_MANAGER_H

#include "NavMesh.h"
#include <memory>
#include <vector>

class AIEntity;

class NavMeshManager {
private:
    NavMesh* navMesh;
    std::vector<AIEntity*> aiEntities;
    float refreshRate;
    float timeSinceLastRefresh;
    
    // Singleton instance
    static NavMeshManager* instance;
    
public:
    NavMeshManager();
    ~NavMeshManager();
    
    // Get singleton instance
    static NavMeshManager& GetInstance();
    
    // Initialize the navigation mesh manager
    void Initialize();
    
    // Update the navigation mesh
    void Update(float deltaTime);
    
    // Refresh the navigation mesh
    void RefreshNavMesh();
    
    // Get the navigation mesh
    NavMesh* GetNavMesh() const;
    
    // Set the navigation mesh
    void SetNavMesh(NavMesh* mesh);
    
    // Add an AI entity to the manager
    void AddAIEntity(AIEntity* entity);
    
    // Remove an AI entity from the manager
    void RemoveAIEntity(AIEntity* entity);
    
    // Get all AI entities
    const std::vector<AIEntity*>& GetAIEntities() const;
    
    // Set the refresh rate for the navigation mesh
    void SetRefreshRate(float rate);
    
    // Get the refresh rate for the navigation mesh
    float GetRefreshRate() const;
};

#endif // NAV_MESH_MANAGER_H
