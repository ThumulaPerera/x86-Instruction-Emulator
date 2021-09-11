#ifndef InstructionFactory_H
#define InstructionFactory_H

#include <stdexcept>

#include "instructions/AbstractInstruction.hpp"
#include "instructions/ADD01.hpp"
#include "instructions/ADD83.hpp"
#include "instructions/LEA8D.hpp"

#include "instructions/SomeOther.hpp"
#include "Storage.hpp"

class InstructionFactory
{
public:
    static AbstractInstruction *createInstruction(const uint8_t *sequence, int *sequence_current_index, Storage *storage)
    {
        uint8_t opCode = sequence[(*sequence_current_index)++];
        printf("%x\n", opCode);
        AbstractInstruction *instruction;
        switch (opCode)
        {

        case 0x01:
            instruction = new ADD01(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x83:
            instruction = new ADD83(opCode, sequence, sequence_current_index, storage);
            break;
        case 0x8d:
            instruction = new LEA8D(opCode, sequence, sequence_current_index, storage);
            break;

        default:
            throw std::logic_error("Opcode not implemented");
            break;
        }
        return instruction;
    }


private:
    // Disallow creating an instance of this object
    InstructionFactory() {}
};

#endif