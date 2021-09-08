#ifndef ADD83_H
#define ADD83_H

#include "AbstractInstruction.hpp"


class ADD83 : public AbstractInstruction
{
private:
    /* data */
public:
    // ADD83(uint8_t opCode, int* seq_idx) : AbstractInstruction(opCode, seq_idx){};
    using AbstractInstruction::AbstractInstruction;     
    void execute()
    {
        std::cout << "executing ADD83\n";
    }
    ~ADD83(){};
};

#endif