#include "Scene.h"
#include "GameObject.h"
#include "SceneSerializer.h"
#include "MonoBehaviourLike.h"
#include <iostream>
#include <memory>

// Example RPG scene transition component
class SceneTransitionTrigger : public MonoBehaviourLike {
private:
    std::string targetScene;
    std::string spawnPoint;
    bool playerInRange;

public:
    SceneTransitionTrigger(const std::string& targetScene, const std::string& spawnPoint)
        : targetScene(targetScene), spawnPoint(spawnPoint), playerInRange(false) {}

    void OnTriggerEnter() override {
        playerInRange = true;
        std::cout << "Player entered transition trigger area. Press 'E' to enter " << targetScene << std::endl;
    }

    void OnTriggerExit() override {
        playerInRange = false;
    }

    void Update(float deltaTime) override {
        // Check if player is in range and pressed the interaction key
        if (playerInRange && IsKeyPressed('E')) {
            // Save player state before transition
            GameObject* player = GetScene()->FindGameObject("Player");
            if (player) {
                SceneSerializer::SaveObjectToJson(player, "player_persistence.json");
                std::cout << "Saved player state to player_persistence.json" << std::endl;
            }

            // Transition to the new scene
            TransitionToScene(targetScene, spawnPoint);
        }
    }

    void TransitionToScene(const std::string& scenePath, const std::string& spawnPointName) {
        std::cout << "Transitioning to scene: " << scenePath << std::endl;

        // Load the new scene
        Scene* newScene = new Scene();
        newScene->Load(scenePath);

        // Find the spawn point in the new scene
        GameObject* spawnPointObj = newScene->FindGameObject(spawnPointName);
        Vector3 spawnPosition = spawnPointObj ? spawnPointObj->GetPosition() : Vector3(0, 0, 0);

        // Load the persistent player data
        GameObject* persistentPlayer = SceneSerializer::LoadObjectFromJson("player_persistence.json");
        if (persistentPlayer) {
            // Position the player at the spawn point
            persistentPlayer->SetPosition(spawnPosition);
            
            // Add the player to the new scene
            newScene->AddGameObject(persistentPlayer);
            std::cout << "Loaded persistent player data at spawn point: " << spawnPointName << std::endl;
        }

        // Set the new scene as the active scene
        // In a real implementation, this would be handled by a SceneManager
        GetScene()->Shutdown();
        delete GetScene();
        SetActiveScene(newScene);
    }

    // Helper methods (would be implemented in a real game)
    bool IsKeyPressed(char key) { return false; /* Placeholder */ }
    Scene* GetScene() { return nullptr; /* Placeholder */ }
    void SetActiveScene(Scene* scene) { /* Placeholder */ }
};

// Example usage in main game code
int main() {
    // Load the initial scene (main world)
    Scene* mainWorld = new Scene();
    mainWorld->Load("Scenes/main_world.json");

    // Game loop would run here
    // ...

    return 0;
}
