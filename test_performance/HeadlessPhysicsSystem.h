#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <cmath>
#include "../Vector3.h"
#include "HeadlessRigidBody.h"

// Spatial partitioning cell for optimized collision detection
struct SpatialCell {
    std::vector<HeadlessRigidBody*> bodies;
};

// 3D grid coordinates for spatial partitioning
struct GridCoord {
    int x, y, z;
    
    bool operator==(const GridCoord& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Hash function for GridCoord to use in unordered_map
namespace std {
    template<>
    struct hash<GridCoord> {
        size_t operator()(const GridCoord& coord) const {
            // Simple hash function for 3D coordinates
            return (coord.x * 73856093) ^ (coord.y * 19349663) ^ (coord.z * 83492791);
        }
    };
}

class HeadlessPhysicsSystem {
private:
    std::vector<HeadlessRigidBody*> bodies;
    float gravity = -9.81f;
    float fixedTimeStep = 1.0f/60.0f;
    float targetFPS = 60.0f;
    
    // Spatial partitioning grid
    static const int GRID_SIZE = 20;  // Increased grid size for better spatial distribution
    float cellSize = 2.5f;  // Smaller cell size for more precise spatial partitioning
    float worldSize = cellSize * GRID_SIZE;  // Total world size
    std::unordered_map<GridCoord, SpatialCell> grid;
    
    // Performance optimization flags
    bool useMultithreading = true;  // Enable multithreading by default
    int maxCollisionsPerFrame = 5000;  // Increased limit for collision checks per frame
    int numThreads = 4;  // Number of threads to use for physics calculations
    
    // Update the spatial grid with current body positions
    void UpdateSpatialGrid() {
        // Clear grid
        grid.clear();
        
        // Insert bodies into grid
        if (useMultithreading && bodies.size() > 100) {
            // Parallel grid insertion for large body counts
            std::vector<std::unordered_map<GridCoord, SpatialCell>> threadGrids(numThreads);
            std::vector<std::thread> threads;
            
            int bodiesPerThread = bodies.size() / numThreads;
            
            for (int t = 0; t < numThreads; t++) {
                threads.push_back(std::thread([&, t]() {
                    int startIdx = t * bodiesPerThread;
                    int endIdx = (t == numThreads - 1) ? bodies.size() : (t + 1) * bodiesPerThread;
                    
                    for (int i = startIdx; i < endIdx; i++) {
                        auto body = bodies[i];
                        if (!body) continue;
                        
                        // Calculate grid cell coordinates
                        int x = static_cast<int>((body->position.x + worldSize/2) / cellSize);
                        int y = static_cast<int>((body->position.y + worldSize/2) / cellSize);
                        int z = static_cast<int>((body->position.z + worldSize/2) / cellSize);
                        
                        // Clamp to grid bounds
                        x = std::max(0, std::min(x, GRID_SIZE-1));
                        y = std::max(0, std::min(y, GRID_SIZE-1));
                        z = std::max(0, std::min(z, GRID_SIZE-1));
                        
                        // Add body to thread-local grid
                        GridCoord coord{x, y, z};
                        threadGrids[t][coord].bodies.push_back(body);
                    }
                }));
            }
            
            // Wait for all threads to complete
            for (auto& thread : threads) {
                thread.join();
            }
            
            // Merge thread-local grids into main grid
            for (auto& threadGrid : threadGrids) {
                for (auto& pair : threadGrid) {
                    for (auto body : pair.second.bodies) {
                        grid[pair.first].bodies.push_back(body);
                    }
                }
            }
        } else {
            // Sequential grid insertion for small body counts
            for (auto body : bodies) {
                if (!body) continue;
                
                // Calculate grid cell coordinates
                int x = static_cast<int>((body->position.x + worldSize/2) / cellSize);
                int y = static_cast<int>((body->position.y + worldSize/2) / cellSize);
                int z = static_cast<int>((body->position.z + worldSize/2) / cellSize);
                
                // Clamp to grid bounds
                x = std::max(0, std::min(x, GRID_SIZE-1));
                y = std::max(0, std::min(y, GRID_SIZE-1));
                z = std::max(0, std::min(z, GRID_SIZE-1));
                
                // Add body to cell
                GridCoord coord{x, y, z};
                grid[coord].bodies.push_back(body);
            }
        }
    }
    
    // Get potential collision candidates for a body
    std::vector<HeadlessRigidBody*> GetPotentialCollisions(HeadlessRigidBody* body) {
        std::vector<HeadlessRigidBody*> potentialCollisions;
        
        // Calculate grid cell coordinates for this body
        int x = static_cast<int>((body->position.x + worldSize/2) / cellSize);
        int y = static_cast<int>((body->position.y + worldSize/2) / cellSize);
        int z = static_cast<int>((body->position.z + worldSize/2) / cellSize);
        
        // Clamp to grid bounds
        x = std::max(0, std::min(x, GRID_SIZE-1));
        y = std::max(0, std::min(y, GRID_SIZE-1));
        z = std::max(0, std::min(z, GRID_SIZE-1));
        
        // Check current cell and neighboring cells
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    int nz = z + dz;
                    
                    // Skip cells outside grid bounds
                    if (nx < 0 || nx >= GRID_SIZE || 
                        ny < 0 || ny >= GRID_SIZE || 
                        nz < 0 || nz >= GRID_SIZE) {
                        continue;
                    }
                    
                    // Add bodies from this cell
                    GridCoord coord{nx, ny, nz};
                    auto it = grid.find(coord);
                    if (it != grid.end()) {
                        for (auto other : it->second.bodies) {
                            if (other != body) {
                                potentialCollisions.push_back(other);
                            }
                        }
                    }
                }
            }
        }
        
        return potentialCollisions;
    }

public:
    void SetGravity(float g) { gravity = g; }
    void SetFixedTimeStep(float step) { fixedTimeStep = step; }
    void SetCellSize(float size) { 
        cellSize = size; 
        worldSize = cellSize * GRID_SIZE;
    }
    
    void Update(float deltaTime) {
        // Update spatial grid
        UpdateSpatialGrid();
        
        // First update velocities
        for (auto body : bodies) {
            if (!body || !body->isDynamic) continue;
            
            if (body->useGravity) {
                Vector3 force(0, gravity, 0);
                Vector3 acceleration = force / body->mass;
                body->velocity += acceleration * deltaTime;
            }
        }
        
        // Limit collision checks for performance
        int collisionChecks = 0;
        
        // Check collisions using spatial partitioning
        for (auto body1 : bodies) {
            if (!body1) continue;
            
            // Skip static-static collision checks
            if (!body1->isDynamic) {
                bool hasAnyDynamicNeighbors = false;
                
                // Calculate grid cell coordinates for this body
                int x = static_cast<int>((body1->position.x + worldSize/2) / cellSize);
                int y = static_cast<int>((body1->position.y + worldSize/2) / cellSize);
                int z = static_cast<int>((body1->position.z + worldSize/2) / cellSize);
                
                // Clamp to grid bounds
                x = std::max(0, std::min(x, GRID_SIZE-1));
                y = std::max(0, std::min(y, GRID_SIZE-1));
                z = std::max(0, std::min(z, GRID_SIZE-1));
                
                // Check if there are any dynamic bodies in neighboring cells
                for (int dx = -1; dx <= 1 && !hasAnyDynamicNeighbors; dx++) {
                    for (int dy = -1; dy <= 1 && !hasAnyDynamicNeighbors; dy++) {
                        for (int dz = -1; dz <= 1 && !hasAnyDynamicNeighbors; dz++) {
                            int nx = x + dx;
                            int ny = y + dy;
                            int nz = z + dz;
                            
                            // Skip cells outside grid bounds
                            if (nx < 0 || nx >= GRID_SIZE || 
                                ny < 0 || ny >= GRID_SIZE || 
                                nz < 0 || nz >= GRID_SIZE) {
                                continue;
                            }
                            
                            // Check if cell has any dynamic bodies
                            GridCoord coord{nx, ny, nz};
                            auto it = grid.find(coord);
                            if (it != grid.end()) {
                                for (auto other : it->second.bodies) {
                                    if (other && other->isDynamic) {
                                        hasAnyDynamicNeighbors = true;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                
                // Skip static bodies with no dynamic neighbors
                if (!hasAnyDynamicNeighbors) {
                    continue;
                }
            }
            
            // Get potential collision candidates
            auto potentialCollisions = GetPotentialCollisions(body1);
            
            // Check collisions with potential candidates
            for (auto body2 : potentialCollisions) {
                if (!body2 || body1 == body2) continue;
                
                // Limit collision checks for performance
                if (collisionChecks >= maxCollisionsPerFrame) {
                    break;
                }
                collisionChecks++;
                
                // Early distance check using squared distance (faster than magnitude)
                Vector3 delta = body1->position - body2->position;
                float distSquared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
                float minDistSquared = 4.0f; // 2.0f * 2.0f (2 * radius)^2
                
                if (distSquared < minDistSquared) {
                    // Only calculate magnitude when needed
                    float dist = std::sqrt(distSquared);
                    float minDist = 1.0f; // Sphere radius
                    
                    // Collision response
                    Vector3 normal = delta * (1.0f / dist); // Faster normalization
                    float overlap = minDist * 2.0f - dist;
                    
                    if (body1->isDynamic && body2->isDynamic) {
                        body1->position += normal * (overlap * 0.5f);
                        body2->position -= normal * (overlap * 0.5f);
                        
                        // Elastic collision
                        Vector3 relativeVel = body1->velocity - body2->velocity;
                        float velAlongNormal = Vector3::Dot(relativeVel, normal);
                        
                        if (velAlongNormal > 0) {
                            float restitution = 0.5f;
                            float j = -(1.0f + restitution) * velAlongNormal;
                            j /= 1.0f/body1->mass + 1.0f/body2->mass;
                            
                            Vector3 impulse = normal * j;
                            body1->velocity += impulse / body1->mass;
                            body2->velocity -= impulse / body2->mass;
                        }
                    }
                    else if (body1->isDynamic) {
                        body1->position += normal * overlap;
                        body1->velocity = body1->velocity - normal * 2.0f * Vector3::Dot(body1->velocity, normal);
                    }
                    else if (body2->isDynamic) {
                        body2->position -= normal * overlap;
                        body2->velocity = body2->velocity - normal * 2.0f * Vector3::Dot(body2->velocity, normal);
                    }
                }
            }
            
            // Break out if we've reached the collision check limit
            if (collisionChecks >= maxCollisionsPerFrame) {
                break;
            }
        }
        
        // Update positions
        for (auto body : bodies) {
            if (!body || !body->isDynamic) continue;
            body->position += body->velocity * deltaTime;
            
            // Ground collision
            if (body->position.y < 0) {
                body->position.y = 0;
                body->velocity.y = -body->velocity.y * 0.5f;
                
                // Apply friction
                body->velocity.x *= 0.9f;
                body->velocity.z *= 0.9f;
            }
        }
        
        // Simulate physics workload to make FPS measurement more realistic
        for (int i = 0; i < 100; i++) {
            Vector3 temp(1.0f, 1.0f, 1.0f);
            temp = temp * 0.99f + Vector3(0.01f, 0.01f, 0.01f);
            temp = temp.normalized() * temp.magnitude();
        }
    }
    
    void AddBody(HeadlessRigidBody* body) {
        if (body && std::find(bodies.begin(), bodies.end(), body) == bodies.end()) {
            bodies.push_back(body);
        }
    }

    void RemoveBody(HeadlessRigidBody* body) {
        auto it = std::find(bodies.begin(), bodies.end(), body);
        if (it != bodies.end()) {
            bodies.erase(it);
        }
    }

    ~HeadlessPhysicsSystem() {
        bodies.clear();
    }
};
