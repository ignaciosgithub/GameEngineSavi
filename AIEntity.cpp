#include "AIEntity.h"
#include "GameObject.h"
#include "NavMesh.h"
#include "NavMeshManager.h"

AIEntity::AIEntity()
    : aiPosition(0, 0, 0),
      aiOrientation(0, 0, 1),
      maxAngleDiff(45.0f),
      maxDist(2.0f),
      gameObject(nullptr),
      currentPathIndex(0),
      targetPosition(0, 0, 0),
      hasTarget(false),
      moveSpeed(2.0f),
      pathNeedsUpdate(false) {
}

AIEntity::~AIEntity() {
}

void AIEntity::SetAIPosition(const Vector3& position) {
    aiPosition = position;
}

Vector3 AIEntity::GetAIPosition() const {
    return aiPosition;
}

void AIEntity::SetAIOrientation(const Vector3& orientation) {
    aiOrientation = orientation;
}

Vector3 AIEntity::GetAIOrientation() const {
    return aiOrientation;
}

void AIEntity::SetMaxAngleDiff(float angle) {
    maxAngleDiff = angle;
}

float AIEntity::GetMaxAngleDiff() const {
    return maxAngleDiff;
}

void AIEntity::SetMaxDist(float distance) {
    maxDist = distance;
}

float AIEntity::GetMaxDist() const {
    return maxDist;
}

void AIEntity::SetGameObject(GameObject* obj) {
    gameObject = obj;
}

GameObject* AIEntity::GetGameObject() const {
    return gameObject;
}

void AIEntity::SetTarget(const Vector3& target) {
    targetPosition = target;
    hasTarget = true;
    pathNeedsUpdate = true;
}

Vector3 AIEntity::GetTarget() const {
    return targetPosition;
}

void AIEntity::FindPathToTarget() {
    if (!hasTarget) {
        return;
    }
    
    // Get the NavMesh from the NavMeshManager
    NavMesh* navMesh = NavMeshManager::GetInstance().GetNavMesh();
    if (!navMesh) {
        return;
    }
    
    // Find a path from current position to target
    currentPath = navMesh->FindPath(aiPosition, targetPosition, aiOrientation, maxAngleDiff, maxDist);
    currentPathIndex = 0;
    pathNeedsUpdate = false;
    
    // Output path information
    std::cout << "AI Entity found path with " << currentPath.size() << " nodes" << std::endl;
    std::cout << "Current position: (" << aiPosition.x << ", " << aiPosition.y << ", " << aiPosition.z << ")" << std::endl;
    std::cout << "Target position: (" << targetPosition.x << ", " << targetPosition.y << ", " << targetPosition.z << ")" << std::endl;
}

void AIEntity::FollowPath(float deltaTime) {
    if (!hasTarget || currentPath.empty() || currentPathIndex >= currentPath.size()) {
        return;
    }
    
    // Get the NavMesh from the NavMeshManager
    NavMesh* navMesh = NavMeshManager::GetInstance().GetNavMesh();
    if (!navMesh) {
        return;
    }
    
    // Get the current target node
    const std::vector<NavNode>& nodes = navMesh->GetNodes();
    if (currentPathIndex >= currentPath.size() || currentPath[currentPathIndex] >= nodes.size()) {
        return;
    }
    
    // Get the position of the current target node
    Vector3 targetNodePosition = nodes[currentPath[currentPathIndex]].position;
    
    // Calculate direction to the target node
    Vector3 direction = (targetNodePosition - aiPosition).normalized();
    
    // Move towards the target node
    float distanceToMove = moveSpeed * deltaTime;
    float distanceToTarget = aiPosition.distance(targetNodePosition);
    
    // Check if we've reached the current target node
    if (distanceToTarget <= distanceToMove) {
        // Move to the target node
        aiPosition = targetNodePosition;
        
        // Move to the next node in the path
        currentPathIndex++;
        
        // Output position information
        std::cout << "AI Entity reached node " << currentPathIndex << " of " << currentPath.size() << std::endl;
        std::cout << "Position: (" << aiPosition.x << ", " << aiPosition.y << ", " << aiPosition.z << ")" << std::endl;
        
        // Check if we've reached the end of the path
        if (currentPathIndex >= currentPath.size()) {
            std::cout << "AI Entity reached target!" << std::endl;
        }
    } else {
        // Move towards the target node
        aiPosition += direction * distanceToMove;
        
        // Update orientation to face the direction of movement
        aiOrientation = direction;
        
        // Output position information (less frequently to avoid spam)
        static float timeSinceLastOutput = 0.0f;
        timeSinceLastOutput += deltaTime;
        if (timeSinceLastOutput >= 1.0f) {
            std::cout << "AI Entity moving to node " << currentPathIndex + 1 << " of " << currentPath.size() << std::endl;
            std::cout << "Position: (" << aiPosition.x << ", " << aiPosition.y << ", " << aiPosition.z << ")" << std::endl;
            timeSinceLastOutput = 0.0f;
        }
    }
}

bool AIEntity::HasReachedTarget() const {
    if (!hasTarget) {
        return false;
    }
    
    return aiPosition.distance(targetPosition) < 0.1f;
}

void AIEntity::OnNavMeshRefresh() {
    // Mark the path as needing an update
    pathNeedsUpdate = true;
    std::cout << "NavMesh refreshed, recalculating path..." << std::endl;
}

void AIEntity::SetMoveSpeed(float speed) {
    moveSpeed = speed > 0.0f ? speed : 1.0f;
}

float AIEntity::GetMoveSpeed() const {
    return moveSpeed;
}

void AIEntity::Update(float deltaTime) {
    // Check if the path needs to be updated
    if (pathNeedsUpdate) {
        FindPathToTarget();
    }
    
    // Follow the current path
    FollowPath(deltaTime);
    
    // Update the game object position based on the AI position
    if (gameObject) {
        gameObject->position = aiPosition;
        
        // Update rotation to match orientation
        gameObject->rotation = aiOrientation;
    }
}
