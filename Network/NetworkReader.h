#ifndef NETWORK_READER_H
#define NETWORK_READER_H

#include "Packet.h"
#include <string>

namespace Network {
    class NetworkReader {
    private:
        const Packet& packet;
        mutable size_t readPosition;
        
    public:
        NetworkReader(const Packet& packet) : packet(packet), readPosition(0) {}
        
        // Read methods for common types
        bool Read(bool& value) { return packet.Read(value, readPosition); }
        bool Read(int8_t& value) { return packet.Read(value, readPosition); }
        bool Read(uint8_t& value) { return packet.Read(value, readPosition); }
        bool Read(int16_t& value) { return packet.Read(value, readPosition); }
        bool Read(uint16_t& value) { return packet.Read(value, readPosition); }
        bool Read(int32_t& value) { return packet.Read(value, readPosition); }
        bool Read(uint32_t& value) { return packet.Read(value, readPosition); }
        bool Read(float& value) { return packet.Read(value, readPosition); }
        bool Read(double& value) { return packet.Read(value, readPosition); }
        bool Read(std::string& value) { return packet.Read(value, readPosition); }
        
        // Read raw data
        bool ReadBytes(void* data, size_t sizeInBytes) {
            return packet.Read(data, sizeInBytes, readPosition);
        }
        
        // Get the underlying packet
        const Packet& GetPacket() const { return packet; }
        
        // Get/set read position
        size_t GetReadPosition() const { return readPosition; }
        void SetReadPosition(size_t position) { readPosition = position; }
    };
}

#endif // NETWORK_READER_H
