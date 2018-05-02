/*
	z80.h

	Copyright 2018 Nolan Varani

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#pragma once
#include "globaldefs.h"
#include <stdbool.h>
#include <stdio.h>

#define Z80_CYCLES 0x400000

typedef byte* reg8;
typedef word* reg16;

extern byte registers[];

extern reg8 A, F, B, C, D, E, H, L;
extern reg16 AF, BC, DE, HL, SP, PC;

extern unsigned cycles;
extern bool isHalting, isStopped;
extern bool prefixCB;
extern bool interruptsEnabled, pendingDI, pendingEI;

typedef enum conditional
{
	NOTZERO,
	ZERO,
	NOTCARRY,
	CARRY
} conditional;

//generic CPU ops
//TODO: refactor opcode funcs that take params as inline
//8 bit loads
void ld_nn_n(reg8);
void ld_reg8_reg8(reg8,reg8);
void ld_reg8_mem(reg8);
void ld_mem_reg8(reg8);
void ld_mem_n(void);
void ld_a_mem(reg16);
void ld_a_nnmem(void);
void ld_mem_a(reg16);
void ld_nnmem_a(void);
void ld_a_c_relative(void);
void ld_c_relative_a(void);
void ld_a_hld(void);
void ld_hld_a(void);
void ld_a_hli(void);
void ld_hli_a(void);
void ldh_n_a(void);
void ldh_a_n(void);

//16 bit loads
void ld_n_nn(reg16);
void ld_sp_hl(void);
void ldhl_sp_n(void);
void ld_nn_sp(void);
void push_nn(reg16);
void pop_nn(reg16);

//8 bit ALU
void add_a_n(void);
void add_a_reg8(reg8);
void add_a_hl(void);
void adc_a_n(void);
void adc_a_reg8(reg8);
void adc_a_hl(void);
void sub_a_n(void);
void sub_a_reg8(reg8);
void sub_a_hl(void);
void sbc_a_n(void);
void sbc_a_reg8(reg8);
void sbc_a_hl(void);
void and_n(void);
void and_reg8(reg8);
void and_hl(void);
void or_n(void);
void or_reg8(reg8);
void or_hl(void);
void xor_n(void);
void xor_reg8(reg8);
void xor_hl(void);
void cp_n(void);
void cp_reg8(reg8);
void cp_hl(void);
void inc_reg8(reg8);
void inc_hl(void);
void dec_reg8(reg8);
void dec_hl(void);

//16 bit ALU
void add_hl_reg16(reg16);
void add_sp_n(void);
void inc_reg16(reg16);
void dec_reg16(reg16);

//Misc
void swap_reg8(reg8);
void swap_hl(void);
void daa(void);
void cpl(void);
void ccf(void);
void scf(void);
void nop(void);
void halt(void);
void stop(void);
void di(void);
void ei(void);

//Rotates + shifts
void rlc_reg8(reg8);
void rlc_hl(void);
void rlca(void);
void rl_reg8(reg8);
void rl_hl(void);
void rla(void);
void rrc_reg8(reg8);
void rrc_hl(void);
void rrca(void);
void rr_reg8(reg8);
void rr_hl(void);
void rra(void);
void sla_reg8(reg8);
void sla_hl(void);
void sra_reg8(reg8);
void sra_hl(void);
void srl_reg8(reg8);
void srl_hl(void);

//Bit opcodes
void bit_reg8(byte, reg8);
void bit_hl(byte);
void set_reg8(byte, reg8);
void set_hl(byte);
void res_reg8(byte, reg8);
void res_hl(byte);

//Jumps
void jp(void);
void jp_hl(void);
void jp_cc(conditional);
void jr(void);
void jr_cc(conditional);

//Calls
void call(void);
void call_cc(conditional);

//Restarts
void rst(byte);

//Returns
void ret(void);
void ret_cc(conditional);
void reti(void);

//Prefix CB
void prefix_cb(void);


//flag operations and other inline funcs
static inline bool calc_carry_8(byte, byte, bool);
static inline bool calc_halfcarry_16(word, word, bool);
static inline bool calc_carry_8(byte, byte, bool);
static inline bool calc_halfcarry_16(word, word, bool);
static inline void set_flag(byte, bool);
static inline void set_zero(bool);
static inline void set_subtract(bool);
static inline void set_halfcarry(bool);
static inline void set_carry(bool);
static inline bool get_flag(byte);
static inline bool get_zero(void);
static inline bool get_subtract(void);
static inline bool get_halfcarry(void);
static inline bool get_carry(void);

static inline word do_signed_add_reg16_byte(reg16, byte);
static inline word do_signed_add_word_byte(word, byte);

//inline function defs
static inline void set_flag(byte flag, bool set)
{
	if(set) *F |= (1 << flag);
	else *F &= ~(1 << flag);
}
static inline void set_carry(bool set)
{
	set_flag(4, set);
}
static inline void set_halfcarry(bool set)
{
	set_flag(5, set);
}
static inline void set_subtract(bool set)
{
	set_flag(6, set);
}
static inline void set_zero(bool set)
{
	set_flag(7, set);
}
static inline bool get_flag(byte flag)
{
	return *F & (1 << flag);
}
static inline bool get_carry()
{
	return get_flag(4);
}
static inline bool get_halfcarry()
{
	return get_flag(5);
}
static inline bool get_subtract()
{
	return get_flag(6);
}
static inline bool get_zero()
{
	return get_flag(7);
}

static inline bool calc_carry_8(byte result, byte registervalue, bool isSubtract)
{
	if(!isSubtract)
	{
		if(result < registervalue)
		{
			set_carry(true);
			return true;
		}
	}
	else
	{
		if(result > registervalue)
		{
			set_carry(true);
			return true;
		}
	}
	return false;
}
static inline bool calc_carry_16(word result, word registervalue, bool isSubtract)
{
	if(!isSubtract) return result < registervalue;
	else return result > registervalue;
}

static inline bool calc_halfcarry_8(byte result, byte registervalue, bool isSubtract)
{
	if(!isSubtract)
	{
		if((result & 0xF) < (registervalue & 0xF))
		{
			return true;
		}
	}
	else
	{
		if((result & 0xF) > (registervalue & 0xF))
		{
			return true;
		}
	}
	return false;
}
static inline bool calc_halfcarry_16(word a, word b, bool isSubtract)
{
	word res = a + b;
	return (res ^ a ^ b) & 0x1000;
}

static inline word do_signed_add_reg16_byte(reg16 dest, byte value)
{
	word result;
	if(!(value & (1 << 7))) result = *dest + value;
	else result = *dest - ((~value & 0xFF) + 1);
	return result;
}
static inline word do_signed_add_word_byte(word dest, byte value)
{
	if(!(value & (1 << 7))) dest += value;
	else dest -= (~value & 0xFF) + 1;
	return dest;
}
