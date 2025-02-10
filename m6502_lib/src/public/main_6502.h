#ifndef MAIN_6502_H
#define MAIN_6502_H

#pragma once
#include <stdio.h>
#include <stdlib.h>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Init_mem()
	{
		for (u32 i = 0; i < MAX_MEM; i++)
		{
			Data[i] = 0;
		}
	}

	/**
	 * read 1 byte 
	 */
	Byte operator[](u32 Address) const
	{
		// assert here teh address is < MAX_MEM
		return Data[Address];
	}

    /** write 2 bytes */
    void WriteWord(Word Value, u32 Address, u32 &Cycles)
    {
        Data[Address] = Value & 0xFF;
        Data[Address] = (Value >> 8);
        Cycles -= 2;
    }

	/**Validating the test */
	Byte &operator[](u32 Address)
	{
		return Data[Address];
	}

};

struct CPU
{
	Word PC;	// program counter
	Word SP;	// stack pointer

	Byte A, X, Y;	// registers

	// Flags
	Byte C : 1;
	Byte Z : 1;
	Byte I : 1;
	Byte D : 1;
	Byte B : 1;
	Byte V : 1;
	Byte N : 1;

	void Reset(Mem &memory)
	{
		PC = 0xFFFC;
		SP = 0x0100;
		C = Z = I = B = V = N = D = 0;
		A = X = Y = 0;

        memory.Init_mem();
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
        // is a little endian
        Word Data = memory[PC];
        PC++;

        Data |= (memory[PC] << 8);
        PC++;

        Cycles += 2;


        return Data;
    }

	Byte ReadByte(u32 &Cycles, Byte Addr, Mem &memory)
	{
		Byte Data = memory[Addr];
		Cycles--;

		return Data;
	}

	// opcodes
	static constexpr Byte
		INS_LDA_IM = 0xA9,
		INS_LDA_ZP = 0xA5,
		INS_LDA_ZPX = 0xB5,

        INS_JSR = 0x20;

	void LDASetStatus()
	{
		Z = (A == 0);
		N = (A & 0b10000000) > 0;
	}

	void Exec(u32 Cycles, Mem &memory)
	{
		while (Cycles > 0)
		{
			// TODO:
			// fetch instruction
			Byte Ins = FetchByte(Cycles, memory);
			switch (Ins)
			{
				case INS_LDA_IM:
				{
					Byte Value = FetchByte(Cycles, memory);
					A = Value;
					LDASetStatus();
				}
				break;

				case INS_LDA_ZP:
				{
					Byte ZeroPageAddr = FetchByte(Cycles, memory);

					A = ReadByte(Cycles, ZeroPageAddr, memory);
					LDASetStatus();
				}
				break;

				case INS_LDA_ZPX:
				{
					Byte ZeroPageAddr = FetchByte(Cycles, memory);
					ZeroPageAddr += X;
					Cycles --;
					A = ReadByte(Cycles, ZeroPageAddr, memory);
					LDASetStatus();
				}
				break;

                case INS_JSR:
                {
                    Word SubAddr = FetchWord(Cycles, memory);
                    memory.WriteWord(PC - 1, SP, Cycles);
                    SP += 2;
                    PC = SubAddr;
                    Cycles--;
                }
                break;

				default:
				{
					printf("Instruction not handled %d", Ins);
				}
			}
		}

	}
};


#endif  /* MAIN_6502_H*/
