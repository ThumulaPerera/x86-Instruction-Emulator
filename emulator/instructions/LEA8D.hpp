#ifndef LEA8D_H
#define LEA8D_H

#include "AbstractInstruction.hpp"
#include "../Utils.hpp"

class LEA8D : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing LEA8D\n";
        this->modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageArgs operand1Args = modRMByte->getReg();
        StorageArgs operand2Args = modRMByte->getModRM();

        if (operand2Args.storage_type != MEMORY)
        {
            throw std::logic_error("LEA instruction expects memory location as a source operand. Register is given instead.");
        }

        int32_t result = operand2Args.address;
        std::cout << "result = " << result << std::endl;
        this->storage->save<int32_t>(result, operand1Args);
        free(this->modRMByte);
    }
    ~LEA8D(){};
};

#endif