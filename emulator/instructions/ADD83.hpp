#ifndef ADD83_H
#define ADD83_H

#include <iostream>

#include "AbstractInstruction.hpp"
#include "../ModRM.hpp"

class ADD83 : public AbstractInstruction
{
private:
    ModRM* modRMByte;
    static const int register_operand_size = 32;
public:
    // ADD83(uint8_t opCode, int* seq_idx) : AbstractInstruction(opCode, seq_idx){};
    using AbstractInstruction::AbstractInstruction;     
    void execute()
    {
        std::cout << "executing ADD83\n";
        modRMByte = new ModRM(register_operand_size, this->sequence,this->sequence_current_index, this->storage);
        StorageArgs storageArgs = modRMByte->getModRM();
        const int32_t imm = (int32_t)(this->getImmediateValue<uint8_t>());
        int32_t result = imm + this->storage->load<int32_t>(storageArgs);
        std::cout << "result = "<< result << std::endl;
        this->storage->save<int32_t>(result, storageArgs);
        free(modRMByte);
        
        
    }
    ~ADD83(){};
};

#endif