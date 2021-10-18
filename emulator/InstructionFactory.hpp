#ifndef InstructionFactory_H
#define InstructionFactory_H

#include <stdexcept>
#include <cstdio>

#include "instructions/AbstractInstruction.hpp"
#include "instructions/ADD00.hpp"
#include "instructions/ADD01.hpp"
#include "instructions/ADD05.hpp"
#include "instructions/ADD81.hpp"
#include "instructions/IMM83.hpp"
#include "instructions/LEA8D.hpp"
#include "instructions/POP.hpp"
#include "instructions/PUSH.hpp"

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

        case 0x00:
            instruction = new ADD00(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x01:
            instruction = new ADD01(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x05:
            instruction = new ADD05(opCode, sequence, sequence_current_index, storage);
            break;
        case 0x50 ... 0x57:
            instruction = new PUSH(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x58 ... 0x5f:
            instruction = new POP(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x81:
            instruction = new ADD81(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x83:
            instruction = new IMM83(opCode, sequence, sequence_current_index, storage);
            break;
        case 0x8d:
            instruction = new LEA8D(opCode, sequence, sequence_current_index, storage);
            break;

        default:
            printf("Unknown opcode %x\n", opCode);
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