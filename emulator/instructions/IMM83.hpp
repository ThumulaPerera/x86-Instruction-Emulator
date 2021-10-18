#ifndef IMM83_H
#define IMM83_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class IMM83 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    // IMM83(uint8_t opCode, int* seq_idx) : AbstractInstruction(opCode, seq_idx){};
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing IMM83\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageArgs storageArgs = modRMByte->getModRM();
        int opcodeExtension = modRMByte->getOpcodeExtension();
        int32_t imm = (int32_t)(this->getImmediateValue<int8_t>());
        int32_t regMemoryOperand = this->storage->load<int32_t>(storageArgs);
        int32_t result;

        switch (opcodeExtension)
        {
        case 0:
            result = regMemoryOperand + imm;
            break;

        case 1:
            result = regMemoryOperand | imm;
            break;

        case 4:
            result = regMemoryOperand & imm;
            break;

        case 5:
            result = regMemoryOperand - imm;
            break;

        case 6:
            result = regMemoryOperand ^ imm;
            break;

        default:
            throw std::logic_error("Opcode extension not implemented for opcode 83");
            break;
        }
        std::cout << "result = " << result << std::endl;
        this->storage->save<int32_t>(result, storageArgs);
        free(modRMByte);
    }
    ~IMM83(){};
};

#endif