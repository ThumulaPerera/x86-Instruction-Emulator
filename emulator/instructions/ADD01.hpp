#ifndef ADD01_H
#define ADD01_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class ADD01 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing ADD01\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageArgs operand1Args = modRMByte->getModRM();
        StorageArgs operand2Args = modRMByte->getReg();
        int32_t result = this->storage->load<int32_t>(operand1Args) +
                         this->storage->load<int32_t>(operand2Args);
        std::cout << "result = " << result << std::endl;
        this->storage->save<int32_t>(result, operand1Args);
        free(modRMByte);
    }
    ~ADD01(){};
};

#endif