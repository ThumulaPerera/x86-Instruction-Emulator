#ifndef ADD01_H
#define ADD01_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

// Add r32 to r/m32
class ADD01 : public AbstractInstruction
{
private:
    ModRM *modRMByte;
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing ADD01\n";
        modRMByte = new ModRM(register_operand_size, this->sequence, this->sequence_current_index, this->storage);
        StorageArgs operand1Args = modRMByte->getModRM();
        StorageArgs operand2Args = modRMByte->getReg();

        int32_t operand1 = this->storage->load<int32_t>(operand1Args);
        int32_t operand2 = this->storage->load<int32_t>(operand2Args);

        int32_t result = operand1 + operand2;

        std::cout << "result = " << result << std::endl;
        this->storage->save<int32_t>(result, operand1Args);

        bool hasCarry = FlagHandler::is32BitAddCarry(operand1, operand2);
        bool hasOverflow = FlagHandler::is32BitAddOveflow(operand1, operand2);

        std::vector<FlagType> flagsAffected;
        flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);

        free(modRMByte);
    }
    ~ADD01(){};
};

#endif