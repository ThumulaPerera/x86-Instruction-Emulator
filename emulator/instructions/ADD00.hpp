#ifndef ADD00_H
#define ADD00_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class ADD00 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 8;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing ADD00\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageArgs operand1Args = modRMByte->getModRM();
        StorageArgs operand2Args = modRMByte->getReg();
        int8_t result = this->storage->load<int8_t>(operand1Args) +
                         this->storage->load<int8_t>(operand2Args);
        std::cout << "result = " << result << std::endl;
        this->storage->save<int8_t>(result, operand1Args);
        free(modRMByte);
    }
    ~ADD00(){};
};

#endif