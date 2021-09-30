#ifndef ModRM_H
#define ModRM_H

#include <stdexcept>
#include <cstring>
#include "Utils.hpp"

class ModRM
{
private:
    uint8_t byte_value;
    const uint8_t *sequence;
    int *sequence_current_index;
    Storage *storage;

public:
    ModRM(const uint8_t *sequence, int *sequence_current_index, Storage *storage)
    {
        byte_value = sequence[(*sequence_current_index)++];
        this->sequence = sequence;
        this->sequence_current_index = sequence_current_index;
        this->storage = storage;
    };
    ~ModRM(){};

    struct StorageArgs getReg()
    {
        struct StorageArgs output;
        output.storage_type = R32;
        output.address = (int)((byte_value & ((uint8_t)0b00111000)) >> 3);
        return output;
    }

    struct StorageArgs getModRM()
    {
        uint8_t mod = (byte_value & ((uint8_t)0b11000000)) >> 6;
        uint8_t rm = byte_value & ((uint8_t)0b00000111);
        uint8_t reg_opcode = (byte_value & ((uint8_t)0b00111000)) >> 3;
        struct StorageArgs output;
        switch (mod)
        {
        case 0b00:
        {
            if (rm == 0b100 || rm == 0b101)
            {
                throw std::logic_error("rm=100 and rm=101 mode not implemented for mod=00");
            }

            output.storage_type = MEMORY;

            struct StorageArgs intermediate_storage_args;
            intermediate_storage_args.storage_type = R32;
            intermediate_storage_args.address = rm;
            output.address = this->storage->load<int32_t>(intermediate_storage_args);
            break;
        }

        case 0b01:
        {
            if (rm == 0b100)
            {
                throw std::logic_error("rm=100 mode not implemented for mod=01");
            }
            int32_t displacement = (int32_t)(this->sequence[(*this->sequence_current_index)++]);

            output.storage_type = MEMORY;

            struct StorageArgs intermediate_storage_args;
            intermediate_storage_args.storage_type = R32;
            intermediate_storage_args.address = rm;
            output.address = this->storage->load<int32_t>(intermediate_storage_args) + displacement;
            break;
        }

        case 0b10:
        {
            if (rm == 0b100)
            {
                throw std::logic_error("rm=100 mode not implemented for mod=10");
            }
            int32_t displacement;
            std::memcpy(&displacement, &(this->sequence[*this->sequence_current_index]), sizeof(int32_t));
            (*this->sequence_current_index) += sizeof(int32_t);

            output.storage_type = MEMORY;

            struct StorageArgs intermediate_storage_args;
            intermediate_storage_args.storage_type = R32;
            intermediate_storage_args.address = rm;
            output.address = this->storage->load<int32_t>(intermediate_storage_args) + displacement;
            break;
        }

        case 0b11:
            // TODO: determine the register size based on the instruction
            output.storage_type = R32;
            output.address = (int)rm;
            break;

        default:
            throw std::logic_error("ModRM mode access not implemented");
            break;
        }
        return output;
    }
};

#endif