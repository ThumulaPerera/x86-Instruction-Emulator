#ifndef SomeOther_H
#define SomeOther_H

#include "AbstractInstruction.hpp"

class SomeOther : public AbstractInstruction
{
private:
    /* data */
public:
    // SomeOther(uint8_t opCode) : AbstractInstruction(opCode){};
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing SomeOther\n";
    }
    ~SomeOther(){};
};

#endif