#ifndef XOR34_H
#define XOR34_H

#include <iostream>
#include <string>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class XOR34 : public AbstractInstruction
{
private:
    static const int register_operand_size = 8;

public:
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {

        StorageArgs regOperandArgs;
        regOperandArgs.storage_type = R8;
        regOperandArgs.address = 0;

        StorageRawArgs regOperandRawArgs;
        regOperandRawArgs.storage_type = R8;
        regOperandRawArgs.direct_reg = 0;

        int8_t imm = this->getImmediateValue<int8_t>();

        std::cout << "XOR "
                  << "$" << intToHexString<int8_t>(imm) << " , " << stringifyStorageRawArgs(regOperandRawArgs) << std::endl;
        int8_t result = this->storage->load<int8_t>(regOperandArgs) ^ imm;

        std::vector<FlagType> flagsAffected;
        bool hasOverflow = false;
        bool hasCarry = false;

        std::cout << "result = " << std::to_string(result) << std::endl << std::endl;
        this->storage->save<int8_t>(result, regOperandArgs);

        flagsAffected.insert(flagsAffected.end(), {OF, SF, ZF, PF, CF});
        FlagHandler::setFlags(result, this->storage, flagsAffected, hasCarry, hasOverflow);
    }
    ~XOR34(){};
};

#endif