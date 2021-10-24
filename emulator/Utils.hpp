#ifndef Utils_H
#define Utils_H

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
