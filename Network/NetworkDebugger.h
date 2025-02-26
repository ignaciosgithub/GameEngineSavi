#ifndef NETWORK_DEBUGGER_H
#define NETWORK_DEBUGGER_H

#include "NetworkSystem.h"
#include "Packet.h"
#include <vector>
#include <string>
#include <chrono>
#include <deque>

namespace Network {
    // Structure to store packet information for debugging
    struct PacketInfo {
        std::chrono::system_clock::time_point timestamp;
        PacketType type;
        size_t size;
        std::string senderAddress;
        int senderPort;
        bool isSending;
        uint32_t sequenceNumber;
    };
    
    // Structure to store connection quality metrics
    struct ConnectionQuality {
        float latency;          // Average round-trip time in milliseconds
        float packetLoss;       // Percentage of packets lost
        float jitter;           // Variation in latency
        float bandwidth;        // Estimated bandwidth in bytes per second
    };
    
    class NetworkDebugger {
    private:
        bool enabled;
        std::deque<PacketInfo> packetHistory;
        size_t maxPacketHistory;
        
        // Simulation settings
        float simulatedLatency;
        float simulatedPacketLoss;
        
        // Connection quality metrics
        ConnectionQuality connectionQuality;
        
        // Statistics
        uint32_t totalPacketsSent;
        uint32_t totalPacketsReceived;
        uint32_t totalBytesSent;
        uint32_t totalBytesReceived;
        
    public:
        NetworkDebugger();
        ~NetworkDebugger();
        
        // Enable/disable the debugger
        void Enable(bool enable);
        bool IsEnabled() const { return enabled; }
        
        // Log packet information
        void LogPacket(const Packet& packet, bool isSending);
        
        // Simulation controls
        void UpdateLatencySimulation(float milliseconds);
        void UpdatePacketLossSimulation(float percentage);
        
        // Get simulation settings
        float GetSimulatedLatency() const { return simulatedLatency; }
        float GetSimulatedPacketLoss() const { return simulatedPacketLoss; }
        
        // Display statistics
        void DisplayStats() const;
        
        // Get connection quality
        const ConnectionQuality& GetConnectionQuality() const { return connectionQuality; }
        
        // Clear history and reset statistics
        void Reset();
        
        // Get packet history
        const std::deque<PacketInfo>& GetPacketHistory() const { return packetHistory; }
    };
}

#endif // NETWORK_DEBUGGER_H
