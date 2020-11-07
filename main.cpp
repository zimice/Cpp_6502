#include <stdio.h>
#include <stdlib.h>


struct Mem 
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];
};

struct CPU
{
    using Byte = unsigned char;
    using Word = unsigned short;

    Word PC; //Program Counter
    Word SP; //Stack pointer

    Byte A, X, Y; // Registers

    Byte C : 1; //Status flag
    Byte Z : 1; 
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte V : 1;
    Byte N : 1;

    void Reset()
    {
        PC = 0xFFFC;
        SP = 0x100;
        D = 0;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;

    }
};

int main()
{

    CPU cpu;
    cpu.Reset();
    return 0;
}