#ifndef FlagHandler_H
#define FlagHandler_H

#include "Storage.hpp"

#include <string>
#include <math.h>
#include <vector>

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
                         std::vector<FlagType> flagsAffected,
                         bool carry = false,
                         bool oveflow = false)
    {
        for (FlagType flagType : flagsAffected)
        {
            // std::cout << "flag: " << flagType << std::endl;
            switch (flagType)
            {
            case CF:
                handleCF(carry, storage);
                break;

            case PF:
                handlePF(result, storage);
                break;

            case ZF:
                handleZF(result, storage);
                break;

            case SF:
                handleSF(result, storage);
                break;

            case OF:
                handleOF(oveflow, storage);
                break;

            default:
                printf("Unknown Flag Type %x\n", flagType);
                throw std::logic_error("Flag not implemented");
                break;
            }
        }
    }

    // template <class T>
    // static bool is32BitAddOveflow(T op1, T op2)
    template <class T>
    static bool isAddOverflow(T op1, T op2)
    {
        // overflow occurs if and only if the result has the opposite sign

        T signed_op1 = (T)op1;
        T signed_op2 = (T)op2;
        T result = signed_op1 + signed_op2;
        if (signed_op1 > 0 && signed_op2 > 0 && result < 0)
            return true;
        if (signed_op1 < 0 && signed_op2 < 0 && result > 0)
            return true;
        return false;
    }

    // template <class T>
    // static bool is32BitSubOveflow(T op1, T op2)
    template <class T>
    static bool isSubOverflow(T op1, T op2)
    {
        // overflow occurs if and only if the result has the same sign as the subtrahend

        T signed_op1 = (T)op1;
        T signed_op2 = (T)op2;
        T result = signed_op1 - signed_op2;
        if (signbit(result) ^ signbit(signed_op2))
            return false;
        return true;
    }

    // template <class T>
    // static bool is32BitAddCarry(T op1, T op2)
    template <class T, class UT>
    static bool isAddCarry(T op1, T op2)
    {
        UT unsigned_op1 = (UT)op1;
        UT unsigned_op2 = (UT)op2;
        UT max_value;
        switch (sizeof(UT))
        {
        case 4:
            max_value = UINT32_MAX;
            break;
        case 2:
            max_value = UINT16_MAX;
            break;

        case 1:
            max_value = UINT8_MAX;
            break;

        default:
            throw std::logic_error("Error in flag handler.");
            break;
        }
        if (unsigned_op1 > max_value - unsigned_op2)
            return true;
        return false;
    }

    // template <class T>
    // static bool is32BitSubCarry(T op1, T op2)
    template <class T, class UT>
    static bool isSubCarry(T op1, T op2)
    {
        UT unsigned_op1 = (UT)op1;
        UT unsigned_op2 = (UT)op2;
        if (unsigned_op1 < unsigned_op2)
            return true;
        return false;
    }

    ~FlagHandler(){};
};

#endif