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
        struct StorageArgs operandArgs;
        operandArgs.storage_type = R32;
        operandArgs.address = (int32_t)(this->opCode - ((uint8_t)0x50));

        struct StorageRawArgs operandRawArgs;
        operandRawArgs.storage_type = R32;
        operandRawArgs.direct_reg = operandArgs.address;

        std::cout << "PUSH " << stringifyStorageRawArgs(operandRawArgs) << std::endl;
        
        int32_t value_to_push = this->storage->load<int32_t>(operandArgs);
        std::cout << "value to push = " << value_to_push << std::endl << std::endl;
        this->storage->stackPush<int32_t>(value_to_push);
    }
    ~PUSH(){};
};

#endif