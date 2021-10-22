#ifndef INCDECFF_H
#define INCDECFF_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class INCDECFF : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing INCDECFF\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageArgs operand1Args = modRMByte->getModRM();
        int opcodeExtension = modRMByte->getOpcodeExtension();
        int32_t regMemoryOperand = this->storage->load<int32_t>(operand1Args);
        int32_t result;

        switch (opcodeExtension)
        {
        case 0:
            result = regMemoryOperand + 1;
            this->storage->save<int32_t>(result, operand1Args);
            break;

        case 1:
            result = regMemoryOperand - 1;
            this->storage->save<int32_t>(result, operand1Args);
            break;
        
        case 6:
            this->storage->stackPush<int32_t>(regMemoryOperand);
            break;

        default:
            throw std::logic_error("Opcode extension not implemented for opcode FF");
            break;
        }


        std::cout << "result = " << result << std::endl;
        free(modRMByte);
    }
    ~INCDECFF(){};
};

#endif