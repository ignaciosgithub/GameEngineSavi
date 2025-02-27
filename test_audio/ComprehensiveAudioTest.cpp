#include "../Audio/AudioSystem.h"
#include "../Audio/AudioSource.h"
#include "../Audio/AudioClip.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <memory>
#include <functional>

// Simple class to test conditional audio playback
class AudioCondition {
private:
    bool shouldStop;
    std::chrono::time_point<std::chrono::steady_clock> startTime;
    std::chrono::seconds duration;

public:
    AudioCondition(int durationSeconds) 
        : shouldStop(false), 
          startTime(std::chrono::steady_clock::now()),
          duration(durationSeconds) {}

    void Reset() {
        shouldStop = false;
        startTime = std::chrono::steady_clock::now();
    }

    bool ShouldStop() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
        return elapsed >= duration;
    }
};

int main() {
    std::cout << "GameEngineSavi Comprehensive Audio Test" << std::endl;
    std::cout << "=======================================" << std::endl;
    
    // Initialize audio system
    if (!AudioSystem::GetInstance().Initialize()) {
        std::cerr << "Failed to initialize audio system" << std::endl;
        return 1;
    }
    
    std::cout << "Audio system initialized successfully" << std::endl;
    
    // Create audio sources
    std::vector<std::unique_ptr<AudioSource>> sources;
    
    // Background music
    auto bgSource = new AudioSource();
    sources.emplace_back(bgSource);
    
    // Effect sounds
    auto effect1Source = new AudioSource();
    sources.emplace_back(effect1Source);
    
    auto effect2Source = new AudioSource();
    sources.emplace_back(effect2Source);
    
    // Conditional sound
    auto conditionalSource = new AudioSource();
    sources.emplace_back(conditionalSource);
    
    // Load audio clips
    std::cout << "Loading audio clips..." << std::endl;
    
    // Create placeholder files if they don't exist
    system("mkdir -p test_audio/samples");
    if (system("test -f test_audio/samples/background.mp3") != 0) {
        std::cout << "Creating placeholder background music..." << std::endl;
        system("dd if=/dev/urandom of=test_audio/samples/background.mp3 bs=1k count=10");
    }
    
    if (system("test -f test_audio/samples/effect1.mp3") != 0) {
        std::cout << "Creating placeholder effect1 sound..." << std::endl;
        system("dd if=/dev/urandom of=test_audio/samples/effect1.mp3 bs=1k count=5");
    }
    
    if (system("test -f test_audio/samples/effect2.mp3") != 0) {
        std::cout << "Creating placeholder effect2 sound..." << std::endl;
        system("dd if=/dev/urandom of=test_audio/samples/effect2.mp3 bs=1k count=5");
    }
    
    if (system("test -f test_audio/samples/conditional.mp3") != 0) {
        std::cout << "Creating placeholder conditional sound..." << std::endl;
        system("dd if=/dev/urandom of=test_audio/samples/conditional.mp3 bs=1k count=5");
    }
    
    // Load the clips
    auto bgClip = AudioSystem::GetInstance().LoadClip("test_audio/samples/background.mp3");
    auto effect1Clip = AudioSystem::GetInstance().LoadClip("test_audio/samples/effect1.mp3");
    auto effect2Clip = AudioSystem::GetInstance().LoadClip("test_audio/samples/effect2.mp3");
    auto conditionalClip = AudioSystem::GetInstance().LoadClip("test_audio/samples/conditional.mp3");
    
    // Set up audio sources
    bgSource->SetClip(bgClip);
    bgSource->SetVolume(0.5f);
    bgSource->SetLoop(true);
    
    effect1Source->SetClip(effect1Clip);
    effect1Source->SetVolume(0.7f);
    
    effect2Source->SetClip(effect2Clip);
    effect2Source->SetVolume(0.7f);
    
    conditionalSource->SetClip(conditionalClip);
    conditionalSource->SetVolume(0.6f);
    conditionalSource->SetLoop(true);
    
    // Create condition for conditional audio
    auto condition = std::make_shared<AudioCondition>(3); // 3 seconds duration
    conditionalSource->SetStopCondition([condition]() { return condition->ShouldStop(); });
    
    // Test 1: Volume management with multiple sources
    std::cout << "\nTest 1: Volume Management with Multiple Sources" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    std::cout << "Playing background music..." << std::endl;
    bgSource->Play();
    
    // Display initial volume
    std::cout << "Background volume: " << bgSource->GetVolume() << std::endl;
    std::cout << "Active sources: " << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Play first effect
    std::cout << "Playing effect 1..." << std::endl;
    effect1Source->Play();
    std::cout << "Effect 1 volume: " << effect1Source->GetVolume() << std::endl;
    std::cout << "Active sources: " << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Play second effect
    std::cout << "Playing effect 2..." << std::endl;
    effect2Source->Play();
    std::cout << "Effect 2 volume: " << effect2Source->GetVolume() << std::endl;
    std::cout << "Active sources: " << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Test 2: Conditional stopping
    std::cout << "\nTest 2: Conditional Stopping" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Playing conditional sound (will stop after 3 seconds)..." << std::endl;
    condition->Reset();
    conditionalSource->Play();
    
    // Wait and update
    for (int i = 0; i < 5; i++) {
        std::cout << "Time: " << i + 1 << "s, Conditional playing: " 
                  << (conditionalSource->IsPlaying() ? "Yes" : "No") << std::endl;
        AudioSystem::GetInstance().Update();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    // Test 3: Master volume control
    std::cout << "\nTest 3: Master Volume Control" << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Setting master volume to 0.3..." << std::endl;
    AudioSystem::GetInstance().SetMasterVolume(0.3f);
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "Setting master volume back to 1.0..." << std::endl;
    AudioSystem::GetInstance().SetMasterVolume(1.0f);
    
    // Wait a bit
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // Test 4: Resource cleanup
    std::cout << "\nTest 4: Resource Cleanup" << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << "Stopping all audio..." << std::endl;
    
    for (auto& source : sources) {
        source->Stop();
    }
    
    std::cout << "Active sources after stopping: " << AudioSystem::GetInstance().GetActiveSourceCount() << std::endl;
    
    // Shutdown audio system
    std::cout << "\nShutting down audio system..." << std::endl;
    AudioSystem::GetInstance().Shutdown();
    
    std::cout << "\nAudio test completed successfully" << std::endl;
    return 0;
}
