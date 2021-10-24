#ifndef FlagHandler_H
#define FlagHandler_H

#include "Storage.hpp"

#include <string>

class FlagHandler
{
    private:
        // Disallow creating an instance of this object externally
        FlagHandler();   

        // methods to handle individual flags

    static void handleCF(bool carry, Storage *storage)
    {
        if (carry)
        {
            storage->setFlag(CF);
        }
        else
        {
            storage->resetFlag(CF);
        }
    };

    template <class T>
    static void handlePF(T result, Storage *storage)
    {
        uint8_t lsbyte = result & 0xff;   // extract the least significant byte of the result
        bool parity = 1;
        while (lsbyte)
        {
            parity = !parity;
            lsbyte = lsbyte & (lsbyte - 1);  // Unset rightmost set bit
        }

        if (parity)
        {
            storage->setFlag(PF);
        }
        else
        {
            storage->resetFlag(PF);
        }
    };

    template <class T>
    static void handleZF(T result, Storage *storage)
    {
        if (result == 0b0)
        {
            storage->setFlag(ZF);
        }
        else
        {
            storage->resetFlag(ZF);
        }
    };

    template <class T>
    static void handleSF(T result, Storage *storage)
    {
        T msb = (result >> ((sizeof(T) * 8) - 1));
        if (msb) {
            storage->setFlag(SF);
        }
        else{
            storage->resetFlag(SF);
        }
    };

    static void handleOF(bool overflow, Storage *storage)
    {
        if (overflow)
        {
            storage->setFlag(OF);
        }
        else
        {
            storage->resetFlag(OF);
        }
    };

    public:
        template <class T>
        static void setFlags(T result,
            Storage *storage, 
            enum FlagType flagsAffected[], 
            int flagsAffectedCount, 
            bool carry = false, 
            bool oveflow = false)
        {
            for (int i = 0; i < flagsAffectedCount; i++)
            {
                std::cout << "flag: " << flagsAffected[i] << std::endl;

                FlagType flagType = flagsAffected[i];

                switch (flagType)
                {
                case CF:
                    handleCF(carry, storage);
                    break;

                case ZF:
                    handleZF(result, storage);
                    break;

                default:
                    printf("Unknown Flag Type %x\n", flagType);
                    throw std::logic_error("Flag not implemented");
                    break;
                }
            }
        }

        ~FlagHandler(){};
};

#endif