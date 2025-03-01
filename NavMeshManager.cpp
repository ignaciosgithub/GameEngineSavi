#include "NavMeshManager.h"
#include "AIEntity.h"
#include "ProjectSettings/ProjectSettings.h"
#include <algorithm>

// Initialize static instance
NavMeshManager* NavMeshManager::instance = nullptr;

NavMeshManager::NavMeshManager()
    : navMesh(new NavMesh()),
      refreshRate(10.0f),
      timeSinceLastRefresh(0.0f) {
}

NavMeshManager::~NavMeshManager() {
    aiEntities.clear();
    delete navMesh;
}

NavMeshManager& NavMeshManager::GetInstance() {
    if (!instance) {
        instance = new NavMeshManager();
    }
    return *instance;
}

void NavMeshManager::Initialize() {
    // Load settings from ProjectSettings
    auto& settings = ProjectSettings::GetInstance();
    
    // Default refresh rate is 10 seconds (0.1 Hz)
    refreshRate = 10.0f;
    
    // Clear the navigation mesh
    navMesh->Clear();
    
    // Reset the refresh timer
    timeSinceLastRefresh = 0.0f;
}

void NavMeshManager::Update(float deltaTime) {
    // Update the refresh timer
    timeSinceLastRefresh += deltaTime;
    
    // Refresh the navigation mesh if needed
    if (timeSinceLastRefresh >= refreshRate) {
        RefreshNavMesh();
        timeSinceLastRefresh = 0.0f;
    }
}

void NavMeshManager::RefreshNavMesh() {
    // This is a simplified implementation
    // In a real implementation, you would analyze the scene and build the navigation mesh
    // based on the geometry of the scene
    
    // For now, we'll just clear the navigation mesh and rebuild it with some basic nodes
    navMesh->Clear();
    
    // Add some basic nodes to the navigation mesh
    // In a real implementation, you would analyze the scene and add nodes based on the geometry
    navMesh->AddNode(Vector3(0, 0, 0), 0, Vector3(0, 1, 0));
    navMesh->AddNode(Vector3(5, 0, 0), 0, Vector3(0, 1, 0));
    navMesh->AddNode(Vector3(0, 0, 5), 0, Vector3(0, 1, 0));
    navMesh->AddNode(Vector3(5, 0, 5), 0, Vector3(0, 1, 0));
    
    // Connect the nodes
    navMesh->ConnectNodes(0, 1);
    navMesh->ConnectNodes(0, 2);
    navMesh->ConnectNodes(1, 3);
    navMesh->ConnectNodes(2, 3);
    
    // Notify all AI entities that the NavMesh has been refreshed
    for (AIEntity* entity : aiEntities) {
        if (entity) {
            entity->OnNavMeshRefresh();
        }
    }
}

NavMesh* NavMeshManager::GetNavMesh() const {
    return navMesh;
}

void NavMeshManager::SetNavMesh(NavMesh* mesh) {
    if (mesh) {
        // Delete the old navMesh if it exists
        if (navMesh) {
            delete navMesh;
        }
        navMesh = mesh;
        
        // Notify all AI entities that the NavMesh has been updated
        for (AIEntity* entity : aiEntities) {
            if (entity) {
                entity->OnNavMeshRefresh();
            }
        }
    }
}

void NavMeshManager::AddAIEntity(AIEntity* entity) {
    if (entity && std::find(aiEntities.begin(), aiEntities.end(), entity) == aiEntities.end()) {
        aiEntities.push_back(entity);
    }
}

void NavMeshManager::RemoveAIEntity(AIEntity* entity) {
    aiEntities.erase(std::remove(aiEntities.begin(), aiEntities.end(), entity), aiEntities.end());
}

const std::vector<AIEntity*>& NavMeshManager::GetAIEntities() const {
    return aiEntities;
}

void NavMeshManager::SetRefreshRate(float rate) {
    refreshRate = rate > 0.0f ? rate : 10.0f;
}

float NavMeshManager::GetRefreshRate() const {
    return refreshRate;
}
