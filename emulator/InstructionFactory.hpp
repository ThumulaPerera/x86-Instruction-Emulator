#ifndef InstructionFactory_H
#define InstructionFactory_H

#include <stdexcept>
#include <cstdio>

#include "instructions/AbstractInstruction.hpp"
#include "instructions/ADD00.hpp"
#include "instructions/ADD01.hpp"
#include "instructions/ADD05.hpp"
#include "instructions/IMM80.hpp"
#include "instructions/IMM81.hpp"
#include "instructions/IMM83.hpp"
#include "instructions/LEA8D.hpp"
#include "instructions/POP.hpp"
#include "instructions/PUSH.hpp"
#include "instructions/INCDECFF.hpp"
#include "instructions/MOV89.hpp"
#include "instructions/MOV8B.hpp"
#include "instructions/MOVIMM.hpp"
#include "instructions/MULF7.hpp"

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

        case 0x80:
            instruction = new IMM80(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x81:
            instruction = new IMM81(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x83:
            instruction = new IMM83(opCode, sequence, sequence_current_index, storage);
            break;
        case 0x8d:
            instruction = new LEA8D(opCode, sequence, sequence_current_index, storage);
            break;

        case 0xff:
            instruction = new INCDECFF(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x89:
            instruction = new MOV89(opCode, sequence, sequence_current_index, storage);
            break;

        case 0x8b:
            instruction = new MOV8B(opCode, sequence, sequence_current_index, storage);
            break;

        case 0xb8 ... 0xbf:
            instruction = new MOVIMM(opCode, sequence, sequence_current_index, storage);
            break;

        case 0xf7:
            instruction = new MULF7(opCode, sequence, sequence_current_index, storage);
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