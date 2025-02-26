#include "NetworkSystem_impl.h" // Include the implementation header instead
#include "NetworkDebugger.h"
#include "../EngineCondition.h"
#include <iostream>

namespace Network {

    NetworkSystem::NetworkSystem()
        : isServer(false), isPeerToPeer(false), packetLoggingEnabled(false),
          simulatedLatency(0.0f), simulatedPacketLoss(0.0f) {
    }

    NetworkSystem::~NetworkSystem() {
        Cleanup();
    }

    void NetworkSystem::Initialize(bool asServer, bool asPeerToPeer) {
        isServer = asServer;
        isPeerToPeer = asPeerToPeer;
        
        // Set the engine condition to server build if we're initializing as a server
        if (isServer) {
            EngineCondition::SetState(EngineCondition::State::SERVER_BUILD_STATE);
        }
        
        std::cout << "Initializing NetworkSystem as " 
                  << (isServer ? "server" : "client") 
                  << " in " 
                  << (isPeerToPeer ? "peer-to-peer" : "client-server") 
                  << " mode" << std::endl;
                  
        // Initialize platform-specific networking
#ifdef PLATFORM_WINDOWS
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock" << std::endl;
            return;
        }
#endif
    }

    void NetworkSystem::Update() {
        // Process incoming packets
        // Update connections
        // Handle timeouts
    }

    void NetworkSystem::Cleanup() {
        // Close all connections
        connections.clear();
        
        // Cleanup platform-specific networking
#ifdef PLATFORM_WINDOWS
        WSACleanup();
#endif
        
        std::cout << "NetworkSystem cleaned up" << std::endl;
    }

    void NetworkSystem::Connect(const std::string& address, int port) {
        std::cout << "Connecting to " << address << ":" << port << std::endl;
        // Create a new connection
        connections.push_back(std::make_unique<Connection>(address, port));
        connections.back()->Connect();
    }

    void NetworkSystem::Disconnect() {
        std::cout << "Disconnecting from all servers/peers" << std::endl;
        // Close all connections
        connections.clear();
    }

    void NetworkSystem::StartServer(int port) {
        if (!isServer) {
            std::cout << "Warning: StartServer called but not initialized as server" << std::endl;
            isServer = true;
            EngineCondition::SetState(EngineCondition::State::SERVER_BUILD_STATE);
        }
        
        std::cout << "Starting server on port " << port << std::endl;
        // Initialize server socket
        // Start listening for connections
    }

    void NetworkSystem::StartClient() {
        if (isServer) {
            std::cout << "Warning: StartClient called but initialized as server" << std::endl;
            isServer = false;
        }
        
        if (isPeerToPeer) {
            std::cout << "Warning: StartClient called but initialized as peer-to-peer" << std::endl;
            isPeerToPeer = false;
        }
        
        std::cout << "Starting client" << std::endl;
        // Initialize client
    }

    void NetworkSystem::StartPeer() {
        if (!isPeerToPeer) {
            std::cout << "Warning: StartPeer called but not initialized as peer-to-peer" << std::endl;
            isPeerToPeer = true;
        }
        
        std::cout << "Starting peer" << std::endl;
        // Initialize peer
    }

    void NetworkSystem::EnablePacketLogging(bool enable) {
        packetLoggingEnabled = enable;
        std::cout << "Packet logging " << (enable ? "enabled" : "disabled") << std::endl;
    }

    void NetworkSystem::SimulateLatency(float milliseconds) {
        simulatedLatency = milliseconds;
        std::cout << "Simulating " << milliseconds << "ms latency" << std::endl;
    }

    void NetworkSystem::SimulatePacketLoss(float percentage) {
        simulatedPacketLoss = percentage;
        std::cout << "Simulating " << percentage << "% packet loss" << std::endl;
    }
    
    void NetworkSystem::DisplayDebugStats() const {
        std::cout << "===== Network System Status =====" << std::endl;
        std::cout << "Mode: " << (isServer ? "Server" : (isPeerToPeer ? "Peer-to-Peer" : "Client")) << std::endl;
        std::cout << "Active connections: " << connections.size() << std::endl;
        std::cout << "Packet logging: " << (packetLoggingEnabled ? "Enabled" : "Disabled") << std::endl;
        std::cout << "Simulated latency: " << simulatedLatency << " ms" << std::endl;
        std::cout << "Simulated packet loss: " << simulatedPacketLoss << "%" << std::endl;
        std::cout << "===============================" << std::endl;
    }

} // namespace Network
