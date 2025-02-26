#ifndef NETWORK_SYSTEM_H
#define NETWORK_SYSTEM_H

#include "../platform.h"
#include <vector>
#include <memory>
#include <string>

namespace Network {
    // Forward declarations
    class Connection;
    class Packet;
    
    class NetworkSystem {
    private:
        bool isServer;
        bool isPeerToPeer;
        std::vector<std::unique_ptr<Connection>> connections;
        bool packetLoggingEnabled;
        float simulatedLatency;
        float simulatedPacketLoss;
        
    public:
        NetworkSystem();
        ~NetworkSystem();
        
        // Initialization and shutdown
        void Initialize(bool asServer, bool asPeerToPeer);
        void Update();
        void Shutdown();
        
        // Connection management
        void Connect(const std::string& address, int port);
        void Disconnect();
        
        // Networking modes
        void StartServer(int port);
        void StartClient();
        void StartPeer();
        
        // Debug features
        void EnablePacketLogging(bool enable);
        void SimulateLatency(float milliseconds);
        void SimulatePacketLoss(float percentage);
        
        // Getters
        bool IsServer() const { return isServer; }
        bool IsPeerToPeer() const { return isPeerToPeer; }
        bool IsPacketLoggingEnabled() const { return packetLoggingEnabled; }
        float GetSimulatedLatency() const { return simulatedLatency; }
        float GetSimulatedPacketLoss() const { return simulatedPacketLoss; }
    };
}

#endif // NETWORK_SYSTEM_H
