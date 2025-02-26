#ifndef CONNECTION_H
#define CONNECTION_H

#include "../platform.h"
#include <string>
#include <vector>
#include <memory>

namespace Network {
    // Forward declarations
    class Packet;
    
    class Connection {
    private:
        std::string address;
        int port;
        bool isConnected;
        
        // Platform-specific socket handle
#ifdef PLATFORM_WINDOWS
        SOCKET socketHandle;
#else
        int socketHandle;
#endif
        
    public:
        Connection(const std::string& address, int port);
        ~Connection();
        
        // Connection management
        bool Connect();
        void Disconnect();
        bool IsConnected() const { return isConnected; }
        
        // Data transmission
        bool Send(const Packet& packet);
        bool Receive(Packet& packet);
        
        // Getters
        std::string GetAddress() const { return address; }
        int GetPort() const { return port; }
    };
}

#endif // CONNECTION_H
