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

        case R8:
            // if (AL <= storage_id <= BL)
            // {
            //     registers[id - AL] =
            // }

            // registers[id - EAX] = (int32_t)value;
            throw std::logic_error("R8 register access not implemented");
            break;
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

        case R8:
            throw std::logic_error("R8 register access not implemented");
            break;
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
