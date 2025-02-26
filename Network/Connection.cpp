#include "Connection.h"
#include "Packet.h"
#include <iostream>

namespace Network {

    Connection::Connection(const std::string& address, int port)
        : address(address), port(port), isConnected(false) {
#ifdef PLATFORM_WINDOWS
        socketHandle = INVALID_SOCKET;
#else
        socketHandle = -1;
#endif
    }

    Connection::~Connection() {
        if (isConnected) {
            Disconnect();
        }
    }

    bool Connection::Connect() {
        std::cout << "Connecting to " << address << ":" << port << std::endl;
        
        // Platform-specific connection code
#ifdef PLATFORM_WINDOWS
        // Windows socket connection code
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &(serverAddr.sin_addr));
        
        // Create socket
        socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socketHandle == INVALID_SOCKET) {
            std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
            return false;
        }
        
        // Connect to server
        if (connect(socketHandle, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Failed to connect: " << WSAGetLastError() << std::endl;
            closesocket(socketHandle);
            socketHandle = INVALID_SOCKET;
            return false;
        }
#else
        // Linux socket connection code
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &(serverAddr.sin_addr));
        
        // Create socket
        socketHandle = socket(AF_INET, SOCK_STREAM, 0);
        if (socketHandle < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return false;
        }
        
        // Connect to server
        if (connect(socketHandle, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Failed to connect" << std::endl;
            close(socketHandle);
            socketHandle = -1;
            return false;
        }
#endif
        
        isConnected = true;
        return isConnected;
    }

    void Connection::Disconnect() {
        if (!isConnected) {
            return;
        }
        
        std::cout << "Disconnecting from " << address << ":" << port << std::endl;
        
        // Platform-specific disconnection code
#ifdef PLATFORM_WINDOWS
        closesocket(socketHandle);
        socketHandle = INVALID_SOCKET;
#else
        close(socketHandle);
        socketHandle = -1;
#endif
        
        isConnected = false;
    }

    bool Connection::Send(const Packet& packet) {
        if (!isConnected) {
            std::cerr << "Cannot send packet: not connected" << std::endl;
            return false;
        }
        
        std::cout << "Sending packet to " << address << ":" << port << std::endl;
        
        // Get packet data
        const std::vector<uint8_t>& data = packet.GetData();
        
        // Platform-specific send code
#ifdef PLATFORM_WINDOWS
        int bytesSent = send(socketHandle, reinterpret_cast<const char*>(data.data()), data.size(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Failed to send data: " << WSAGetLastError() << std::endl;
            return false;
        }
#else
        int bytesSent = send(socketHandle, reinterpret_cast<const char*>(data.data()), data.size(), 0);
        if (bytesSent < 0) {
            std::cerr << "Failed to send data" << std::endl;
            return false;
        }
#endif
        
        return true;
    }

    bool Connection::Receive(Packet& packet) {
        if (!isConnected) {
            return false;
        }
        
        // Buffer for receiving data
        uint8_t buffer[1024];
        
        // Platform-specific receive code
#ifdef PLATFORM_WINDOWS
        int bytesReceived = recv(socketHandle, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Failed to receive data: " << WSAGetLastError() << std::endl;
            return false;
        }
#else
        int bytesReceived = recv(socketHandle, reinterpret_cast<char*>(buffer), sizeof(buffer), 0);
        if (bytesReceived < 0) {
            std::cerr << "Failed to receive data" << std::endl;
            return false;
        }
#endif
        
        if (bytesReceived > 0) {
            // Create a new packet with the received data
            packet = Packet(PacketType::CUSTOM);
            packet.Append(buffer, bytesReceived);
            return true;
        }
        
        return false;
    }

} // namespace Network
