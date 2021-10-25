#ifndef IMM81_H
#define IMM81_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

class IMM81 : public AbstractInstruction
{
private:
    ModRM* modRMByte;
    static const int register_operand_size = 32;
public:
    using AbstractInstruction::AbstractInstruction;     
    void execute()
    {
        std::cout << "executing IMM81\n";
        modRMByte = new ModRM(register_operand_size, this->sequence,this->sequence_current_index, this->storage);
        StorageArgs storageArgs = modRMByte->getModRM();
        int opcodeExtension = modRMByte->getOpcodeExtension();
        int32_t imm = this->getImmediateValue<int32_t>();
        int32_t regMemoryOperand = this->storage->load<int32_t>(storageArgs);
        int32_t result;
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        switch (opcodeExtension)
        {
            case 0:
            {
                result = regMemoryOperand + imm;
                hasCarry = FlagHandler::is32BitAddCarry(regMemoryOperand, imm);
                hasOverflow = FlagHandler::is32BitAddOveflow(regMemoryOperand, imm);
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                break;
            }
            case 1:
            {
                result = regMemoryOperand | imm;
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                break;
            }
            case 4:
            {
                result = regMemoryOperand & imm;
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                break;
            }
            case 5:
            {
                hasCarry = FlagHandler::is32BitSubCarry(regMemoryOperand, imm);
                hasOverflow = FlagHandler::is32BitSubOveflow(regMemoryOperand, imm);
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                result = regMemoryOperand - imm;
                break;
            }
            case 6:
            {
                result = regMemoryOperand ^ imm;
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                break;
            }
            default:
            {
                throw std::logic_error("Opcode extension not implemented for opcode 81");
                break;
            }
        }

        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow); 

        std::cout << "result = "<< result << std::endl;
        this->storage->save<int32_t>(result, storageArgs);
        free(modRMByte);
                
    }
    ~IMM81(){};
};

#endif