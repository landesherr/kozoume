#pragma once
#include "globaldefs.h"
#include <stdbool.h>

typedef byte* reg8;
typedef word* reg16;

extern byte registers[];

extern reg8 A, F, B, C, D, E, H, L;
extern reg16 AF, BC, DE, HL, SP, PC;

extern unsigned cycles;

//generic CPU ops
void ld_nn_n(reg8);
void add_a_n(void);

//flag operations
inline void calc_carry_8(byte, byte, bool);
inline void calc_halfcarry_16(word, word, bool);
inline void calc_carry_8(byte, byte, bool);
inline void calc_halfcarry_16(word, word, bool);
inline void set_flag(byte, bool);
inline void set_zero(bool);
inline void set_subtract(bool);
inline void set_halfcarry(bool);
inline void set_carry(bool);

//inline function defs
inline void set_flag(byte flag, bool set)
{
	if(set) *F |= (1 << flag);
	else *F &= ~(1 << flag);
}
inline void set_carry(bool set)
{
	set_flag(4, set);
}
inline void set_halfcarry(bool set)
{
	set_flag(5, set);
}
inline void set_subtract(bool set)
{
	set_flag(6, set);
}
inline void set_zero(bool set)
{
	set_flag(7, set);
}

inline void calc_carry_8(byte result, byte registervalue, bool isSubtract)
{
	if(!isSubtract)
	{
		if(result < registervalue) set_carry(true);
	}
	else
	{
		if(result > registervalue) set_carry(true);
	}
}
inline void calc_carry_16(word result, word registervalue, bool isSubtract)
{
	calc_carry_8((byte)(result >> 8), (byte)(registervalue >> 8), isSubtract);
}

inline void calc_halfcarry_8(byte result, byte registervalue, bool isSubtract)
{
	if(!isSubtract)
	{
		if((result & 0xF) < (registervalue & 0xF)) set_halfcarry(true);
	}
	else
	{
		if((result & 0xF) > (registervalue & 0xF)) set_halfcarry(true);
	}
}
inline void calc_halfcarry_16(word result, word registervalue, bool isSubtract)
{
	calc_halfcarry_8((byte)(result >> 8), (byte)(registervalue >> 8), isSubtract);
}