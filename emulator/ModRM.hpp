#ifndef ModRM_H
#define ModRM_H

#include <stdexcept>
#include <cstring>
#include "Utils.hpp"
#include "Storage.hpp"
#include "SIB.hpp"

class ModRM
{
private:
    uint8_t byte_value;
    int register_operand_size;
    const uint8_t *sequence;
    int *sequence_current_index;
    Storage *storage;

    enum StorageType getRegisterType()
    {
        switch (this->register_operand_size)
        {
        case 8:
            return R8;
            break;

        case 16:
            return R16;
            break;

        case 32:
            return R32;
            break;

        default:
            throw std::logic_error("Operand size should be one of 8, 16, 32");
            break;
        }
    }

public:
    ModRM(const int register_operand_size, const uint8_t *sequence, int *sequence_current_index, Storage *storage)
    {
        byte_value = sequence[(*sequence_current_index)++];
        this->register_operand_size = register_operand_size;
        this->sequence = sequence;
        this->sequence_current_index = sequence_current_index;
        this->storage = storage;
    };
    ~ModRM(){};

    struct StorageArgs getReg()
    {
        struct StorageArgs output;
        output.storage_type = this->getRegisterType();
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

            output.storage_type = MEMORY;

            if (rm == 0b100)
            {
                output.address = SIB::getScaledIndex(this->sequence, this->sequence_current_index, mod, this->storage);
            }
            else if (rm == 0b101)
            {
                int32_t displacement;
                std::memcpy(&displacement, &(this->sequence[*this->sequence_current_index]), sizeof(int32_t));
                (*this->sequence_current_index) += sizeof(int32_t);
                output.address = displacement;
            }
            else
            {
                struct StorageArgs intermediate_storage_args;
                intermediate_storage_args.storage_type = R32;
                intermediate_storage_args.address = rm;
                output.address = this->storage->load<int32_t>(intermediate_storage_args);
            }

            break;
        }

        case 0b01:
        {

            output.storage_type = MEMORY;

            if (rm == 0b100)
            {
                int32_t SIB_value = SIB::getScaledIndex(this->sequence, this->sequence_current_index, mod, this->storage);
                int32_t displacement = (int32_t)(this->sequence[(*this->sequence_current_index)++]);
                output.address = SIB_value + displacement;
            }
            else
            {
                int32_t displacement = (int32_t)(this->sequence[(*this->sequence_current_index)++]);

                struct StorageArgs intermediate_storage_args;
                intermediate_storage_args.storage_type = R32;
                intermediate_storage_args.address = rm;
                output.address = this->storage->load<int32_t>(intermediate_storage_args) + displacement;
            }

            break;
        }

        case 0b10:
        {

            output.storage_type = MEMORY;

            if (rm == 0b100)
            {
                int32_t SIB_value = SIB::getScaledIndex(this->sequence, this->sequence_current_index, mod, this->storage);

                int32_t displacement;
                std::memcpy(&displacement, &(this->sequence[*this->sequence_current_index]), sizeof(int32_t));
                (*this->sequence_current_index) += sizeof(int32_t);
                output.address = SIB_value + displacement;
            }
            else
            {
                int32_t displacement;
                std::memcpy(&displacement, &(this->sequence[*this->sequence_current_index]), sizeof(int32_t));
                (*this->sequence_current_index) += sizeof(int32_t);
                struct StorageArgs intermediate_storage_args;
                intermediate_storage_args.storage_type = R32;
                intermediate_storage_args.address = rm;
                output.address = this->storage->load<int32_t>(intermediate_storage_args) + displacement;
            }

            break;
        }

        case 0b11:
            output.storage_type = this->getRegisterType();
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