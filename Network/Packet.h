#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <string>
#include <cstdint>

namespace Network {
    enum class PacketType {
        CONNECT,
        DISCONNECT,
        DATA,
        PING,
        PONG,
        HEARTBEAT,
        CUSTOM
    };
    
    class Packet {
    private:
        PacketType type;
        std::vector<uint8_t> data;
        std::string senderAddress;
        int senderPort;
        uint32_t sequenceNumber;
        bool reliable;
        
    public:
        Packet(PacketType type = PacketType::DATA);
        ~Packet();
        
        // Getters
        PacketType GetType() const { return type; }
        const std::vector<uint8_t>& GetData() const { return data; }
        std::string GetSenderAddress() const { return senderAddress; }
        int GetSenderPort() const { return senderPort; }
        uint32_t GetSequenceNumber() const { return sequenceNumber; }
        bool IsReliable() const { return reliable; }
        
        // Setters
        void SetType(PacketType newType) { type = newType; }
        void SetData(const std::vector<uint8_t>& newData) { data = newData; }
        void SetSenderAddress(const std::string& address) { senderAddress = address; }
        void SetSenderPort(int port) { senderPort = port; }
        void SetSequenceNumber(uint32_t number) { sequenceNumber = number; }
        void SetReliable(bool isReliable) { reliable = isReliable; }
        
        // Data manipulation
        void Clear();
        void Append(const void* data, size_t sizeInBytes);
        
        // Specialized append methods for common types
        void Append(bool value);
        void Append(int8_t value);
        void Append(uint8_t value);
        void Append(int16_t value);
        void Append(uint16_t value);
        void Append(int32_t value);
        void Append(uint32_t value);
        void Append(float value);
        void Append(double value);
        void Append(const std::string& value);
        
        // Reading data
        bool Read(void* data, size_t sizeInBytes, size_t& readPos) const;
        
        // Specialized read methods for common types
        bool Read(bool& value, size_t& readPos) const;
        bool Read(int8_t& value, size_t& readPos) const;
        bool Read(uint8_t& value, size_t& readPos) const;
        bool Read(int16_t& value, size_t& readPos) const;
        bool Read(uint16_t& value, size_t& readPos) const;
        bool Read(int32_t& value, size_t& readPos) const;
        bool Read(uint32_t& value, size_t& readPos) const;
        bool Read(float& value, size_t& readPos) const;
        bool Read(double& value, size_t& readPos) const;
        bool Read(std::string& value, size_t& readPos) const;
    };
}

#endif // PACKET_H
