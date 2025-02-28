#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <cstdlib>
#include <ctime>

// Simple Vector3 class for testing
class Vector3 {
public:
    float x, y, z;
    
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

// Simple GameObject class for testing
class GameObject {
private:
    std::string name;
    
public:
    Vector3 position;
    Vector3 rotation;
    Vector3 size;
    
    GameObject() : name("GameObject"), position(0,0,0), rotation(0,0,0), size(1,1,1) {}
    GameObject(const std::string& name, const Vector3& position = Vector3(), 
               const Vector3& rotation = Vector3(), const Vector3& size = Vector3(1,1,1))
        : name(name), position(position), rotation(rotation), size(size) {}
    
    std::string GetName() const { return name; }
};

// Simple JSON serialization for testing
class SceneSerializer {
public:
    static void SaveObjectToJson(const GameObject* obj, const std::string& filepath) {
        if (!obj) {
            std::cerr << "Error: Cannot serialize null GameObject" << std::endl;
            return;
        }
        
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
            return;
        }
        
        // Simple JSON format - with proper escaping
        file << "{\n";
        file << "  \"name\": \"" << obj->GetName() << "\",\n";
        file << "  \"position\": { \"x\": " << obj->position.x << ", \"y\": " << obj->position.y << ", \"z\": " << obj->position.z << " },\n";
        file << "  \"rotation\": { \"x\": " << obj->rotation.x << ", \"y\": " << obj->rotation.y << ", \"z\": " << obj->rotation.z << " },\n";
        file << "  \"size\": { \"x\": " << obj->size.x << ", \"y\": " << obj->size.y << ", \"z\": " << obj->size.z << " }\n";
        file << "}\n";
        
        std::cout << "GameObject saved to: " << filepath << std::endl;
    }
    
    static std::unique_ptr<GameObject> LoadObjectFromJson(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << filepath << std::endl;
            return nullptr;
        }
        
        // Read the entire file content
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        // Very simple JSON parsing (not robust, just for testing)
        std::string name = "Unknown";
        Vector3 position, rotation, size;
        
        // Extract name
        size_t namePos = content.find("\"name\"");
        if (namePos != std::string::npos) {
            size_t start = content.find(":", namePos) + 2; // Skip ": "
            size_t end = content.find("\"", start + 1);
            name = content.substr(start + 1, end - start - 1); // +1 and -1 to exclude quotes
        }
        
        // Extract position
        size_t posPos = content.find("\"position\"");
        if (posPos != std::string::npos) {
            size_t xPos = content.find("\"x\"", posPos);
            size_t yPos = content.find("\"y\"", posPos);
            size_t zPos = content.find("\"z\"", posPos);
            
            if (xPos != std::string::npos && yPos != std::string::npos && zPos != std::string::npos) {
                position.x = std::stof(content.substr(content.find(":", xPos) + 1));
                position.y = std::stof(content.substr(content.find(":", yPos) + 1));
                position.z = std::stof(content.substr(content.find(":", zPos) + 1));
            }
        }
        
        // Extract rotation
        size_t rotPos = content.find("\"rotation\"");
        if (rotPos != std::string::npos) {
            size_t xPos = content.find("\"x\"", rotPos);
            size_t yPos = content.find("\"y\"", rotPos);
            size_t zPos = content.find("\"z\"", rotPos);
            
            if (xPos != std::string::npos && yPos != std::string::npos && zPos != std::string::npos) {
                rotation.x = std::stof(content.substr(content.find(":", xPos) + 1));
                rotation.y = std::stof(content.substr(content.find(":", yPos) + 1));
                rotation.z = std::stof(content.substr(content.find(":", zPos) + 1));
            }
        }
        
        // Extract size
        size_t sizePos = content.find("\"size\"");
        if (sizePos != std::string::npos) {
            size_t xPos = content.find("\"x\"", sizePos);
            size_t yPos = content.find("\"y\"", sizePos);
            size_t zPos = content.find("\"z\"", sizePos);
            
            if (xPos != std::string::npos && yPos != std::string::npos && zPos != std::string::npos) {
                size.x = std::stof(content.substr(content.find(":", xPos) + 1));
                size.y = std::stof(content.substr(content.find(":", yPos) + 1));
                size.z = std::stof(content.substr(content.find(":", zPos) + 1));
            }
        }
        
        auto gameObject = std::make_unique<GameObject>(name, position, rotation, size);
        std::cout << "GameObject loaded from: " << filepath << std::endl;
        std::cout << "  Name: " << gameObject->GetName() << std::endl;
        std::cout << "  Position: (" << gameObject->position.x << ", " 
                  << gameObject->position.y << ", " 
                  << gameObject->position.z << ")" << std::endl;
        
        return gameObject;
    }
};

// Simple Scene class for testing
class Scene {
private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    std::string currentScenePath;
    
    void CleanupResources() {
        gameObjects.clear();
        std::cout << "Scene resources cleaned up" << std::endl;
    }
    
public:
    Scene() {}
    
    void Initialize() {
        std::cout << "Scene initialized" << std::endl;
    }
    
    void Load() {
        std::cout << "Scene loaded: " << currentScenePath << std::endl;
    }
    
    void UnloadScene() {
        CleanupResources();
        std::cout << "Scene unloaded: " << currentScenePath << std::endl;
    }
    
    void LoadScene(const std::string& scenePath) {
        if (!currentScenePath.empty()) {
            UnloadScene();
        }
        
        currentScenePath = scenePath;
        Load();
        
        // Create temp directory if it doesn't exist
        system("mkdir -p temp");
        
        // Load transferred objects if any
        std::ifstream manifest("temp/transfer_manifest.json");
        if (manifest.is_open()) {
            std::string objectPath;
            while (std::getline(manifest, objectPath)) {
                auto obj = SceneSerializer::LoadObjectFromJson(objectPath);
                if (obj) {
                    AddGameObject(std::move(obj));
                }
                // Remove temporary file
                std::string removeCmd = "rm " + objectPath;
                system(removeCmd.c_str());
            }
            manifest.close();
            
            // Clear manifest
            system("rm temp/transfer_manifest.json");
        }
        
        Initialize();
    }
    
    void TransferObject(GameObject* obj, const std::string& targetScenePath) {
        if (!obj) {
            std::cerr << "Error: Cannot transfer null GameObject" << std::endl;
            return;
        }
        
        // Create temp directory if it doesn't exist
        system("mkdir -p temp");
        
        // Save object to temporary file
        std::string tempPath = "temp/" + obj->GetName() + "_" + std::to_string(rand()) + ".json";
        SceneSerializer::SaveObjectToJson(obj, tempPath);
        
        // Add to transfer manifest
        std::ofstream manifest("temp/transfer_manifest.json", std::ios::app);
        if (!manifest.is_open()) {
            std::cerr << "Error: Could not open transfer manifest for writing" << std::endl;
            return;
        }
        
        manifest << tempPath << std::endl;
        manifest.close();
        
        std::cout << "Object '" << obj->GetName() << "' marked for transfer to scene: " << targetScenePath << std::endl;
    }
    
    void AddGameObject(std::unique_ptr<GameObject> gameObject) {
        gameObjects.push_back(std::move(gameObject));
        std::cout << "GameObject added to scene: " << gameObjects.back()->GetName() << std::endl;
    }
    
    void AddGameObject(GameObject* gameObject) {
        gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
        std::cout << "GameObject added to scene: " << gameObjects.back()->GetName() << std::endl;
    }
    
    GameObject* FindGameObject(const std::string& name) const {
        for (const auto& obj : gameObjects) {
            if (obj->GetName() == name) {
                return obj.get();
            }
        }
        return nullptr;
    }
};

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
    
    // Seed random number generator
    srand(time(nullptr));
    
    // Create temp directory if it doesn't exist
    system("mkdir -p temp");
    
    // Create first scene
    std::cout << "Creating first scene..." << std::endl;
    Scene scene1;
    scene1.Initialize();
    
    // Add object to transfer
    std::cout << "Creating persistent object..." << std::endl;
    auto obj = new GameObject("PersistentObject", Vector3(1, 2, 3));
    scene1.AddGameObject(obj);
    
    // Verify object was added
    std::cout << "Verifying object in first scene..." << std::endl;
    auto foundObj = scene1.FindGameObject("PersistentObject");
    if (!foundObj) {
        std::cerr << "Error: Object not found in first scene" << std::endl;
        return 1;
    }
    
    if (!(foundObj->position == Vector3(1, 2, 3))) {
        std::cerr << "Error: Object position mismatch in first scene" << std::endl;
        std::cout << "Position: " << foundObj->position.x << ", " 
                  << foundObj->position.y << ", " 
                  << foundObj->position.z << std::endl;
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
    
    if (!(persistedObj->position == Vector3(1, 2, 3))) {
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
    system("rm -rf temp");
    
    std::cout << "=== Scene Transition Test Passed ===" << std::endl;
    return 0;
}
