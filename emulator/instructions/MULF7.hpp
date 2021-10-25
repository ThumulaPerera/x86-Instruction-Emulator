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
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        struct StorageArgs operand1Args;
        operand1Args.storage_type = R32;
        operand1Args.address = (int32_t)EAX;

        StorageRawArgs operand2RawArgs;
        StorageArgs operand2Args = modRMByte->getModRM(operand2RawArgs);

        struct StorageArgs operand3Args;
        operand3Args.storage_type = R32;
        operand3Args.address = (int32_t)EDX;

        switch (opcodeExtension)
        {
        case 4:
        {
            uint64_t operand1 = (uint64_t)this->storage->load<uint32_t>(operand1Args);
            uint64_t operand2 = (uint64_t)this->storage->load<uint32_t>(operand2Args);

            uint64_t result = operand1 * operand2;

            uint32_t lower = (uint32_t)(result & 0x00000000ffffffff);
            uint32_t upper = (uint32_t)(result >> 32);

            std::cout << "result = " << result << std::endl;
            std::cout << "upper = " << upper << std::endl;
            std::cout << "lower = " << lower << std::endl;

            this->storage->save<uint32_t>(lower, operand1Args);
            this->storage->save<uint32_t>(upper, operand3Args);

            hasCarry = upper;
            hasOverflow = upper;

            flagsAffected.insert(flagsAffected.end(), {OF, CF});
            FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);

            break;
        }
        case 5:
        {
            int64_t operand1 = (int64_t)this->storage->load<int32_t>(operand1Args);
            int64_t operand2 = (int64_t)this->storage->load<int32_t>(operand2Args);

            int64_t result = operand1 * operand2;

            uint32_t lower = (uint32_t)(result & 0x00000000ffffffff);
            uint32_t upper = (uint32_t)(result >> 32);

            int32_t signed_lower = (int32_t)lower;

            std::cout << "result = " << result << std::endl;
            std::cout << "upper = " << upper << std::endl;
            std::cout << "lower = " << lower << std::endl;

            this->storage->save<uint32_t>(lower, operand1Args);
            this->storage->save<uint32_t>(upper, operand3Args);

            if(signed_lower != result){
                hasCarry = true;
                hasOverflow = true;
            }

            flagsAffected.insert(flagsAffected.end(), {OF, CF, SF});
            FlagHandler::setFlags(lower, this->storage, flagsAffected, hasCarry, hasOverflow);

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