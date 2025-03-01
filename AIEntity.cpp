#include "AIEntity.h"
#include "GameObject.h"

AIEntity::AIEntity()
    : aiPosition(0, 0, 0),
      aiOrientation(0, 0, 1),
      maxAngleDiff(45.0f),
      maxDist(2.0f),
      gameObject(nullptr) {
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

void AIEntity::Update(float deltaTime) {
    // Update the game object position based on the AI position if needed
    if (gameObject) {
        gameObject->position = aiPosition;
        
        // Update rotation to match orientation
        // This is a simplified approach; in a real implementation, you would calculate the appropriate rotation
        // based on the orientation vector
        gameObject->rotation = aiOrientation;
    }
}
