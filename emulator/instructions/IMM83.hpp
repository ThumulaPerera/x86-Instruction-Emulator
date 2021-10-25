#ifndef IMM83_H
#define IMM83_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

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

        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageRawArgs storageRawArgs;
        StorageArgs storageArgs = modRMByte->getModRM(storageRawArgs);
        int opcodeExtension = modRMByte->getOpcodeExtension();
        // std::cout << "IMM83 " << stringifyStorageRawArgs(storageRawArgs) << std::endl;
        int32_t imm = (int32_t)(this->getImmediateValue<int8_t>());
        int32_t regMemoryOperand = this->storage->load<int32_t>(storageArgs);
        int32_t result;
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        switch (opcodeExtension)
        {
        case 0:

        {
            std::cout << "ADD ";
            result = regMemoryOperand + imm;
            hasCarry = FlagHandler::is32BitAddCarry(regMemoryOperand, imm);
            hasOverflow = FlagHandler::is32BitAddOveflow(regMemoryOperand, imm);
            flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
            break;
        }
        case 1:

        {
            std::cout << "OR ";
            result = regMemoryOperand | imm;
            flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
            break;
        }
        case 4:

        {
            std::cout << "AND ";
            result = regMemoryOperand & imm;
            flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
            break;
        }
        case 5:

        {
            std::cout << "SUB ";
            result = regMemoryOperand - imm;
            hasCarry = FlagHandler::is32BitAddCarry(regMemoryOperand, imm);
            hasOverflow = FlagHandler::is32BitAddOveflow(regMemoryOperand, imm);
            flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
            break;
        }
        case 6:
        {
            std::cout << "XOR ";
            result = regMemoryOperand ^ imm;
            flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
            break;
        }
        default:
        {
            throw std::logic_error("Opcode extension not implemented for opcode 83");
            break;
        }
        }
        std::cout << "$" << intToHexString<int32_t>(imm) << " , " << stringifyStorageRawArgs(storageRawArgs) << std::endl;

        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);

        std::cout << "result = " << result << std::endl;
        this->storage->save<int32_t>(result, storageArgs);
        free(modRMByte);
    }
    ~IMM83(){};
};

#endif