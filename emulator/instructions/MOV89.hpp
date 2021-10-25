#ifndef MOV89_H
#define MOV89_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

// Move r32 to r/m32

class MOV89 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing MOV89\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageRawArgs operand1RawArgs;
        StorageRawArgs operand2RawArgs;
        StorageArgs operand1Args = modRMByte->getModRM(operand1RawArgs);
        StorageArgs operand2Args = modRMByte->getReg(operand2RawArgs);
        int32_t movedValue = this->storage->load<int32_t>(operand2Args);
        std::cout << "value being moved = " << movedValue << std::endl;
        this->storage->save<int32_t>(movedValue, operand1Args);
        free(modRMByte);
    }
    ~MOV89(){};
};

#endif