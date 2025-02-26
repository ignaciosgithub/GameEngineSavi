#ifndef NETWORK_COMPONENT_H
#define NETWORK_COMPONENT_H

#include "../MonoBehaviourLike.h"
#include "NetworkIdentity.h"
#include "NetworkWriter.h"
#include "NetworkReader.h"

class NetworkComponent : public MonoBehaviourLike {
private:
    Network::NetworkIdentity identity;
    bool isOwner;
    
public:
    NetworkComponent() : isOwner(false) {}
    virtual ~NetworkComponent() {}
    
    // Network lifecycle events
    virtual void OnNetworkStart() {}
    virtual void OnNetworkDestroy() {}
    virtual void OnOwnershipChanged(bool newOwner) {}
    
    // Serialization
    virtual void Serialize(Network::NetworkWriter& writer) {}
    virtual void Deserialize(Network::NetworkReader& reader) {}
    
    // Getters and setters
    Network::NetworkIdentity& GetIdentity() { return identity; }
    void SetIdentity(const Network::NetworkIdentity& newIdentity) { identity = newIdentity; }
    
    bool IsOwner() const { return isOwner; }
    void SetOwner(bool owner) { 
        if (owner != isOwner) {
            isOwner = owner;
            OnOwnershipChanged(isOwner);
        }
    }
};

#endif // NETWORK_COMPONENT_H
