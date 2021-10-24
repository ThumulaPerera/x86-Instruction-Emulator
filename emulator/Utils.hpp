#ifndef Utils_H
#define Utils_H

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cstdio>

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

#define AL 0
#define CL 1
#define DL 2
#define BL 3
#define AH 4
#define CH 5
#define DH 6
#define BH 7

#define AX 0
#define CX 1
#define DX 2
#define BX 3
#define SP 4
#define BP 5
#define SI 6
#define DI 7

enum StorageType
{
    R8,
    R16,
    R32,
    MEMORY,
};

struct StorageArgs
{
    enum StorageType storage_type;
    int32_t address;
};

struct MemoryBlock
{
    uint32_t address;
    int8_t value;
};

enum FlagType
{
    CF = 0,
    PF = 2,
    AF = 4,
    ZF = 6,
    SF = 7,
    TF = 8,
    IF = 9,
    DF = 10,
    OF = 11,
};

struct StorageRawArgs
{
    enum StorageType storage_type;
    int32_t direct_reg;
    int32_t base_reg;
    int32_t scale_reg;
    int32_t scale_factor;
    int32_t displacement;
    bool has_base;
    bool has_scale;
    bool has_scale_factor;
    bool has_displacement;
};

const std::string Register8Names[8] = {"%al", "%cl", "%dl", "%bl", "%ah", "%ch", "%dh", "%bh"};
const std::string Register16Names[8] = {"%ax", "%cx", "%dx", "%bx", "%sp", "%bp", "%si", "%di"};
const std::string Register32Names[8] = {"%eax", "%ecx", "%edx", "%ebx", "%esp", "%ebp", "%esi", "%edi"};

template <typename T>
std::string intToHexString(T value)
{
    if (sizeof(T) == 1)
    {
        char buff[5];
        sprintf(buff, "0x%02x", (unsigned char)value);
        return std::string(buff);
    }

    std::stringstream sstream;
    sstream << std::hex << std::setw(sizeof(T) * 2) << std::setfill('0') << value;
    return "0x" + sstream.str();
}

std::string stringifyStorageRawArgs(struct StorageRawArgs args)
{
    switch (args.storage_type)
    {
    case R8:
        return Register8Names[args.direct_reg];
        break;

    case R16:
        return Register16Names[args.direct_reg];
        break;
    case R32:
        return Register32Names[args.direct_reg];
        break;

    case MEMORY:
    {
        std::string output = "(";
        if (args.has_base)
        {
            output += " " + Register32Names[args.base_reg];
        }

        if (args.has_scale)
        {
            if (args.has_base)
            {
                output += " +";
            }

            output += " " + Register32Names[args.scale_reg];
            if (args.has_scale_factor)
            {

                output += "*" + std::to_string(args.scale_factor);
            }
        }

        if (args.has_displacement)
        {
            if (args.has_base || args.has_scale)
            {
                output += " +";
            }

            output += " " + intToHexString<int32_t>(args.displacement);
        }
        output += " )";

        return output;

        break;
    }

    default:
        throw std::logic_error("Stringifying unknown storage access\n");
        break;
    }
}

// enum StorageId
// {

//     EAX,
//     ECX,
//     EDX,
//     EBX,
//     ESP,
//     EBP,
//     ESI,
//     EDI,

// };

#endif
