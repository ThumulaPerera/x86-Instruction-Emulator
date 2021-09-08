#ifndef Storage_H
#define Storage_H

#include <stack>

#include "Utils.hpp"

class Storage
{
private:
    int32_t registers[8]; // for EAX,ECX,EDX,EBX,ESP,EBP,ESI,EDI,
    std::stack<int32_t> stack;
    int32_t stack_pointer;

public:
    Storage(/* args */)
    {
        stack_pointer = 0;
    };
    ~Storage(){};
    int stackPush(int32_t value)
    {
        stack.push(value);
        return 0;
    }

    int32_t stackPush()
    {
        int32_t value = stack.top();
        stack.pop();
        return value;
    }

    int save(int32_t value, StorageId id)
    {
        if (EAX <= id && id <= EDI)
        {
            registers[id - EAX] = value;
            return 0;
        }

        return 1;
    }

    int32_t load(StorageId id)
    {
        if (EAX <= id && id <= EDI)
        {
            return registers[id - EAX];
        }
        return -1;
    }
};

#endif
