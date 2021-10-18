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
        std::cout << "executing POP\n";

        struct StorageArgs operandArgs;
        operandArgs.storage_type = R32;
        operandArgs.address = (int32_t)(this->opCode - ((uint8_t)0x58));
        int32_t poppped_value = this->storage->stackPop<int32_t>();
        std::cout << "result = " << poppped_value << std::endl;
        this->storage->save<int32_t>(poppped_value, operandArgs);
    }
    ~POP(){};
};

#endif