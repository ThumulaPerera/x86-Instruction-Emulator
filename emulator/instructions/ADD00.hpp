#ifndef ADD00_H
#define ADD00_H

#include <iostream>
#include <string>

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
        StorageRawArgs operand1RawArgs;
        StorageArgs operand1Args = modRMByte->getModRM(operand1RawArgs);
        StorageRawArgs operand2RawArgs;
        StorageArgs operand2Args = modRMByte->getReg(operand2RawArgs);
        int8_t result = this->storage->load<int8_t>(operand1Args) +
                         this->storage->load<int8_t>(operand2Args);
        std::cout << "result = " << std::to_string(result) << std::endl;
        this->storage->save<int8_t>(result, operand1Args);
        free(modRMByte);
    }
    ~ADD00(){};
};

#endif