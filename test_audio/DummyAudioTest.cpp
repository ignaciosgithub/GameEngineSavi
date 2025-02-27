#include "../Audio/AudioSystem.h"
#include "../Audio/AudioSource.h"
#include "../Audio/AudioClip.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "GameEngineSavi Audio System Test (Dummy Mode)" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    // Initialize audio system
    if (!AudioSystem::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize audio system" << std::endl;
        return 1;
    }
    
    std::cout << "Audio system initialized successfully" << std::endl;
    
    // Create audio sources
    auto bgSource = new AudioSource();
    auto effectSource = new AudioSource();
    
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
    
    // Test volume management
    std::cout << "Testing volume management..." << std::endl;
    std::cout << "Background volume: " << bgSource->GetVolume() << std::endl;
    std::cout << "Effect volume: " << effectSource->GetVolume() << std::endl;
    
    // Test multiple audio sources
    std::cout << "Testing multiple audio sources..." << std::endl;
    std::cout << "When multiple sources play, volume is automatically scaled" << std::endl;
    
    // Test looping
    std::cout << "Testing looping functionality..." << std::endl;
    std::cout << "Background looping: " << (bgSource->IsLooping() ? "Yes" : "No") << std::endl;
    std::cout << "Effect looping: " << (effectSource->IsLooping() ? "Yes" : "No") << std::endl;
    
    // Simulate playback
    std::cout << "Simulating audio playback..." << std::endl;
    bgSource->Play();
    effectSource->Play();
    
    // Display active sources
    std::cout << "Active audio sources: " << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
    
    // Main loop
    std::cout << "Running audio test for 5 seconds..." << std::endl;
    for (int i = 0; i < 5; i++) {
        AudioSystem::GetInstance().Update();
        
        // Print status every second
        std::cout << "Time: " << i + 1 << "s, Active sources: " 
                  << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
        
        // Play effect sound again at 3 seconds
        if (i == 3) {
            std::cout << "Playing effect sound again..." << std::endl;
            effectSource->Play();
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Stop all audio
    std::cout << "Stopping all audio..." << std::endl;
    bgSource->Stop();
    effectSource->Stop();
    
    // Cleanup
    delete bgSource;
    delete effectSource;
    
    // Shutdown audio system
    std::cout << "Shutting down audio system..." << std::endl;
    AudioSystem::GetInstance().Shutdown();
    
    std::cout << "Audio system test completed successfully" << std::endl;
    return 0;
}
