#ifndef Storage_H
#define Storage_H

// #include <stack>
#include <stdexcept>
#include <cstring>
#include <string>
#include <iostream>

#include "Utils.hpp"

#define MAX_MEMORY_SIZE 10000
#define REGISTER_COUNT 8

class Storage
{
private:
    int32_t registers[REGISTER_COUNT]; // for EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI,
    int8_t memory[MAX_MEMORY_SIZE];
    // std::stack<int32_t> stack;
    // int32_t stack_pointer;

public:
    Storage(/* args */)
    {
        // stack_pointer = 0;
        for (int i = 0; i < REGISTER_COUNT; i++)
        {
            registers[i] = 0;
        }
        registers[EBP] = MAX_MEMORY_SIZE;
        registers[ESP] = MAX_MEMORY_SIZE;
    };

    template <class T>
    int stackPush(T value)
    {
        registers[ESP] -= sizeof(T);
        std::memcpy(&memory[ESP], &value, sizeof(T));
        return 0;
    }

    template <class T>
    T stackPop()
    {
        T output;
        std::memcpy(&output, &memory[ESP], sizeof(T));
        registers[ESP] -= sizeof(T);
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
            std::memcpy(&memory[storageArgs.address], &value, sizeof(T));
            break;

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
            std::memcpy(&output, &memory[storageArgs.address], sizeof(T));
            break;

        default:
            throw std::logic_error("Unknown storage access");
            break;
        }
        return output;
    }

    void printAll()
    {
        std::string register_names[REGISTER_COUNT] = {"EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI"};
        std::cout << "Registers\n=========================\n";
        for (int i = 0; i < REGISTER_COUNT; i++)
        {
            std::cout << register_names[i] << "\t" << registers[i] << std::endl;
        }

        std::cout << "=========================\n";
    }

    ~Storage(){};
};

#endif
