#ifndef ADD05_H
#define ADD05_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"
#include "../FlagHandler.hpp"

class ADD05 : public AbstractInstruction
{
private:
    static const int register_operand_size = 32;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing ADD05\n";

        struct StorageArgs operandArgs;
        operandArgs.storage_type = R32;
        operandArgs.address = EAX;
        int32_t imm = this->getImmediateValue<int32_t>();
        int32_t regOperand = this->storage->load<int32_t>(operandArgs);

        struct StorageRawArgs operandRawArgs;
        operandRawArgs.storage_type = R32;
        operandRawArgs.direct_reg = EAX;

        std::cout << "ADD "
                  << "$" << intToHexString<int32_t>(imm) << " , " << stringifyStorageRawArgs(operandRawArgs) << std::endl;


        int32_t result = regOperand + imm;
        std::cout << "result = " << result << std::endl << std::endl;
        this->storage->save<int32_t>(result, operandArgs);

        bool hasCarry = FlagHandler::isAddCarry<int32_t, uint32_t>(imm, regOperand);
        bool hasOverflow = FlagHandler::isAddOverflow<int32_t>(imm, regOperand);

        std::vector<FlagType> flagsAffected;
        flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);
    }
    ~ADD05(){};
};

#endif