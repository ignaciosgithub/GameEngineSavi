#include "../Network/NetworkSystem.h"
#include "../Network/NetworkDebugger.h"
#include "../Network/Packet.h"
#include "../EngineCondition.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <csignal>

// Global flag for handling Ctrl+C
volatile sig_atomic_t running = 1;

// Signal handler for Ctrl+C
void signalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nReceived Ctrl+C, shutting down..." << std::endl;
        running = 0;
    }
}

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --server         Run in server mode" << std::endl;
    std::cout << "  --client <addr>  Run in client mode and connect to server at <addr>" << std::endl;
    std::cout << "  --p2p            Run in peer-to-peer mode" << std::endl;
    std::cout << "  --port <port>    Specify port (default: 7777)" << std::endl;
    std::cout << "  --debug          Enable network debugging" << std::endl;
    std::cout << "  --help           Display this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    // Register signal handler for Ctrl+C
    std::signal(SIGINT, signalHandler);
    
    // Default values
    bool isServer = false;
    bool isPeerToPeer = false;
    bool enableDebug = false;
    std::string serverAddress = "127.0.0.1";
    int port = 7777;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "--server") {
            isServer = true;
        } else if (arg == "--client" && i + 1 < argc) {
            isServer = false;
            isPeerToPeer = false;
            serverAddress = argv[++i];
        } else if (arg == "--p2p") {
            isPeerToPeer = true;
            isServer = false;
        } else if (arg == "--port" && i + 1 < argc) {
            port = std::atoi(argv[++i]);
        } else if (arg == "--debug") {
            enableDebug = true;
        } else if (arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    // Set the engine condition to SERVER_BUILD if running as server
    if (isServer) {
        EngineCondition::SetState(EngineCondition::SERVER_BUILD);
    } else {
        EngineCondition::SetState(EngineCondition::DEBUG_BUILD);
    }
    
    // Initialize networking system
    Network::NetworkSystem netSystem;
    netSystem.Initialize(isServer, isPeerToPeer);
    
    // Enable packet logging if debug is enabled
    if (enableDebug) {
        netSystem.EnablePacketLogging(true);
    }
    
    // Start networking based on mode
    if (isServer) {
        std::cout << "Starting server on port " << port << "..." << std::endl;
        netSystem.StartServer(port);
    } else if (isPeerToPeer) {
        std::cout << "Starting in peer-to-peer mode..." << std::endl;
        netSystem.StartPeer();
    } else {
        std::cout << "Starting client and connecting to " << serverAddress << ":" << port << "..." << std::endl;
        netSystem.Connect(serverAddress, port);
    }
    
    // Main loop
    std::cout << "Network demo running. Press Ctrl+C to exit." << std::endl;
    std::cout << "Mode: " << (isServer ? "Server" : (isPeerToPeer ? "Peer-to-Peer" : "Client")) << std::endl;
    
    // Create a ping packet
    Network::Packet pingPacket(Network::PacketType::PING);
    pingPacket.Append(std::string("PING"));
    
    // Counter for periodic actions
    int counter = 0;
    
    while (running) {
        // Update network system
        netSystem.Update();
        
        // Every 2 seconds, perform some action
        if (counter % 20 == 0) {
            if (!isServer) {
                // Clients and peers send ping packets
                std::cout << "Sending ping packet..." << std::endl;
                // This would normally use the Connection class to send
                // For demo purposes, we're just showing the packet creation
            } else {
                // Server prints status
                std::cout << "Server listening for connections..." << std::endl;
            }
            
            // If debug is enabled, display network statistics
            if (enableDebug) {
                netSystem.DisplayDebugStats();
            }
        }
        
        // Sleep for 100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        counter++;
    }
    
    // Shutdown networking
    std::cout << "Shutting down network system..." << std::endl;
    netSystem.Shutdown();
    
    return 0;
}
