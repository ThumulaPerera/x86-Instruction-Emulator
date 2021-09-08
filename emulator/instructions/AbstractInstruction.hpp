
#ifndef AbstractInstruction_H
#define AbstractInstruction_H

#include "../Storage.hpp"

class AbstractInstruction
{
private:
    /* data */
protected:
    uint8_t opCode;
    int *sequence_current_index;
    Storage *storage;

public:
    AbstractInstruction(uint8_t opCode, int *seq_idx, Storage *storage)
        : opCode(opCode), sequence_current_index(seq_idx), storage(storage){};
    virtual ~AbstractInstruction(){};
    virtual void execute(){};
};

#endif