#include "NetworkDebugger.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace Network {

    NetworkDebugger::NetworkDebugger()
        : enabled(false), maxPacketHistory(1000),
          simulatedLatency(0.0f), simulatedPacketLoss(0.0f),
          totalPacketsSent(0), totalPacketsReceived(0),
          totalBytesSent(0), totalBytesReceived(0) {
        
        // Initialize connection quality metrics
        connectionQuality.latency = 0.0f;
        connectionQuality.packetLoss = 0.0f;
        connectionQuality.jitter = 0.0f;
        connectionQuality.bandwidth = 0.0f;
    }

    NetworkDebugger::~NetworkDebugger() {
        // Clean up resources if needed
    }

    void NetworkDebugger::Enable(bool enable) {
        enabled = enable;
        std::cout << "Network debugger " << (enable ? "enabled" : "disabled") << std::endl;
        
        if (!enable) {
            // Clear history when disabling to free memory
            packetHistory.clear();
        }
    }

    void NetworkDebugger::LogPacket(const Packet& packet, bool isSending) {
        if (!enabled) {
            return;
        }
        
        // Create packet info
        PacketInfo info;
        info.timestamp = std::chrono::system_clock::now();
        info.type = packet.GetType();
        info.size = packet.GetData().size();
        info.senderAddress = packet.GetSenderAddress();
        info.senderPort = packet.GetSenderPort();
        info.isSending = isSending;
        info.sequenceNumber = packet.GetSequenceNumber();
        
        // Add to history
        packetHistory.push_back(info);
        
        // Trim history if it gets too large
        if (packetHistory.size() > maxPacketHistory) {
            packetHistory.pop_front();
        }
        
        // Update statistics
        if (isSending) {
            totalPacketsSent++;
            totalBytesSent += info.size;
        } else {
            totalPacketsReceived++;
            totalBytesReceived += info.size;
        }
        
        // Print packet info
        std::cout << "[" << (isSending ? "SEND" : "RECV") << "] ";
        std::cout << "Type: ";
        
        switch (info.type) {
            case PacketType::CONNECT:
                std::cout << "CONNECT";
                break;
            case PacketType::DISCONNECT:
                std::cout << "DISCONNECT";
                break;
            case PacketType::DATA:
                std::cout << "DATA";
                break;
            case PacketType::PING:
                std::cout << "PING";
                break;
            case PacketType::PONG:
                std::cout << "PONG";
                break;
            case PacketType::HEARTBEAT:
                std::cout << "HEARTBEAT";
                break;
            case PacketType::CUSTOM:
                std::cout << "CUSTOM";
                break;
            default:
                std::cout << "UNKNOWN";
                break;
        }
        
        std::cout << ", Size: " << info.size << " bytes";
        std::cout << ", Seq: " << info.sequenceNumber;
        
        if (!info.senderAddress.empty()) {
            std::cout << ", From: " << info.senderAddress << ":" << info.senderPort;
        }
        
        std::cout << std::endl;
    }

    void NetworkDebugger::UpdateLatencySimulation(float milliseconds) {
        simulatedLatency = milliseconds;
        std::cout << "Simulated latency set to " << milliseconds << " ms" << std::endl;
    }

    void NetworkDebugger::UpdatePacketLossSimulation(float percentage) {
        simulatedPacketLoss = std::max(0.0f, std::min(100.0f, percentage));
        std::cout << "Simulated packet loss set to " << simulatedPacketLoss << "%" << std::endl;
    }

    void NetworkDebugger::DisplayStats() const {
        if (!enabled) {
            std::cout << "Network debugger is disabled. Enable it to see statistics." << std::endl;
            return;
        }
        
        std::cout << "===== Network Statistics =====" << std::endl;
        std::cout << "Packets sent: " << totalPacketsSent << std::endl;
        std::cout << "Packets received: " << totalPacketsReceived << std::endl;
        std::cout << "Bytes sent: " << totalBytesSent << std::endl;
        std::cout << "Bytes received: " << totalBytesReceived << std::endl;
        std::cout << std::endl;
        
        std::cout << "Connection Quality:" << std::endl;
        std::cout << "  Latency: " << connectionQuality.latency << " ms" << std::endl;
        std::cout << "  Packet Loss: " << connectionQuality.packetLoss << "%" << std::endl;
        std::cout << "  Jitter: " << connectionQuality.jitter << " ms" << std::endl;
        std::cout << "  Bandwidth: " << connectionQuality.bandwidth / 1024.0f << " KB/s" << std::endl;
        std::cout << std::endl;
        
        std::cout << "Simulation Settings:" << std::endl;
        std::cout << "  Simulated Latency: " << simulatedLatency << " ms" << std::endl;
        std::cout << "  Simulated Packet Loss: " << simulatedPacketLoss << "%" << std::endl;
        std::cout << "=============================" << std::endl;
    }

    void NetworkDebugger::Reset() {
        packetHistory.clear();
        totalPacketsSent = 0;
        totalPacketsReceived = 0;
        totalBytesSent = 0;
        totalBytesReceived = 0;
        
        connectionQuality.latency = 0.0f;
        connectionQuality.packetLoss = 0.0f;
        connectionQuality.jitter = 0.0f;
        connectionQuality.bandwidth = 0.0f;
        
        std::cout << "Network debugger statistics reset" << std::endl;
    }

} // namespace Network
