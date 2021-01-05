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
	//** write 1 byte **//
	Byte &operator[](u32 Address)
	{
		//assert here Address is < MAX_MEM
		return Data[Address];
	}
	//** write 2 bytes **//
	void WriteWord(u32 Cycles,Word Value, u32 Address)
	{
		Data[Address] = Value & 0xFF;
		Data[Address+1] = (Value >> 8 );
		Cycles -=2;

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

		Byte FetchByte(u32 &Cycles, Mem &memory)
		{
			Byte Data = memory[PC];
			PC++;
			Cycles--;
			return Data;
		}
		Word FetchWord(u32 &Cycles, Mem &memory)
		{
			//6502 is little endian
			Word Data = memory[PC];
			PC++;

			Data |= (memory[PC] << 8);
			PC++;

			Cycles -= 2;

			//if you wanted to handle endianess
			//you would have to swap bytes here
			// IF (PLATFORM_BIG_ENDIAN) swap bytes in Word

			return Data;
		}
		Byte ReadByte(u32 &Cycles, Byte Address, Mem &memory)
		{
			Byte Data = memory[Address];
			Cycles--;
			return Data;
		}
		//opcodes
		static constexpr Byte
			INS_LDA_IM = 0xA9,
			INS_LDA_ZP = 0XA5,
			INS_LDA_ZPX = 0XB5,
			INS_JSR = 0x20;
		void LDASetStatus()
		{
			Z = (A == 0);
			N = (A & 0b10000000) > 0;
		}

		void Execute(u32 Cycles, Mem &memory)
		{
			while (Cycles > 0)
			{
				Byte Ins = FetchByte(Cycles, memory);
				switch (Ins)
				{
				case INS_LDA_IM:
				{
					Byte Value = FetchByte(Cycles, memory);
					A = Value;
					LDASetStatus();
				}
				case INS_LDA_ZP:
				{
					Byte ZeroPageAddr = FetchByte(Cycles, memory);
					ReadByte(Cycles, ZeroPageAddr, memory);
					LDASetStatus();
				}
				break;
				case INS_LDA_ZPX:
				{
					Byte ZeroPageAddr = FetchByte(Cycles, memory);
					ZeroPageAddr += X;
					Cycles--;
					A = ReadByte(Cycles, ZeroPageAddr, memory);
					LDASetStatus(); //TODO: NO OVERFLOW Control
				}
				break;
				case INS_JSR:
				{
					Word SubAddr = FetchWord(Cycles, memory);
					memory[SP] = PC - 1;
					Cycles--;
					PC = SubAddr;
					Cycles--;
				}
				break;
				default:
				{
					printf("Instruction not handled %d", Ins);
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
		//inline little program
		mem[0xFFFC] = CPU::INS_LDA_ZP;
		mem[0xFFFD] = 0x42;
		mem[0x0042] = 0x84;
		//inline little program
		cpu.Execute(3, mem); //TODO Need to find suitable debugger
		printf("Program finished");
		return 0;
	}