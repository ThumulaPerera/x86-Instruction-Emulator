#ifndef MOVC7_H
#define MOVC7_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

// Move imm32 to r/m32

class MOVC7 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {

        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageRawArgs operandRawArgs;
        StorageArgs operandArgs = modRMByte->getModRM(operandRawArgs);

        int32_t movedValue = this->getImmediateValue<int32_t>();
        std::cout << "MOV "
                  << "$" << intToHexString<int32_t>(movedValue) << " , " << stringifyStorageRawArgs(operandRawArgs) << std::endl;

        std::cout << "value being moved = " << movedValue << std::endl << std::endl;
        this->storage->save<int32_t>(movedValue, operandArgs);
    }
    ~MOVC7(){};
};

#endif