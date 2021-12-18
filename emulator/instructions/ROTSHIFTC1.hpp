#ifndef ROTSHIFTC1_H
#define ROTSHIFTC1_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

class ROTSHIFTC1 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageRawArgs storageRawArgs;
        StorageArgs storageArgs = modRMByte->getModRM(storageRawArgs);
        int opcodeExtension = modRMByte->getOpcodeExtension();
        uint8_t imm = this->getImmediateValue<uint8_t>();
        int32_t regMemoryOperand = this->storage->load<int32_t>(storageArgs);
        int32_t result;
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        switch (opcodeExtension)
        {
        case 7:
        {
            std::cout << "SAR ";
            result = regMemoryOperand;
            for (uint8_t i = 0; i < imm; i++)
            {
                hasCarry = result & (uint32_t)1;
                result = result / 2;
            }

            flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
            break;
        }

        default:
        {
            throw std::logic_error("Opcode extension not implemented for opcode C1");
            break;
        }
        }
        std::cout << "$" << intToHexString<uint8_t>(imm) << " , " << stringifyStorageRawArgs(storageRawArgs) << std::endl;

        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);

        std::cout << "result = " << result << std::endl << std::endl;

        this->storage->save<int32_t>(result, storageArgs);

        free(modRMByte);
    }
    ~ROTSHIFTC1(){};
};

#endif