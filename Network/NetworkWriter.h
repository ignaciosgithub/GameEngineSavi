#ifndef NETWORK_WRITER_H
#define NETWORK_WRITER_H

#include "Packet.h"
#include <string>

namespace Network {
    class NetworkWriter {
    private:
        Packet& packet;
        
    public:
        NetworkWriter(Packet& packet) : packet(packet) {}
        
        // Write methods for common types
        void Write(bool value) { packet.Append(value); }
        void Write(int8_t value) { packet.Append(value); }
        void Write(uint8_t value) { packet.Append(value); }
        void Write(int16_t value) { packet.Append(value); }
        void Write(uint16_t value) { packet.Append(value); }
        void Write(int32_t value) { packet.Append(value); }
        void Write(uint32_t value) { packet.Append(value); }
        void Write(float value) { packet.Append(value); }
        void Write(double value) { packet.Append(value); }
        void Write(const std::string& value) { packet.Append(value); }
        
        // Write raw data
        void WriteBytes(const void* data, size_t sizeInBytes) {
            packet.Append(data, sizeInBytes);
        }
        
        // Get the underlying packet
        Packet& GetPacket() { return packet; }
    };
}

#endif // NETWORK_WRITER_H
