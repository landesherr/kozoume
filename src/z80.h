#pragma once
#include "globaldefs.h"
#include <stdbool.h>

typedef byte* reg8;
typedef word* reg16;

extern byte registers[];

extern reg8 A, F, B, C, D, E, H, L;
extern reg16 AF, BC, DE, HL, SP, PC;

extern unsigned cycles;
extern bool isHalting, isStopped;
extern bool interruptsEnabled;

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
void add sp_n(void);
void inc_reg16(reg16);
void dec_reg16(reg16);

//Misc
void swap_reg8(reg8);
void swap_hl(void);
void daa(void);
void ccf(void);
void scf(void);
void nop(void);
void halt(void);
void stop(void);
void di(void);
void ei(void);

/*
//Rotates + shifts
void rlca(void);
void rla(void);
void rrca(void);
void rra(void);
void rlc_reg8(reg8);
void rlc_hl(void);
void rl_reg8(reg8);
void rl_hl(void);
void rrc_reg8(reg8);
void rrc_hl(void);
void rr_reg8(void);
void rr_hl(void);
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
void jp_nz(void);
void jp_z(void);
void jp_nc(void);
void jp_c(void);
void jp_hl(void);
void jr(void);
void jr_nz(void);
void jr_z(void);
void jr_nc(void);
void jr_c(void);

//Calls
void call(void);
void call_nz(void);
void call_z(void);
void call_nc(void);
void call_c(void);

//Restarts
void rst(void);

//Returns
void ret(void);
void ret_nz(void);
void ret_z(void);
void ret_nc(void);
void ret_c(void);
void reti(void);
*/

//flag operations and other inline funcs
inline void calc_carry_8(byte, byte, bool);
inline void calc_halfcarry_16(word, word, bool);
inline void calc_carry_8(byte, byte, bool);
inline void calc_halfcarry_16(word, word, bool);
inline void set_flag(byte, bool);
inline void set_zero(bool);
inline void set_subtract(bool);
inline void set_halfcarry(bool);
inline void set_carry(bool);
inline bool get_flag(byte);
inline bool get_zero(void);
inline bool get_subtract(void);
inline bool get_halfcarry(void);
inline bool get_carry(void);

inline word do_signed_add_reg16_byte(reg16, signed char);

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
inline bool get_flag(byte flag)
{
	return *F & (1 << flag);
}
inline bool get_carry()
{
	return get_flag(4);
}
inline bool get_halfcarry()
{
	return get_flag(5);
}
inline bool get_subtract()
{
	return get_flag(6);
}
inline bool get_zero()
{
	return get_flag(7);
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

inline word do_signed_add_reg16_byte(reg16 dest, signed char value)
{
	word result;
	if(value > 0)
	{
		result = *dest + value;
	}
	else if(value < 0)
	{
		isSubtract = true;
		result = *dest - (0 - value);
	}
	return result;
}
