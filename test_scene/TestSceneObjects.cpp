#include "../GameObject.h"
#include "../InvisibleWall.h"
#include "../TriggerVolume.h"
#include "../RigidBody.h"
#include "../Audio/AudioListener.h"
#include "../Audio/AudioSource.h"
#include "../Audio/AudioSystem.h"
#include "../Scene.h"
#include "../PhysicsSystem.h"
#include "../CollisionSystem.h"
#include <iostream>
#include <memory>

// Custom trigger volume that prints messages when objects enter/exit
class CustomTriggerVolume : public TriggerVolume {
public:
    void OnTriggerEnter() override {
        std::cout << "Object entered trigger volume!" << std::endl;
    }
    
    void OnTriggerExit() override {
        std::cout << "Object exited trigger volume!" << std::endl;
    }
    
    void OnTriggerStay() override {
        // Called every frame while an object is in the trigger
    }
};

// Test scene setup
void CreateTestScene(Scene* scene) {
    if (!scene) {
        std::cerr << "Error: Scene is null" << std::endl;
        return;
    }
    
    // Create physics system
    PhysicsSystem* physicsSystem = new PhysicsSystem();
    CollisionSystem* collisionSystem = new CollisionSystem();
    physicsSystem->SetCollisionSystem(collisionSystem);
    
    // Set global physics properties
    physicsSystem->SetGravity(-9.81f);
    physicsSystem->SetGlobalRestitution(0.5f); // Medium bounce by default
    
    // Create invisible wall
    GameObject* wall = new GameObject("InvisibleWall");
    wall->position = Vector3(0.0f, 2.0f, 0.0f);
    wall->size = Vector3(2.0f, 0.1f, 2.0f);
    auto wallComponent = wall->AddComponent(new InvisibleWall());
    scene->AddGameObject(wall);
    
    // Create trigger volume
    GameObject* trigger = new GameObject("TriggerVolume");
    trigger->position = Vector3(0.0f, 0.0f, 2.0f);
    trigger->size = Vector3(1.0f, 1.0f, 1.0f);
    auto triggerComponent = trigger->AddComponent(new CustomTriggerVolume());
    scene->AddGameObject(trigger);
    
    // Create audio listener
    GameObject* listener = new GameObject("AudioListener");
    listener->position = Vector3(0.0f, 0.0f, 0.0f);
    auto listenerComponent = listener->AddComponent(new AudioListener());
    scene->AddGameObject(listener);
    
    // Create test physics objects with different restitution values
    
    // High bounce sphere
    GameObject* sphere1 = new GameObject("HighBounceSphere");
    sphere1->position = Vector3(-1.0f, 5.0f, 0.0f);
    sphere1->size = Vector3(0.5f, 0.5f, 0.5f);
    auto rb1 = sphere1->AddComponent(new RigidBody());
    rb1->SetMass(1.0f);
    rb1->SetDynamic(true);
    rb1->EnableGravity(true);
    // Set high restitution (bounciness)
    rb1->SetFrictionCoefficient(0.1f); // Low friction
    physicsSystem->AddBody(rb1);
    scene->AddGameObject(sphere1);
    
    // Low bounce sphere
    GameObject* sphere2 = new GameObject("LowBounceSphere");
    sphere2->position = Vector3(1.0f, 5.0f, 0.0f);
    sphere2->size = Vector3(0.5f, 0.5f, 0.5f);
    auto rb2 = sphere2->AddComponent(new RigidBody());
    rb2->SetMass(1.0f);
    rb2->SetDynamic(true);
    rb2->EnableGravity(true);
    // Set low restitution (less bouncy)
    rb2->SetFrictionCoefficient(0.8f); // High friction
    physicsSystem->AddBody(rb2);
    scene->AddGameObject(sphere2);
    
    // Create an object with audio source
    GameObject* audioObject = new GameObject("AudioObject");
    audioObject->position = Vector3(0.0f, 1.0f, 0.0f);
    auto audioSource = audioObject->AddComponent(new AudioSource());
    
    // Load an audio clip
    auto audioClip = AudioSystem::GetInstance().LoadClip("test_audio/test.mp3");
    if (audioClip) {
        audioSource->SetClip(audioClip);
        audioSource->SetVolume(0.7f);
        audioSource->SetLoop(true);
        audioSource->Play();
    }
    scene->AddGameObject(audioObject);
    
    // Set the physics system for the scene
    scene->SetPhysicsSystem(physicsSystem);
    
    std::cout << "Test scene created with:" << std::endl;
    std::cout << "- Invisible wall at " << wall->position.toString() << std::endl;
    std::cout << "- Trigger volume at " << trigger->position.toString() << std::endl;
    std::cout << "- Audio listener at " << listener->position.toString() << std::endl;
    std::cout << "- High bounce sphere at " << sphere1->position.toString() << std::endl;
    std::cout << "- Low bounce sphere at " << sphere2->position.toString() << std::endl;
    std::cout << "- Audio object at " << audioObject->position.toString() << std::endl;
}

// Main function for standalone testing
int main(int argc, char** argv) {
    // Initialize audio system
    if (!AudioSystem::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize audio system" << std::endl;
    }
    
    // Create scene
    Scene scene;
    
    // Create test scene
    CreateTestScene(&scene);
    
    std::cout << "Press Enter to toggle invisible wall..." << std::endl;
    std::cin.get();
    
    // Find the invisible wall and toggle it
    for (auto gameObject : scene.GetGameObjects()) {
        if (gameObject->GetName() == "InvisibleWall") {
            auto wall = gameObject->GetComponents<InvisibleWall>()[0];
            if (wall) {
                if (wall->IsEnabled()) {
                    wall->OnDisable();
                    std::cout << "Invisible wall disabled" << std::endl;
                } else {
                    wall->OnEnable();
                    std::cout << "Invisible wall enabled" << std::endl;
                }
            }
            break;
        }
    }
    
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    
    // Shutdown audio system
    AudioSystem::GetInstance().Shutdown();
    
    return 0;
}
