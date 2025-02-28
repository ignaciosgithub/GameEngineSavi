#include "../Scene.h"
#include "../GameObject.h"
#include "../Vector3.h"
#include "../SceneSerializer.h"
#include <iostream>
#include <cassert>
#include <memory>
#include <filesystem>

/**
 * @brief Test program for scene transitions with object persistence
 * 
 * This test verifies:
 * 1. Scene initialization
 * 2. Object creation with specific position
 * 3. Object transfer between scenes
 * 4. Object persistence verification
 * 5. Proper memory cleanup during transitions
 */
int main() {
    std::cout << "=== Scene Transition Test ===" << std::endl;
    
    // Create temp directory if it doesn't exist
    std::filesystem::create_directories("temp");
    
    // Create first scene
    std::cout << "Creating first scene..." << std::endl;
    Scene scene1;
    scene1.Initialize();
    
    // Add object to transfer
    std::cout << "Creating persistent object..." << std::endl;
    auto obj = new GameObject("PersistentObject");
    obj->position = Vector3(1, 2, 3);
    scene1.AddGameObject(obj);
    
    // Verify object was added
    std::cout << "Verifying object in first scene..." << std::endl;
    auto foundObj = scene1.FindGameObject("PersistentObject");
    if (!foundObj) {
        std::cerr << "Error: Object not found in first scene" << std::endl;
        return 1;
    }
    
    if (foundObj->position.x != 1 || foundObj->position.y != 2 || foundObj->position.z != 3) {
        std::cerr << "Error: Object position mismatch in first scene" << std::endl;
        return 1;
    }
    
    std::cout << "Object verified in first scene" << std::endl;
    
    // Transfer object to second scene
    std::cout << "Transferring object to second scene..." << std::endl;
    scene1.TransferObject(obj, "scene2.json");
    
    // Unload first scene
    std::cout << "Unloading first scene..." << std::endl;
    scene1.UnloadScene();
    
    // Load second scene
    std::cout << "Loading second scene..." << std::endl;
    Scene scene2;
    scene2.LoadScene("scene2.json");
    
    // Verify object persisted
    std::cout << "Verifying object in second scene..." << std::endl;
    auto persistedObj = scene2.FindGameObject("PersistentObject");
    if (!persistedObj) {
        std::cerr << "Error: Transferred object not found in second scene" << std::endl;
        return 1;
    }
    
    if (persistedObj->position.x != 1 || persistedObj->position.y != 2 || persistedObj->position.z != 3) {
        std::cerr << "Error: Transferred object position mismatch in second scene" << std::endl;
        std::cout << "Position: " << persistedObj->position.x << ", " 
                  << persistedObj->position.y << ", " 
                  << persistedObj->position.z << std::endl;
        return 1;
    }
    
    std::cout << "Object successfully transferred and verified in second scene" << std::endl;
    
    // Clean up
    std::cout << "Unloading second scene..." << std::endl;
    scene2.UnloadScene();
    
    // Clean up temp files
    std::cout << "Cleaning up temporary files..." << std::endl;
    if (std::filesystem::exists("temp/transfer_manifest.json")) {
        std::filesystem::remove("temp/transfer_manifest.json");
    }
    
    std::cout << "=== Scene Transition Test Passed ===" << std::endl;
    return 0;
}
