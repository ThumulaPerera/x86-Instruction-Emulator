#ifndef POP_H
#define POP_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class POP : public AbstractInstruction
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
        operandArgs.address = (int32_t)(this->opCode - ((uint8_t)0x58));

        struct StorageRawArgs operandRawArgs;
        operandRawArgs.storage_type = R32;
        operandRawArgs.direct_reg = operandArgs.address;

        std::cout << "POP " << stringifyStorageRawArgs(operandRawArgs) << std::endl;

        int32_t poppped_value = this->storage->stackPop<int32_t>();
        std::cout << "poppped value = " << poppped_value << std::endl;
        this->storage->save<int32_t>(poppped_value, operandArgs);
    }
    ~POP(){};
};

#endif