#ifndef LEA8D_H
#define LEA8D_H

#include "AbstractInstruction.hpp"


class LEA8D : public AbstractInstruction
{
private:
    /* data */
public:
    // LEA8D(uint8_t opCode) : AbstractInstruction(opCode){};
    using AbstractInstruction::AbstractInstruction;
    void execute()
    {
        std::cout << "executing LEA8D\n";
    }
    ~LEA8D(){};
};

#endif