#ifndef ADD81_H
#define ADD81_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class ADD81 : public AbstractInstruction
{
private:
    ModRM* modRMByte;
    static const int register_operand_size = 32;
public:
    // ADD81(uint8_t opCode, int* seq_idx) : AbstractInstruction(opCode, seq_idx){};
    using AbstractInstruction::AbstractInstruction;     
    void execute()
    {
        std::cout << "executing ADD81\n";
        modRMByte = new ModRM(register_operand_size, this->sequence,this->sequence_current_index, this->storage);
        StorageArgs storageArgs = modRMByte->getModRM();
        const int32_t imm = this->getImmediateValue<int32_t>();
        int32_t result = imm + this->storage->load<int32_t>(storageArgs);
        std::cout << "result = "<< result << std::endl;
        this->storage->save<int32_t>(result, storageArgs);
        free(modRMByte);
        
        
    }
    ~ADD81(){};
};

#endif