#ifndef ModRM_H
#define ModRM_H

#include <stdexcept>
#include "Utils.hpp"

class ModRM
{
private:
    uint8_t byte_value;

public:
    ModRM(const uint8_t *sequence, int *sequence_current_index)
    {
        byte_value = sequence[(*sequence_current_index)++];
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
        uint8_t reg_opcode = byte_value & ((uint8_t)0b00000111);
        struct StorageArgs output;
        switch (mod)
        {
        case 0b11:
            output.storage_type = R32;
            output.address = (int)reg_opcode;
            break;

        default:
            throw std::logic_error("ModRM mode access not implemented");
            break;
        }
        return output;
    }
};

#endif