#include "NetworkComponent.h"
#include <iostream>

NetworkComponent::NetworkComponent() : isOwner(false) {
}

NetworkComponent::~NetworkComponent() {
    // Call network destroy event
    OnNetworkDestroy();
}

void NetworkComponent::OnNetworkStart() {
    // Default implementation does nothing
    // Derived classes should override this method to implement custom behavior
}

void NetworkComponent::OnNetworkDestroy() {
    // Default implementation does nothing
    // Derived classes should override this method to implement custom behavior
}

void NetworkComponent::OnOwnershipChanged(bool newOwner) {
    // Default implementation does nothing
    // Derived classes should override this method to implement custom behavior
    std::cout << "Ownership changed to " << (newOwner ? "owner" : "non-owner") << std::endl;
}

void NetworkComponent::Serialize(Network::NetworkWriter& writer) {
    // Default implementation does nothing
    // Derived classes should override this method to implement custom serialization
}

void NetworkComponent::Deserialize(Network::NetworkReader& reader) {
    // Default implementation does nothing
    // Derived classes should override this method to implement custom deserialization
}
