#ifndef SCENE_SERIALIZER_H
#define SCENE_SERIALIZER_H

#include <string>
#include <memory>
#include "GameObject.h"
#include "Vector3.h"
#include "PointLight.h"
#include "ThirdParty/json/json.hpp"

/**
 * @class SceneSerializer
 * @brief Handles serialization and deserialization of scene objects to/from JSON
 * 
 * This class provides static methods for saving game objects to JSON files and loading
 * them back, enabling persistence between scene transitions.
 */
class SceneSerializer {
public:
    /**
     * @brief Saves a GameObject to a JSON file
     * @param obj The GameObject to serialize
     * @param filepath The path where the JSON file will be saved
     */
    static void SaveObjectToJson(const GameObject* obj, const std::string& filepath);
    
    /**
     * @brief Loads a GameObject from a JSON file
     * @param filepath The path to the JSON file
     * @return A pointer to the loaded GameObject, or nullptr if loading failed
     */
    static GameObject* LoadObjectFromJson(const std::string& filepath);
    
private:
    /**
     * @brief Serializes a Vector3 to JSON
     * @param vec The Vector3 to serialize
     * @return JSON object representing the Vector3
     */
    static nlohmann::json SerializeVector3(const Vector3& vec);
    
    /**
     * @brief Deserializes a Vector3 from JSON
     * @param json The JSON object containing Vector3 data
     * @return The deserialized Vector3
     */
    static Vector3 DeserializeVector3(const nlohmann::json& json);
    
    /**
     * @brief Serializes a PointLight to JSON
     * @param light The PointLight to serialize
     * @return JSON object representing the PointLight
     */
    static nlohmann::json SerializePointLight(const PointLight& light);
    
    /**
     * @brief Deserializes a PointLight from JSON
     * @param json The JSON object containing PointLight data
     * @return The deserialized PointLight
     */
    static PointLight DeserializePointLight(const nlohmann::json& json);
};

#endif // SCENE_SERIALIZER_H
