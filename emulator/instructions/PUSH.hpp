#ifndef PUSH_H
#define PUSH_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class PUSH : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing PUSH\n";

        struct StorageArgs operandArgs;
        operandArgs.storage_type = R32;
        operandArgs.address = (int32_t)(this->opCode - ((uint8_t)0x50));
        
        int32_t value_to_push = this->storage->load<int32_t>(operandArgs);
        std::cout << "result = " << value_to_push << std::endl;
        this->storage->stackPush<int32_t>(value_to_push);
    }
    ~PUSH(){};
};

#endif