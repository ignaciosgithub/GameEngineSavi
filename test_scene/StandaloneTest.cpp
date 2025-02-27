#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Vector3 class for position and size
class Vector3 {
public:
    float x, y, z;
    
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
};

// Forward declarations
class MonoBehaviourLike;

// Base component class
class MonoBehaviourLike {
protected:
    bool enabled;
    
public:
    MonoBehaviourLike() : enabled(true) {}
    virtual ~MonoBehaviourLike() {}
    
    virtual void Start() {}
    virtual void Update(float deltaTime) {}
    virtual void OnDestroy() {}
    virtual void OnEnable() { enabled = true; }
    virtual void OnDisable() { enabled = false; }
    
    bool IsEnabled() const { return enabled; }
};

// TriggerVolume component
class TriggerVolume : public MonoBehaviourLike {
public:
    TriggerVolume() : MonoBehaviourLike() {}
    
    // Called when an object enters the trigger volume
    virtual void OnTriggerEnter() {
        std::cout << "TriggerVolume: Object entered trigger" << std::endl;
    }
    
    // Called when an object exits the trigger volume
    virtual void OnTriggerExit() {
        std::cout << "TriggerVolume: Object exited trigger" << std::endl;
    }
    
    // Called every frame while an object is in the trigger volume
    virtual void OnTriggerStay() {
        // Default implementation does nothing
    }
    
    // Override from MonoBehaviourLike
    void OnEnable() override {
        MonoBehaviourLike::OnEnable();
        std::cout << "TriggerVolume: Enabled" << std::endl;
    }
    
    void OnDisable() override {
        MonoBehaviourLike::OnDisable();
        std::cout << "TriggerVolume: Disabled" << std::endl;
    }
};

// InvisibleWall component
class InvisibleWall : public MonoBehaviourLike {
public:
    InvisibleWall() : MonoBehaviourLike() {}
    
    // Override from MonoBehaviourLike
    void OnEnable() override {
        MonoBehaviourLike::OnEnable();
        std::cout << "InvisibleWall: Enabled - Collisions will occur" << std::endl;
    }
    
    void OnDisable() override {
        MonoBehaviourLike::OnDisable();
        std::cout << "InvisibleWall: Disabled - Collisions will pass through" << std::endl;
    }
};

// Simple GameObject class
class GameObject {
private:
    std::string name;
    std::vector<std::shared_ptr<MonoBehaviourLike>> components;
    
public:
    Vector3 position;
    Vector3 size;
    
    GameObject(const std::string& name) : name(name), position(0,0,0), size(1,1,1) {}
    
    // Add a component
    template<typename T>
    std::shared_ptr<T> AddComponent() {
        auto component = std::make_shared<T>();
        components.push_back(component);
        return component;
    }
    
    // Add a component from raw pointer
    template<typename T>
    T* AddComponent(T* component) {
        components.push_back(std::shared_ptr<MonoBehaviourLike>(component));
        return component;
    }
    
    // Get components of a specific type
    template<typename T>
    std::vector<std::shared_ptr<T>> GetComponents() {
        std::vector<std::shared_ptr<T>> result;
        for (auto& component : components) {
            std::shared_ptr<T> derived = std::dynamic_pointer_cast<T>(component);
            if (derived) {
                result.push_back(derived);
            }
        }
        return result;
    }
    
    // Get the name
    std::string GetName() const { return name; }
};

// Custom trigger volume that prints messages
class CustomTriggerVolume : public TriggerVolume {
public:
    void OnTriggerEnter() override {
        std::cout << "CustomTriggerVolume: Object entered trigger!" << std::endl;
    }
    
    void OnTriggerExit() override {
        std::cout << "CustomTriggerVolume: Object exited trigger!" << std::endl;
    }
};

// Main function
int main() {
    std::cout << "Testing TriggerVolume and InvisibleWall components..." << std::endl;
    
    // Create a game object with a trigger volume
    GameObject triggerObject("TriggerObject");
    triggerObject.position = Vector3(0.0f, 0.0f, 0.0f);
    triggerObject.size = Vector3(1.0f, 1.0f, 1.0f);
    
    // Add a custom trigger volume component
    auto trigger = triggerObject.AddComponent<CustomTriggerVolume>();
    
    std::cout << "Created TriggerVolume at position " 
              << triggerObject.position.x << ", " 
              << triggerObject.position.y << ", " 
              << triggerObject.position.z << std::endl;
    
    // Create a game object with an invisible wall
    GameObject wallObject("InvisibleWall");
    wallObject.position = Vector3(2.0f, 0.0f, 0.0f);
    wallObject.size = Vector3(0.1f, 2.0f, 2.0f);
    
    // Add an invisible wall component
    auto wall = wallObject.AddComponent<InvisibleWall>();
    
    std::cout << "Created InvisibleWall at position " 
              << wallObject.position.x << ", " 
              << wallObject.position.y << ", " 
              << wallObject.position.z << std::endl;
    
    // Test enabling/disabling the invisible wall
    std::cout << "\nTesting InvisibleWall component:" << std::endl;
    std::cout << "InvisibleWall enabled: " << (wall->IsEnabled() ? "Yes" : "No") << std::endl;
    
    std::cout << "Disabling InvisibleWall..." << std::endl;
    wall->OnDisable();
    
    std::cout << "InvisibleWall enabled: " << (wall->IsEnabled() ? "Yes" : "No") << std::endl;
    
    std::cout << "Enabling InvisibleWall..." << std::endl;
    wall->OnEnable();
    
    std::cout << "InvisibleWall enabled: " << (wall->IsEnabled() ? "Yes" : "No") << std::endl;
    
    // Test enabling/disabling the trigger volume
    std::cout << "\nTesting TriggerVolume component:" << std::endl;
    std::cout << "TriggerVolume enabled: " << (trigger->IsEnabled() ? "Yes" : "No") << std::endl;
    
    std::cout << "Disabling TriggerVolume..." << std::endl;
    trigger->OnDisable();
    
    std::cout << "TriggerVolume enabled: " << (trigger->IsEnabled() ? "Yes" : "No") << std::endl;
    
    std::cout << "Enabling TriggerVolume..." << std::endl;
    trigger->OnEnable();
    
    std::cout << "TriggerVolume enabled: " << (trigger->IsEnabled() ? "Yes" : "No") << std::endl;
    
    // Simulate trigger events
    std::cout << "\nSimulating trigger events:" << std::endl;
    trigger->OnTriggerEnter();
    trigger->OnTriggerStay();
    trigger->OnTriggerExit();
    
    std::cout << "\nTest completed successfully!" << std::endl;
    return 0;
}
