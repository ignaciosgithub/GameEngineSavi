#include "CollisionSystem.h"
#include "GameObject.h"
#include <limits>
#include <algorithm>

// Implementation of CollisionSystem methods

CollisionSystem::BoundingBox CollisionSystem::CalculateBoundingBox(const Model* model) {
    BoundingBox box;
    
    // Initialize min and max to extreme values
    box.min = Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    box.max = Vector3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
    
    // Since we can't access private members directly, we'll use a simplified approach
    // In a real implementation, Model would provide a method to get its bounding box
    
    // Simplified: Use a default box size based on the GameObject's position
    Vector3 position(0, 0, 0); // Default position
    float size = 1.0f; // Default size
    
    box.min = position - Vector3(size, size, size);
    box.max = position + Vector3(size, size, size);
    
    return box;
}
