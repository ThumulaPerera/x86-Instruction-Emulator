#ifndef ADD83_H
#define ADD83_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class ADD83 : public AbstractInstruction
{
private:
    ModRM* modRMByte;
public:
    // ADD83(uint8_t opCode, int* seq_idx) : AbstractInstruction(opCode, seq_idx){};
    using AbstractInstruction::AbstractInstruction;     
    void execute()
    {
        std::cout << "executing ADD83\n";
        modRMByte = new ModRM(this->sequence,this->sequence_current_index);
        const int32_t imm = (int32_t)(this->getImmediateValue<uint8_t>());
        StorageArgs storageArgs = modRMByte->getModRM();
        int32_t result = imm + this->storage->load<int32_t>(storageArgs);
        std::cout << "result = "<< result << std::endl;
        this->storage->save<int32_t>(result, storageArgs);
        free(modRMByte);
        
        
    }
    ~ADD83(){};
};

#endif