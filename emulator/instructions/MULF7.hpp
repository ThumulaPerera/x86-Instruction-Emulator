#ifndef MULF7_H
#define MULF7_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

// Unsigned multiply (EDX:EAX ← EAX ∗ r/m32)

class MULF7 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing MULF7\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        int opcodeExtension = modRMByte->getOpcodeExtension();

        switch (opcodeExtension)
        {
        case 4:
        {
            struct StorageArgs operand1Args;
            operand1Args.storage_type = R32;
            operand1Args.address = (int32_t)EAX;
            StorageArgs operand2Args = modRMByte->getModRM();

            uint64_t operand1 = (uint64_t)this->storage->load<uint32_t>(operand1Args);
            uint64_t operand2 = (uint64_t)this->storage->load<uint32_t>(operand2Args);

            uint64_t result = operand1 * operand2;

            uint32_t lower = (uint32_t)(result & 0x00000000ffffffff);
            uint32_t upper = (uint32_t)(result >> 32);

            std::cout << "result = " << result << std::endl;
            std::cout << "upper = " << upper << std::endl;
            std::cout << "lower = " << lower << std::endl;

            struct StorageArgs operand3Args;
            operand3Args.storage_type = R32;
            operand3Args.address = (int32_t)EDX;

            this->storage->save<int32_t>(lower, operand1Args);
            this->storage->save<int32_t>(upper, operand3Args);

            bool hasCarry = upper;
            bool hasOverflow = upper;

            enum FlagType flagsAffected[] = {OF, CF};
            FlagHandler::setFlags(result, this->storage, flagsAffected, 2, hasCarry, hasOverflow);

            break;
        }

        default:
            throw std::logic_error("Opcode extension not implemented for opcode F7");
            break;
        }

        free(modRMByte);
    }
    ~MULF7(){};
};

#endif