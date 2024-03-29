#ifndef INC40_H
#define INC40_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

class INC40 : public AbstractInstruction
{
private:
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        struct StorageArgs operandArgs;
        operandArgs.storage_type = R32;
        operandArgs.address = (int32_t)(this->opCode - ((uint8_t)0x40));

        struct StorageRawArgs operandRawArgs;
        operandRawArgs.storage_type = R32;
        operandRawArgs.direct_reg = operandArgs.address;

        int32_t regMemoryOperand = this->storage->load<int32_t>(operandArgs);

        int32_t result;
        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        std::cout << "INC " << stringifyStorageRawArgs(operandRawArgs) << std::endl;

        result = regMemoryOperand + 1;
        this->storage->save<int32_t>(result, operandArgs);

        flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF});
        hasOverflow = FlagHandler::isAddOverflow<int32_t>(regMemoryOperand, 1);
        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);  

        std::cout << "result = " << result << std::endl << std::endl;
    }
    ~INC40(){};
};

#endif