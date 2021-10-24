#ifndef SIB_H
#define SIB_H

#include <stdexcept>
// #include <cstring>
#include "Storage.hpp"
#include "Utils.hpp"

class SIB
{

public:
    static int32_t getScaledIndex(const uint8_t *sequence, int *sequence_current_index, uint8_t modrm_mod_value,
                                  Storage *storage, struct StorageRawArgs &output_raw_args)
    {
        uint8_t byte_value = sequence[(*sequence_current_index)++];

        uint8_t scale = (byte_value & ((uint8_t)0b11000000)) >> 6;
        uint8_t index = (byte_value & ((uint8_t)0b00111000)) >> 3;
        uint8_t base = byte_value & ((uint8_t)0b00000111);

        if (index == 0b100)
        {
            throw std::logic_error("index=100 not supported in SIB");
        }

        uint8_t multiplier = ((uint8_t)1) << scale;

        struct StorageArgs index_storage_args;
        index_storage_args.storage_type = R32;
        index_storage_args.address = index;

        output_raw_args.has_scale = true;
        output_raw_args.has_scale_factor = true;

        output_raw_args.scale_reg = index;
        output_raw_args.scale_factor = multiplier;

        int32_t scaled_index = storage->load<int32_t>(index_storage_args) * ((int32_t)multiplier);
        if (base == 0b101 && modrm_mod_value == 0b00)
        {
            /*
            no base in this case
            refer Table 2-3. 32-Bit Addressing Forms with the SIB Byte
            */
            int32_t displacement;
            std::memcpy(&displacement, &(sequence[*sequence_current_index]), sizeof(int32_t));
            (*sequence_current_index) += sizeof(int32_t);

            output_raw_args.has_displacement = true;
            output_raw_args.has_base = false;
            output_raw_args.displacement = displacement;

            return scaled_index + displacement;
        }

        output_raw_args.has_displacement = false;
        output_raw_args.has_base = true;

        struct StorageArgs base_storage_args;
        base_storage_args.storage_type = R32;
        base_storage_args.address = base;

        output_raw_args.base_reg = base;

        int32_t base_value = storage->load<int32_t>(base_storage_args);

        return base_value + scaled_index;
    }
};

#endif