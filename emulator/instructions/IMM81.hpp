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
        modRMByte = new ModRM(register_operand_size, this->sequence,this->sequence_current_index, this->storage);
        StorageRawArgs storageRawArgs;
        StorageArgs storageArgs = modRMByte->getModRM(storageRawArgs);
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
                hasCarry = FlagHandler::is32BitSubCarry(regMemoryOperand, imm);
                hasOverflow = FlagHandler::is32BitSubOveflow(regMemoryOperand, imm);
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                result = regMemoryOperand - imm;
                break;
            }
            case 6:
            {
                std::cout << "XOR ";
                result = regMemoryOperand ^ imm;
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                break;
            }
            case 7:{
                 std::cout << "CMP ";
                // flags same as SUB operation
                hasCarry = FlagHandler::is32BitSubCarry(regMemoryOperand, imm);
                hasOverflow = FlagHandler::is32BitSubOveflow(regMemoryOperand, imm);
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                result = regMemoryOperand - imm;
                break;
            }
            default:
            {
                throw std::logic_error("Opcode extension not implemented for opcode 81");
                break;
            }
        }
        std::cout << "$" << intToHexString<int32_t>(imm) << " , " << stringifyStorageRawArgs(storageRawArgs) << std::endl;

        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow); 

        std::cout << "result = "<< result << std::endl;

        // do not save result for CMP 
        if (opcodeExtension != 7)
        {
            this->storage->save<int32_t>(result, storageArgs);
        }
        
        
        free(modRMByte);
                
    }
    ~IMM81(){};
};

#endif