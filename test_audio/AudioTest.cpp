#include "../Audio/AudioSystem.h"
#include "../Audio/AudioSource.h"
#include "../Scene.h"
#include "../GameObject.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "GameEngineSavi Audio Test" << std::endl;
    std::cout << "=========================" << std::endl;
    
    // Initialize audio system
    if (!AudioSystem::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize audio system" << std::endl;
        return 1;
    }
    
    std::cout << "Audio system initialized successfully" << std::endl;
    
    // Create scene with multiple audio sources
    Scene scene;
    
    // Create background music object
    auto bgMusic = std::make_shared<GameObject>("BackgroundMusic");
    auto bgSource = std::make_shared<AudioSource>();
    bgMusic->AddComponent(bgSource.get());
    
    // Create effect sound object
    auto effect = std::make_shared<GameObject>("Effect");
    auto effectSource = std::make_shared<AudioSource>();
    effect->AddComponent(effectSource.get());
    
    // Add game objects to scene
    scene.AddGameObject(bgMusic);
    scene.AddGameObject(effect);
    
    // Load audio clips
    std::cout << "Loading audio clips..." << std::endl;
    auto bgClip = AudioSystem::GetInstance().LoadClip("test_audio/background.mp3");
    auto effectClip = AudioSystem::GetInstance().LoadClip("test_audio/effect.mp3");
    
    if (!bgClip) {
        std::cerr << "Failed to load background music" << std::endl;
        std::cout << "Creating placeholder background music..." << std::endl;
        // Create directory if it doesn't exist
        system("mkdir -p test_audio");
        // Create placeholder MP3 file
        system("dd if=/dev/urandom of=test_audio/background.mp3 bs=1k count=10");
        bgClip = AudioSystem::GetInstance().LoadClip("test_audio/background.mp3");
    }
    
    if (!effectClip) {
        std::cerr << "Failed to load effect sound" << std::endl;
        std::cout << "Creating placeholder effect sound..." << std::endl;
        // Create directory if it doesn't exist
        system("mkdir -p test_audio");
        // Create placeholder MP3 file
        system("dd if=/dev/urandom of=test_audio/effect.mp3 bs=1k count=5");
        effectClip = AudioSystem::GetInstance().LoadClip("test_audio/effect.mp3");
    }
    
    // Set audio clips
    bgSource->SetClip(bgClip);
    bgSource->SetVolume(0.5f);
    bgSource->SetLoop(true);
    
    effectSource->SetClip(effectClip);
    effectSource->SetVolume(0.7f);
    
    // Start playback
    std::cout << "Starting background music playback (looping)..." << std::endl;
    bgSource->Play();
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Play effect sound
    std::cout << "Playing effect sound (one-shot)..." << std::endl;
    effectSource->Play();
    
    // Display active sources
    std::cout << "Active audio sources: " << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
    
    // Main loop
    std::cout << "Running audio test for 10 seconds..." << std::endl;
    for (int i = 0; i < 10; i++) {
        scene.Update();
        AudioSystem::GetInstance().Update();
        
        // Print status every second
        std::cout << "Time: " << i + 1 << "s, Active sources: " 
                  << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
        
        // Play effect sound again at 5 seconds
        if (i == 5) {
            std::cout << "Playing effect sound again..." << std::endl;
            effectSource->Play();
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Stop all audio
    std::cout << "Stopping all audio..." << std::endl;
    bgSource->Stop();
    effectSource->Stop();
    
    // Shutdown audio system
    std::cout << "Shutting down audio system..." << std::endl;
    AudioSystem::GetInstance().Shutdown();
    
    std::cout << "Audio test completed successfully" << std::endl;
    return 0;
}
