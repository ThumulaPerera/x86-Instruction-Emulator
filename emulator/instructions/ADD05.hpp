#ifndef ADD05_H
#define ADD05_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class ADD05 : public AbstractInstruction
{
private:
    static const int register_operand_size = 16;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing ADD05\n";

        struct StorageArgs operandArgs;
        operandArgs.storage_type = R16;
        operandArgs.address = AX;
        int16_t imm = this->getImmediateValue<int16_t>();
        int16_t result = this->storage->load<int16_t>(operandArgs) + imm;
        std::cout << "result = " << result << std::endl;
        this->storage->save<int16_t>(result, operandArgs);
    }
    ~ADD05(){};
};

#endif