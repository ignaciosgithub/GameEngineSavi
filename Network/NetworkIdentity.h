#ifndef NETWORK_IDENTITY_H
#define NETWORK_IDENTITY_H

#include <string>
#include <cstdint>

namespace Network {
    class NetworkIdentity {
    private:
        uint32_t networkId;
        std::string objectName;
        bool isLocalPlayer;
        
    public:
        NetworkIdentity() : networkId(0), isLocalPlayer(false) {}
        
        // Getters
        uint32_t GetNetworkId() const { return networkId; }
        std::string GetObjectName() const { return objectName; }
        bool IsLocalPlayer() const { return isLocalPlayer; }
        
        // Setters
        void SetNetworkId(uint32_t id) { networkId = id; }
        void SetObjectName(const std::string& name) { objectName = name; }
        void SetIsLocalPlayer(bool local) { isLocalPlayer = local; }
    };
}

#endif // NETWORK_IDENTITY_H
