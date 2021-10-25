#ifndef MOVIMM_H
#define MOVIMM_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

// Move imm32 to r32

class MOVIMM : public AbstractInstruction
{
private:
public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {

        struct StorageArgs operand1Args;
        operand1Args.storage_type = R32;
        operand1Args.address = (int32_t)(this->opCode - ((uint8_t)0xb8));

        struct StorageRawArgs operand1RawArgs;
        operand1RawArgs.storage_type = R32;
        operand1RawArgs.direct_reg = operand1Args.address;

        int32_t movedValue = this->getImmediateValue<int32_t>();
        std::cout << "MOV "
                  << "$" << intToHexString<int32_t>(movedValue) << " , " << stringifyStorageRawArgs(operand1RawArgs) << std::endl;

        std::cout << "value being moved = " << movedValue << std::endl << std::endl;
        this->storage->save<int32_t>(movedValue, operand1Args);
    }
    ~MOVIMM(){};
};

#endif