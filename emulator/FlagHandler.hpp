#ifndef FlagHandler_H
#define FlagHandler_H

#include "Storage.hpp"

#include <string>
#include <math.h>

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
        uint8_t lsbyte = result & 0xff; // extract the least significant byte of the result
        bool parity = 1;
        while (lsbyte)
        {
            parity = !parity;
            lsbyte = lsbyte & (lsbyte - 1); // Unset rightmost set bit
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
        if (msb)
        {
            storage->setFlag(SF);
        }
        else
        {
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

    template <class T>
    static bool is32BitAddOveflow(T op1, T op2)
    {
        // overflow occurs if and only if the result has the opposite sign

        int32_t signed_op1 = (int32_t)op1;
        int32_t signed_op2 = (int32_t)op2;
        int32_t result = signed_op1 + signed_op2;
        if (signed_op1 > 0 && signed_op2 > 0 && result < 0)
            return true;
        if (signed_op1 < 0 && signed_op2 < 0 && result > 0)
            return true;
        return false;
    }

    template <class T>
    static bool is32BitSubOveflow(T op1, T op2)
    {
        // overflow occurs if and only if the result has the same sign as the subtrahend

        int32_t signed_op1 = (int32_t)op1;
        int32_t signed_op2 = (int32_t)op2;
        int32_t result = signed_op1 - signed_op2;
        if (signbit(result) ^ signbit(signed_op2))
            return false;
        return true;
    }

    template <class T>
    static bool is32BitAddCarry(T op1, T op2)
    {
        uint32_t unsigned_op1 = (int32_t)op1;
        uint32_t unsigned_op2 = (int32_t)op2;
        if (unsigned_op1 > UINT32_MAX - unsigned_op2)
            return true;
        return false;
    }

    template <class T>
    static bool is32BitSubCarry(T op1, T op2)
    {
        uint32_t unsigned_op1 = (int32_t)op1;
        uint32_t unsigned_op2 = (int32_t)op2;
        if (unsigned_op1 < unsigned_op2)
            return true;
        return false;
    }

    ~FlagHandler(){};
};

#endif