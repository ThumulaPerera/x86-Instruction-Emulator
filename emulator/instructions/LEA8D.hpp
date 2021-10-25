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

        this->modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);

        StorageRawArgs operand1RawArgs;
        StorageArgs operand1Args = modRMByte->getReg(operand1RawArgs);
        StorageRawArgs operand2RawArgs;
        StorageArgs operand2Args = modRMByte->getModRM(operand2RawArgs);

        if (operand2Args.storage_type != MEMORY)
        {
            throw std::logic_error("LEA instruction expects memory location as a source operand. Register is given instead.");
        }

        std::cout << "LEA " << stringifyStorageRawArgs(operand2RawArgs) << " , " << stringifyStorageRawArgs(operand1RawArgs) << std::endl;

        int32_t result = operand2Args.address;
        std::cout << "result = " << result << std::endl << std::endl;
        this->storage->save<int32_t>(result, operand1Args);
        free(this->modRMByte);
    }
    ~LEA8D(){};
};

#endif