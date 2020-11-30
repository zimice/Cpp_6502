#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Initialise()
    {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }
    //** read 1 byte **//
    Byte operator[](u32 Address) const
    {
        //assert here Address is < MAX_MEM
        return Data[Address];
    }
};

struct CPU
{

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

    void Reset(Mem &memory)
    {
        PC = 0xFFFC;
        SP = 0x100;
        C = Z = I = D = B = V = N = D = 0;
        A = X = Y = 0;
        memory.Initialise();
    }

    Byte FetchByte(u32 Cycles, Mem &memory)
    {
        Byte Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }
    //opcodes
    static constexpr Byte
        INS_LDA_IM = 0xA9;

    void Execute(u32 Cycles, Mem &memory)
    {
        while (Cycles > 0)
        {
            Byte Ins = FetchByte(Cycles, memory);
            switch (Ins)
            {
            case INS_LDA_IM:
            {
               Byte Value =  FetchByte( Cycles, memory);
            }
            break;
            }
        }
    }
};

int main()
{
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    cpu.Execute(2, mem);
    return 0;
}