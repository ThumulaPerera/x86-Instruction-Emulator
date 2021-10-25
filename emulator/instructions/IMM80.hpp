#ifndef IMM80_H
#define IMM80_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

class IMM80 : public AbstractInstruction
{
private:
    ModRM* modRMByte;
    static const int register_operand_size = 8;
public:
    using AbstractInstruction::AbstractInstruction;     
    void execute()
    {
        modRMByte = new ModRM(register_operand_size, this->sequence,this->sequence_current_index, this->storage);
        StorageRawArgs storageRawArgs;
        StorageArgs storageArgs = modRMByte->getModRM(storageRawArgs);
        int opcodeExtension = modRMByte->getOpcodeExtension();
        int8_t imm = this->getImmediateValue<int8_t>();
        int8_t regMemoryOperand = this->storage->load<int8_t>(storageArgs);
        int8_t result;
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        switch (opcodeExtension)
        {
            case 0:
            {
                std::cout << "ADD ";
                result = regMemoryOperand + imm;
                hasCarry = FlagHandler::isAddCarry<int8_t, uint8_t>(regMemoryOperand, imm);
                hasOverflow = FlagHandler::isAddOverflow<int8_t>(regMemoryOperand, imm);
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
                hasCarry = FlagHandler::isSubCarry<int8_t,uint8_t>(regMemoryOperand, imm);
                hasOverflow = FlagHandler::isSubOverflow<int8_t>(regMemoryOperand, imm);
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
                hasCarry = FlagHandler::isSubCarry<int8_t,uint8_t>(regMemoryOperand, imm);
                hasOverflow = FlagHandler::isSubOverflow<int8_t>(regMemoryOperand, imm);
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
                result = regMemoryOperand - imm;
                break;
            }
            default:
            {
                throw std::logic_error("Opcode extension not implemented for opcode 80");
                break;
            }
        }
        std::cout << "$" << intToHexString<int8_t>(imm) << " , " << stringifyStorageRawArgs(storageRawArgs) << std::endl;

        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow); 

        std::cout << "result = "<< result << std::endl;

        // do not save result for CMP 
        if (opcodeExtension != 7)
        {
            this->storage->save<int8_t>(result, storageArgs);
        }
        
        
        free(modRMByte);
                
    }
    ~IMM80(){};
};

#endif