#include "Packet.h"
#include <cstring>
#include <iostream>

namespace Network {

    Packet::Packet(PacketType type)
        : type(type), senderPort(0), sequenceNumber(0), reliable(false) {
    }

    Packet::~Packet() {
        Clear();
    }

    void Packet::Clear() {
        data.clear();
    }

    void Packet::Append(const void* data, size_t sizeInBytes) {
        if (data && sizeInBytes > 0) {
            size_t currentSize = this->data.size();
            this->data.resize(currentSize + sizeInBytes);
            std::memcpy(this->data.data() + currentSize, data, sizeInBytes);
        }
    }

    void Packet::Append(bool value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(int8_t value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(uint8_t value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(int16_t value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(uint16_t value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(int32_t value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(uint32_t value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(float value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(double value) {
        Append(&value, sizeof(value));
    }

    void Packet::Append(const std::string& value) {
        // First append the size of the string
        uint32_t size = static_cast<uint32_t>(value.size());
        Append(size);
        
        // Then append the string data
        if (size > 0) {
            Append(value.data(), size);
        }
    }

    bool Packet::Read(void* data, size_t sizeInBytes, size_t& readPos) const {
        if (readPos + sizeInBytes <= this->data.size()) {
            std::memcpy(data, this->data.data() + readPos, sizeInBytes);
            readPos += sizeInBytes;
            return true;
        }
        
        return false;
    }

    bool Packet::Read(bool& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(int8_t& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(uint8_t& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(int16_t& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(uint16_t& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(int32_t& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(uint32_t& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(float& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(double& value, size_t& readPos) const {
        return Read(&value, sizeof(value), readPos);
    }

    bool Packet::Read(std::string& value, size_t& readPos) const {
        // First read the size of the string
        uint32_t size;
        if (!Read(size, readPos)) {
            return false;
        }
        
        // Then read the string data
        value.clear();
        if (size > 0) {
            if (readPos + size <= this->data.size()) {
                value.assign(reinterpret_cast<const char*>(this->data.data() + readPos), size);
                readPos += size;
                return true;
            }
            return false;
        }
        
        return true;
    }

} // namespace Network
