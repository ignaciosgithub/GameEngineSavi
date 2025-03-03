#include "SceneSerializer.h"
#include <fstream>
#include <iostream>

void SceneSerializer::SaveObjectToJson(const GameObject* obj, const std::string& filepath) {
    if (!obj) {
        std::cerr << "Error: Cannot serialize null GameObject" << std::endl;
        return;
    }
    
    try {
        // Create JSON object
        nlohmann::json j;
        
        // Serialize basic properties
        j["name"] = obj->GetName();
        j["position"] = SerializeVector3(obj->position);
        j["rotation"] = SerializeVector3(obj->rotation);
        j["size"] = SerializeVector3(obj->size);
        
        // Serialize lights
        j["lights"] = nlohmann::json::array();
        for (const auto& light : obj->lights) {
            j["lights"].push_back(SerializePointLight(light));
        }
        
        // Save to file
        std::ofstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filepath << std::endl;
            return;
        }
        
        file << j.dump(4); // Pretty print with 4-space indentation
        std::cout << "GameObject saved to: " << filepath << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error serializing GameObject: " << e.what() << std::endl;
    }
}

GameObject* SceneSerializer::LoadObjectFromJson(const std::string& filepath) {
    try {
        // Open file
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for reading: " << filepath << std::endl;
            return nullptr;
        }
        
        // Parse JSON
        nlohmann::json j;
        file >> j;
        
        // Create GameObject
        std::string name = j["name"].get<std::string>();
        Vector3 position = DeserializeVector3(j["position"]);
        Vector3 rotation = DeserializeVector3(j["rotation"]);
        Vector3 size = DeserializeVector3(j["size"]);
        
        GameObject* gameObject = new GameObject(name, position, rotation, size);
        
        // Deserialize lights
        if (j.contains("lights") && j["lights"].is_array()) {
            for (const auto& lightJson : j["lights"]) {
                gameObject->AddLight(DeserializePointLight(lightJson));
            }
        }
        
        std::cout << "GameObject loaded from: " << filepath << std::endl;
        return gameObject;
    }
    catch (const std::exception& e) {
        std::cerr << "Error deserializing GameObject: " << e.what() << std::endl;
        return nullptr;
    }
}

nlohmann::json SceneSerializer::SerializeVector3(const Vector3& vec) {
    nlohmann::json j;
    j["x"] = vec.x;
    j["y"] = vec.y;
    j["z"] = vec.z;
    return j;
}

Vector3 SceneSerializer::DeserializeVector3(const nlohmann::json& json) {
    return Vector3(
        json["x"].get<float>(),
        json["y"].get<float>(),
        json["z"].get<float>()
    );
}

nlohmann::json SceneSerializer::SerializePointLight(const PointLight& light) {
    nlohmann::json j;
    j["position"] = SerializeVector3(light.position);
    j["color"] = SerializeVector3(light.color);
    j["intensity"] = light.intensity;
    j["range"] = light.range;
    return j;
}

PointLight SceneSerializer::DeserializePointLight(const nlohmann::json& json) {
    PointLight light;
    light.position = DeserializeVector3(json["position"]);
    light.color = DeserializeVector3(json["color"]);
    light.intensity = json["intensity"].get<float>();
    light.range = json["range"].get<float>();
    return light;
}
