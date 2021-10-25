#ifndef INCDECFF_H
#define INCDECFF_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

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
        StorageRawArgs storageRawArgs;
        StorageArgs operand1Args = modRMByte->getModRM(storageRawArgs);
        int opcodeExtension = modRMByte->getOpcodeExtension();
        int32_t regMemoryOperand = this->storage->load<int32_t>(operand1Args);
        int32_t result;
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        switch (opcodeExtension)
        {
            case 0:
            {   
                result = regMemoryOperand + 1;
                this->storage->save<int32_t>(result, operand1Args);
                
                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF});
                hasOverflow = FlagHandler::is32BitAddOveflow(regMemoryOperand, 1);

                break;
            }

            case 1:
            {
                result = regMemoryOperand - 1;
                this->storage->save<int32_t>(result, operand1Args);

                flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF});
                hasOverflow = FlagHandler::is32BitAddOveflow(regMemoryOperand, 1);

                break;
            }

            case 6:
            {
                this->storage->stackPush<int32_t>(regMemoryOperand);
                break;
            }

            default:
                throw std::logic_error("Opcode extension not implemented for opcode FF");
                break;
        }
        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);    

        std::cout << "result = " << result << std::endl;
        free(modRMByte);
    }
    ~INCDECFF(){};
};

#endif