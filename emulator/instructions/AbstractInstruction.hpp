
#ifndef AbstractInstruction_H
#define AbstractInstruction_H

#include <cstring>
#include "../Storage.hpp"

class AbstractInstruction
{
private:
    /* data */
protected:
    uint8_t opCode;
    const uint8_t *sequence;
    int *sequence_current_index;
    Storage *storage;

public:
    AbstractInstruction(uint8_t opCode, const uint8_t *sequence, int *seq_idx, Storage *storage)
        : opCode(opCode), sequence(sequence), sequence_current_index(seq_idx), storage(storage){};

    template <class T>
    T getImmediateValue()
    {
        T output = 0;
        std::memcpy(&output, &(this->sequence[*this->sequence_current_index]), sizeof(T));
        (*this->sequence_current_index) += sizeof(int32_t);
        return output;
    }
    virtual ~AbstractInstruction(){};
    virtual void execute(){};
};

#endif