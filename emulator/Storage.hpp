#ifndef Storage_H
#define Storage_H

#include <stdexcept>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdio>
#include <list>

#include "Utils.hpp"

#define REGISTER_COUNT 8
#define SEGMENT_REGISTER_COUNT 6

class Storage
{
private:
    int32_t registers[REGISTER_COUNT]; // for EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI
    int32_t eip;
    int32_t eflags;
    int16_t segment_registers[SEGMENT_REGISTER_COUNT]; // for CS,SS,DS,ES,FS,GS

    // int8_t memory[MAX_MEMORY_SIZE];
    std::list<MemoryBlock> new_memory;

    void saveByteToMemory(uint32_t address, int8_t value)
    {
        std::list<MemoryBlock>::iterator position;
        for (position = new_memory.begin(); position != new_memory.end(); position++)
        {
            if (position->address == address)
            {
                position->value = value;
                return;
            }

            if (position->address > address)
            {
                break;
            }
        }
        new_memory.insert(position, {address : address, value : value});

        return;
    }

    int8_t loadByteFromMemory(uint32_t address)
    {
        std::list<MemoryBlock>::iterator position;
        for (position = new_memory.begin(); position != new_memory.end(); position++)
        {
            if (position->address == address)
            {
                return position->value;
            }
            if (position->address > address)
            {
                break;
            }
        }
        printf("Warning: Reading from uninitialized memory location 0x%x\n", address);
        new_memory.insert(position, {address : address, value : (int8_t)0});
        return (int8_t)0;
    }

public:
    Storage()
    {
        // initialize registers;
        registers[EAX] = 0xbf8db144;
        registers[ECX] = 0x88c5cffb;
        registers[EDX] = 0x1;
        registers[EBX] = 0xae5ff4;
        registers[ESP] = 0xbf8db0bc;
        registers[EBP] = 0xbf8db118;
        registers[ESI] = 0x9a0ca0;
        registers[EDI] = 0x0;

        eip = 0x8048354;
        eflags = 0x246;

        segment_registers[CS] = 0x73;
        segment_registers[SS] = 0x7b;
        segment_registers[DS] = 0x7b;
        segment_registers[ES] = 0x7b;
        segment_registers[FS] = 0x0;
        segment_registers[GS] = 0x33;

        // initialize memory
        new_memory.push_back({address : 0, value : 0});
        new_memory.push_back({address : 0xffffffff, value : 0});
    };

    template <class T>
    int stackPush(T value)
    {
        registers[ESP] -= sizeof(T);
        int8_t *buffer = (int8_t *)malloc(sizeof(T));
        std::memcpy(buffer, &value, sizeof(T));
        for (size_t i = 0; i < sizeof(T); i++)
        {
            saveByteToMemory(((uint32_t)(registers[ESP])) + ((uint32_t)i), buffer[i]);
        }
        free(buffer);
        return 0;
    }

    template <class T>
    T stackPop()
    {
        T output;
        int8_t *buffer = (int8_t *)malloc(sizeof(T));
        for (size_t i = 0; i < sizeof(T); i++)
        {
            buffer[i] = loadByteFromMemory(((uint32_t)(registers[ESP])) + ((uint32_t)i));
        }

        std::memcpy(&output, buffer, sizeof(T));
        free(buffer);
        registers[ESP] += sizeof(T);
        return output;
    }

    template <class T>
    int save(T value, struct StorageArgs storageArgs)
    {
        switch (storageArgs.storage_type)
        {
        case R32:
            std::memcpy(&registers[storageArgs.address - EAX], &value, sizeof(int32_t));
            break;

        case R16:
        {
            uint32_t extended_value = (uint32_t)((uint16_t)(value));
            uint32_t final_value = (((uint32_t)registers[storageArgs.address - AX]) & 0xffff0000) | extended_value;
            std::memcpy(&registers[storageArgs.address - AX], &final_value, sizeof(int32_t));
            break;
        }

        case R8:
        {
            uint32_t bit_mask;
            int shift_size;
            int32_t reference;
            if (storageArgs.address <= BL)
            {
                bit_mask = 0xffffff00;
                shift_size = 0;
                reference = AL;
            }
            else
            {
                bit_mask = 0xffff00ff;
                shift_size = 8;
                reference = AH;
            }

            uint32_t extended_value = ((uint32_t)((uint8_t)(value))) << shift_size;
            uint32_t final_value = (((uint32_t)registers[storageArgs.address - reference]) & bit_mask) | extended_value;
            std::memcpy(&registers[storageArgs.address - reference], &final_value, sizeof(int32_t));
            break;
        }
        case MEMORY:
        {
            int8_t *buffer = (int8_t *)malloc(sizeof(T));
            std::memcpy(buffer, &value, sizeof(T));
            for (size_t i = 0; i < sizeof(T); i++)
            {
                saveByteToMemory(((uint32_t)storageArgs.address) + ((uint32_t)i), buffer[i]);
            }
            free(buffer);

            break;
        }

        default:
            throw std::logic_error("Unknown storage access");
            break;
        }

        return 0;
    }

    template <class T>
    T load(struct StorageArgs storageArgs)
    {
        T output;
        switch (storageArgs.storage_type)
        {
        case R32:
            std::memcpy(&output, &registers[storageArgs.address - EAX], sizeof(int32_t));
            break;

        case R16:
        {
            output = (T)(((uint32_t)registers[storageArgs.address - AX]) & 0x0000ffff);
            break;
        }

        case R8:
        {
            uint32_t bit_mask;
            int shift_size;
            int32_t reference;
            if (storageArgs.address <= BL)
            {
                bit_mask = 0x000000ff;
                shift_size = 0;
                reference = AL;
            }
            else
            {
                bit_mask = 0x0000ff00;
                shift_size = 8;
                reference = AH;
            }
            output = (T)((((uint32_t)registers[storageArgs.address - reference]) & bit_mask) >> shift_size);
            break;
        }
        case MEMORY:
        {
            int8_t *buffer = (int8_t *)malloc(sizeof(T));
            for (size_t i = 0; i < sizeof(T); i++)
            {
                buffer[i] = loadByteFromMemory(((uint32_t)storageArgs.address) + ((uint32_t)i));
            }

            std::memcpy(&output, buffer, sizeof(T));
            free(buffer);
            break;
        }

        default:
            throw std::logic_error("Unknown storage access");
            break;
        }
        return output;
    }

    void setFlag(enum FlagType flag)
    {
        eflags |= (1UL << flag);
    }

    void resetFlag(enum FlagType flag)
    {
        eflags &= ~(1UL << flag);
    }

    void printAll()
    {
        std::string register_names[REGISTER_COUNT] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
        std::cout << "Registers\n=========================\n";
        for (int i = 0; i < REGISTER_COUNT; i++)
        {
            std::cout << register_names[i] << "\t" << intToHexString(registers[i]) << std::endl;
        }
        std::cout << "EIP"
                  << "\t" << intToHexString(eip) << std::endl;
        std::cout << "EFLAGS"
                  << "\t" << intToHexString(eflags) << std::endl;
        std::string segment_register_names[REGISTER_COUNT] = {"CS", "SS", "DS", "ES", "FS", "GS"};
        for (int i = 0; i < SEGMENT_REGISTER_COUNT; i++)
        {
            std::cout << segment_register_names[i] << "\t" << intToHexString(segment_registers[i]) << std::endl;
        }
        std::cout << "=========================\n";
        std::cout << "\nMemory\n=========================\n";
        std::cout << "Address\t\tValue\n";
        std::list<MemoryBlock>::iterator position;
        for (position = new_memory.begin(); position != new_memory.end(); position++)
        {
            std::cout << intToHexString(position->address) << "\t" << intToHexString(position->value) << std::endl;
        }
    }

    ~Storage(){};
};

#endif
