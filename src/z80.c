/*
	z80.c

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
#include "globaldefs.h"
#include "z80.h"
#include "memory.h"
#include <stdbool.h>

byte registers[12];

reg8 A = &registers[1];
reg8 F = &registers[0];
reg8 B = &registers[3];
reg8 C = &registers[2];
reg8 D = &registers[5];
reg8 E = &registers[4];
reg8 H = &registers[7];
reg8 L = &registers[6];

reg16 AF = (word*) &registers[0];
reg16 BC = (word*) &registers[2];
reg16 DE = (word*) &registers[4];
reg16 HL = (word*) &registers[6];
reg16 SP = (word*) &registers[8];
reg16 PC = (word*) &registers[10];

unsigned cycles = 0;
bool isHalting = false, isStopped = false;
bool interruptsEnabled = false, pendingDI = false, pendingEI = false;
bool prefixCB = false;

//8-BIT LOADS
void ld_nn_n(reg8 reg)
{
	*reg = memory_get8(*PC + 1);
	*PC += 2;
	cycles += 8;
}
void ld_reg8_reg8(reg8 dest, reg8 source)
{
	*dest = *source;
	*PC += 1;
	cycles += 4;
}
void ld_reg8_mem(reg8 dest)
{
	*dest = memory_get8(*HL);
	*PC += 1;
	cycles += 8;
}
void ld_mem_reg8(reg8 source)
{
	memory_set8_logical(*HL, *source);
	*PC += 1;
	cycles += 8;
}
void ld_mem_n()
{
	memory_set8_logical(*HL, memory_get8(*PC + 1));
	*PC += 2;
	cycles += 12;
}
void ld_a_mem(reg16 address)
{
	*A = memory_get8(*address);
	*PC += 1;
	cycles += 8;
}
void ld_a_nnmem()
{
	*A = memory_get8(memory_get16(*PC + 1));
	*PC += 3;
	cycles += 16;
}
void ld_mem_a(reg16 address)
{
	memory_set8_logical(*address, *A);
	*PC += 1;
	cycles += 8;
}
void ld_nnmem_a()
{
	memory_set8_logical(memory_get16(*PC + 1), *A);
	*PC += 3;
	cycles += 16;
}
void ld_a_c_relative()
{
	*A = memory_get8(0xFF00 + *C);
	*PC += 1;
	cycles += 8;
}
void ld_c_relative_a()
{
	memory_set8_logical(0xFF00 + *C, *A);
	*PC += 1;
	cycles += 8;
}
void ld_a_hld()
{
	*A = memory_get8(*HL);
	*HL -= 1;
	*PC += 1;
	cycles += 8;
}
void ld_hld_a()
{
	memory_set8_logical(*HL,*A);
	*HL -= 1;
	*PC += 1;
	cycles += 8;
}
void ld_a_hli()
{
	*A = memory_get8(*HL);
	*HL += 1;
	*PC += 1;
	cycles += 8;
}
void ld_hli_a()
{
	memory_set8_logical(*HL,*A);
	*HL += 1;
	*PC += 1;
	cycles += 8;
}
void ldh_n_a()
{
	memory_set8_logical(0xFF00 + memory_get8(*PC + 1), *A);
	*PC += 2;
	cycles += 12;
}
void ldh_a_n()
{
	*A = memory_get8(0xFF00 + memory_get8(*PC + 1));
	*PC += 2;
	cycles += 12;
}
//16-BIT LOADS
void ld_n_nn(reg16 dest)
{
	*dest = memory_get16(*PC + 1);
	*PC += 3;
	cycles += 12;
}
void ld_sp_hl()
{
	*SP = *HL;
	*PC += 1;
	cycles += 8;
}
void ldhl_sp_n()
{
	byte tempvalue;
	word result;
	tempvalue = memory_get8(*PC + 1);
	result = do_signed_add_reg16_byte(SP, tempvalue);
	//Lower 8 bits for carry checks
	set_halfcarry(calc_halfcarry_8(result & 0xFF, *SP & 0xFF, false));
	set_carry(calc_carry_8(result & 0xFF, *SP & 0xFF, false));
	set_zero(false);
	set_subtract(false);
	*HL = result;
	*PC += 2;
	cycles += 12;
}
void ld_nn_sp()
{
	memory_set16_logical(memory_get16(*PC + 1), *SP);
	*PC += 3;
	cycles += 20;
}
void push_nn(reg16 reg)
{
	*SP -= 2;
	memory_set16_logical(*SP, *reg);
	*PC += 1;
	cycles += 16;
}
void pop_nn(reg16 reg)
{
	*reg = memory_get16(*SP);
	*SP += 2;
	*PC += 1;
	cycles += 12;
}


//8-BIT ARITHMETIC
void add_a_n()
{
	byte result;
	result = *A + memory_get8(*PC + 1);
	set_halfcarry(calc_halfcarry_8(result, *A, false));
	set_carry(calc_carry_8(result, *A, false));
	set_zero(!result);
	set_subtract(false);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void add_a_reg8(reg8 reg)
{
	byte result = *A + *reg;
	set_halfcarry(calc_halfcarry_8(result, *A, false));
	set_carry(calc_carry_8(result, *A, false));
	set_zero(!result);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void add_a_hl()
{
	byte result = *A + memory_get8(*HL);
	set_halfcarry(calc_halfcarry_8(result, *A, false));
	set_carry(calc_carry_8(result, *A, false));
	set_zero(!result);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void adc_a_n()
{
	byte result;
	bool carry = get_carry();
	result = *A + memory_get8(*PC + 1);
	set_halfcarry(calc_halfcarry_8(result, *A, false));
	set_carry(calc_carry_8(result, *A, false));
	if(carry)
	{
		if(calc_halfcarry_8(result+1, result, false)) set_halfcarry(true);
		if(calc_carry_8(result+1, result, false)) set_carry(true);
		result++;
	}
	set_zero(!result);
	set_subtract(false);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void adc_a_reg8(reg8 reg)
{
	byte result = *A + *reg;
	bool carry = get_carry();
	set_halfcarry(calc_halfcarry_8(result, *A, false));
	set_carry(calc_carry_8(result, *A, false));
	if(carry)
	{
		if(calc_halfcarry_8(result+1, result, false)) set_halfcarry(true);
		if(calc_carry_8(result+1, result, false)) set_carry(true);
		result++;
	}
	set_zero(!result);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void adc_a_hl()
{
	byte result = *A + memory_get8(*HL);
	bool carry = get_carry();
	set_halfcarry(calc_halfcarry_8(result, *A, false));
	set_carry(calc_carry_8(result, *A, false));
	if(carry)
	{
		if(calc_halfcarry_8(result+1, result, false)) set_halfcarry(true);
		if(calc_carry_8(result+1, result, false)) set_carry(true);
		result++;
	}
	set_zero(!result);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void sub_a_n()
{
	byte result = *A - memory_get8(*PC + 1);
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	set_zero(!result);
	set_subtract(true);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void sub_a_reg8(reg8 reg)
{
	byte result = *A - *reg;
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	set_zero(!result);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void sub_a_hl()
{
	byte result = *A - memory_get8(*HL);
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	set_zero(!result);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void sbc_a_n()
{
	byte result = *A - (memory_get8(*PC + 1));
	bool carry = get_carry();
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	if(carry)
	{
		if(calc_halfcarry_8(result-1, result, true)) set_halfcarry(true);
		if(calc_carry_8(result-1, result, true)) set_carry(true);
		result--;
	}
	set_zero(!result);
	set_subtract(true);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void sbc_a_reg8(reg8 reg)
{
	byte result = *A - *reg;
	bool carry = get_carry();
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	if(carry)
	{
		if(calc_halfcarry_8(result-1, result, true)) set_halfcarry(true);
		if(calc_carry_8(result-1, result, true)) set_carry(true);
		result--;
	}
	set_zero(!result);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void sbc_a_hl()
{
	byte result = *A - memory_get8(*HL);
	bool carry = get_carry();
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	if(carry)
	{
		if(calc_halfcarry_8(result-1, result, true)) set_halfcarry(true);
		if(calc_carry_8(result-1, result, true)) set_carry(true);
		result--;
	}
	set_zero(!result);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void cp_n()
{
	byte result = *A - memory_get8(*PC + 1);
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	set_zero(!result);
	set_subtract(true);
	*PC += 2;
	cycles += 8;
}
void cp_reg8(reg8 reg)
{
	byte result = *A - *reg;
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	set_zero(!result);
	set_subtract(true);
	*PC += 1;
	cycles += 4;
}
void cp_hl()
{
	byte result = *A - memory_get8(*HL);
	set_halfcarry(calc_halfcarry_8(result, *A, true));
	set_carry(calc_carry_8(result, *A, true));
	set_zero(!result);
	set_subtract(true);
	*PC += 1;
	cycles += 8;
}
void and_n()
{
	*A &= memory_get8(*PC + 1);
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(true);
	set_carry(false);
	*PC += 2;
	cycles += 8;
}
void and_reg8(reg8 reg)
{
	*A &= *reg;
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(true);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void and_hl()
{
	*A &= memory_get8(*HL);
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(true);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void or_n()
{
	*A |= memory_get8(*PC + 1);
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 2;
	cycles += 8;
}
void or_reg8(reg8 reg)
{
	*A |= *reg;
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void or_hl()
{
	*A |= memory_get8(*HL);
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void xor_n()
{
	*A ^= memory_get8(*PC + 1);
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 2;
	cycles += 8;
}
void xor_reg8(reg8 reg)
{
	*A ^= *reg;
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void xor_hl()
{
	*A ^= memory_get8(*HL);
	set_zero(!*A);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void inc_reg8(reg8 reg)
{
	byte result = *reg + 1;
	set_halfcarry(calc_halfcarry_8(result, *reg, false));
	set_subtract(false);
	set_zero(!result);
	*reg = result;
	*PC += 1;
	cycles += 4;
}
void inc_hl()
{
	byte result = memory_get8(*HL) + 1;
	set_halfcarry(calc_halfcarry_8(result, memory_get8(*HL), false));
	set_subtract(false);
	set_zero(!result);
	memory_set8_logical(*HL, result);
	*PC += 1;
	cycles += 12;
}
void dec_reg8(reg8 reg)
{
	byte result = *reg - 1;
	set_halfcarry(calc_halfcarry_8(result, *reg, true));
	set_subtract(true);
	set_zero(!result);
	*reg = result;
	*PC += 1;
	cycles += 4;
}
void dec_hl()
{
	byte result = memory_get8(*HL) - 1;
	set_halfcarry(calc_halfcarry_8(result, memory_get8(*HL), true));
	set_subtract(true);
	set_zero(!result);
	memory_set8_logical(*HL, result);
	*PC += 1;
	cycles += 12;
}

//16-bit ALU
void add_hl_reg16(reg16 reg)
{
	word result = *HL + *reg;
	set_halfcarry(calc_halfcarry_16(*reg, *HL));
	set_carry(calc_carry_16(result, *HL, false));
	set_subtract(false);
	*HL = result;
	*PC += 1;
	cycles += 8;
}
void add_sp_n()
{
	word result;
	byte tempval = memory_get8(*PC + 1);
	result = do_signed_add_reg16_byte(SP, tempval);
	//Handle carry flags different for this opcode: use lower 8 bits
	set_halfcarry(calc_halfcarry_8(result & 0xFF, *SP & 0xFF, false));
	set_carry(calc_carry_8(result & 0xFF, *SP & 0xFF, false));
	set_zero(false);
	set_subtract(false);
	*SP = result;
	*PC += 2;
	cycles += 16;
}
//No flags affected on 16-bit inc/dec
void inc_reg16(reg16 reg)
{
	*reg += 1;
	*PC += 1;
	cycles += 8;
}
void dec_reg16(reg16 reg)
{
	*reg -= 1;
	*PC += 1;
	cycles += 8;
}

//Misc
void swap_reg8(reg8 reg)
{
	byte temp = *reg & 0xF;
	*reg = (*reg >> 4);
	*reg |= (temp << 4);
	set_zero(!*reg);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void swap_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value & 0xF;
	value = (value >> 4);
	value |= (temp << 4);
	memory_set8_logical(*HL, value);
	set_zero(!value);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 12;
}
void daa()
{
	word value = *A;

	if(get_subtract())
	{
		if(get_halfcarry()) value = (value - 6) & 0xFF;
		if(get_carry()) value -= 0x60;
	}
	else
	{
		if(get_halfcarry() || ( (value & 0xF) > 9)) value += 6;
		if(get_carry() || value > 0x9F) value += 0x60;
	}
	if(value & 0x100) set_carry(true);
	set_halfcarry(false);
	*A = value;
	set_zero(!*A);
	*PC += 1;
	cycles += 4;
}
void cpl()
{
	*A = ~*A;
	set_subtract(true);
	set_halfcarry(true);
	*PC += 1;
	cycles += 4;
}
void ccf()
{
	set_carry(!get_carry());
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 4;
}
void scf()
{
	set_carry(true);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 4;
}
void nop()
{
	*PC += 1;
	cycles += 4;
}
void halt()
{
	isHalting = true;
	*PC += 1;
	cycles += 4;
}
void stop()
{
	isStopped = true;
	*PC += 1;
	cycles += 4;
}
void di()
{
	//pendingDI = true;
	interruptsEnabled = false;
	*PC += 1;
	cycles += 4;
}
void ei()
{
	pendingEI = true;
	*PC += 1;
	cycles += 4;
}

//Rotates + Shifts
void rlc_reg8(reg8 reg)
{
	//8-bit rotation, bit 7 goes to both CF and bit 0
	byte temp = *reg << 1;
	temp |= (*reg >> 7);
	set_zero(!temp);
	set_carry((*reg & 0x80));
	set_subtract(false);
	set_halfcarry(false);
	*reg = temp;
	*PC += 1;
	cycles += 4;
}
void rlc_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value << 1;
	temp |= (value >> 7);
	set_zero(!temp);
	set_carry((value & 0x80));
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, temp);
	*PC += 1;
	cycles += 12;
}
void rlca()
{
	byte temp = *A << 1;
	temp |= (*A >> 7);
	set_carry((*A & 0x80));
	set_subtract(false);
	set_halfcarry(false);
	set_zero(false);
	*A = temp;
	*PC += 1;
	cycles += 4;
}
void rl_reg8(reg8 reg)
{
	//9-bit rotation, CF to bit 0, bit 7 to CF
	byte temp = *reg << 1;
	temp |= get_carry();
	set_zero(!temp);
	set_carry(*reg >> 7);
	set_subtract(false);
	set_halfcarry(false);
	*reg = temp;
	*PC += 1;
	cycles += 4;
}
void rl_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value << 1;
	temp |= get_carry();
	set_zero(!temp);
	set_carry(value >> 7);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, temp);
	*PC += 1;
	cycles += 12;
}
void rla()
{
	byte temp = *A << 1;
	temp |= get_carry();
	set_carry(*A >> 7);
	set_subtract(false);
	set_halfcarry(false);
	set_zero(false);
	*A = temp;
	*PC += 1;
	cycles += 4;
}
void rrc_reg8(reg8 reg)
{
	//8-bit rotation, bit 0 goes to bit 7 and CF
	byte temp = *reg >> 1;
	temp |= (*reg << 7);
	set_zero(!temp);
	set_carry(*reg & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	*reg = temp;
	*PC += 1;
	cycles += 4;
}
void rrc_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value >> 1;
	temp |= (value << 7);
	set_zero(!temp);
	set_carry(value & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, temp);
	*PC += 1;
	cycles += 12;
}
void rrca()
{
	byte temp = *A >> 1;
	temp |= (*A << 7);
	set_carry(*A & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	set_zero(false);
	*A = temp;
	*PC += 1;
	cycles += 4;
}
void rr_reg8(reg8 reg)
{
	//9-bit rotation, CF goes to bit 7, bit 0 to CF
	byte temp = *reg >> 1;
	temp |= get_carry() << 7;
	set_zero(!temp);
	set_carry(*reg & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	*reg = temp;
	*PC += 1;
	cycles += 4;
}
void rr_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value >> 1;
	temp |= get_carry() << 7;
	set_zero(!temp);
	set_carry(value & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, temp);
	*PC += 1;
	cycles += 12;
}
void rra()
{
	byte temp = *A >> 1;
	temp |= get_carry() << 7;
	set_carry(*A & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	set_zero(false);
	*A = temp;
	*PC += 1;
	cycles += 4;
}
void sla_reg8(reg8 reg)
{
	set_carry(*reg & 0x80);
	*reg <<= 1;
	set_zero(!*reg);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 4;
}
void sla_hl()
{
	byte value = memory_get8(*HL);
	set_carry(value & 0x80);
	value <<= 1;
	set_zero(!value);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, value);
	*PC += 1;
	cycles += 12;
}
void sra_reg8(reg8 reg)
{
	//bit 7 remains unchanged
	byte temp = *reg >> 1;
	set_carry(*reg & 0x1);
	*reg = (*reg & 0x80) | temp;
	set_zero(!*reg);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 4;
}
void sra_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value >> 1;
	set_carry(value & 0x1);
	value = (value & 0x80) | temp;
	set_zero(!value);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, value);
	*PC += 1;
	cycles += 12;
}
void srl_reg8(reg8 reg)
{
	//bit 7 reset to 0
	set_carry(*reg & 0x1);
	*reg >>= 1;
	set_zero(!*reg);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 4;
}
void srl_hl()
{
	byte value = memory_get8(*HL);
	set_carry(value & 0x1);
	value >>= 1;
	set_zero(!value);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8_logical(*HL, value);
	*PC += 1;
	cycles += 12;
}

//Bit opcodes
void bit_reg8(byte bit, reg8 reg)
{
	set_zero( ((*reg >> bit) & 0x1) == 0 );
	set_subtract(false);
	set_halfcarry(true);
	*PC += 1;
	cycles += 4;
}
void bit_hl(byte bit)
{
	byte value = memory_get8(*HL);
	set_zero( ((value >> bit) & 0x1) == 0 );
	set_subtract(false);
	set_halfcarry(true);
	*PC += 1;
	cycles += 12;
}
void set_reg8(byte bit, reg8 reg)
{
	*reg |= (1 << bit);
	*PC += 1;
	cycles += 4;
}
void set_hl(byte bit)
{
	byte value = memory_get8(*HL);
	value |= (1 << bit);
	memory_set8_logical(*HL, value);
	*PC += 1;
	cycles += 12;
}
void res_reg8(byte bit, reg8 reg)
{
	*reg &= ~(1 << bit);
	*PC += 1;
	cycles += 4;
}
void res_hl(byte bit)
{
	byte value = memory_get8(*HL);
	value &= ~(1 << bit);
	memory_set8_logical(*HL, value);
	*PC += 1;
	cycles += 12;
}

//Jumps
void jp()
{
	*PC = memory_get16(*PC + 1);
	cycles += 12;
}
void jp_hl()
{
	//*PC = memory_get16(*HL);
	*PC = *HL;
	cycles += 4;
}
void jp_cc(conditional c)
{
	word address = memory_get16(*PC + 1);
	bool increment = false;
	switch(c)
	{
		case NOTZERO:
			if(!get_zero()) *PC = address;
			else increment = true;
			break;
		case ZERO:
			if(get_zero()) *PC = address;
			else increment = true;
			break;
		case NOTCARRY:
			if(!get_carry()) *PC = address;
			else increment = true;
			break;
		case CARRY:
			if(get_carry()) *PC = address;
			else increment = true;
			break;
		default:
			//Should never happen
			increment = true;
			break;
	}
	if(increment)
	{
		*PC += 3;
		cycles += 12;
	}
	else cycles += 16;
}
void jr()
{
	byte relative_address = memory_get8(*PC + 1);
	*PC += 2;
	*PC = do_signed_add_word_byte(*PC, relative_address);
	cycles += 8;
}
void jr_cc(conditional c)
{
	byte relative_address = memory_get8(*PC + 1);
	*PC += 2;
	word new_address = do_signed_add_word_byte(*PC, relative_address);
	bool increment = false;
	switch(c)
	{
		case NOTZERO:
			if(!get_zero()) *PC = new_address;
			else increment = true;
			break;
		case ZERO:
			if(get_zero()) *PC = new_address;
			else increment = true;
			break;
		case NOTCARRY:
			if(!get_carry()) *PC = new_address;
			else increment = true;
			break;
		case CARRY:
			if(get_carry()) *PC = new_address;
			else increment = true;
			break;
		default:
			//Should never happen
			increment = true;
			break;
	}
	if(increment)
	{
		cycles += 8;
	}
	else cycles += 12;

}
void call()
{
	*SP -= 2;
	memory_set16_logical(*SP, *PC + 3);
	*PC = memory_get16(*PC + 1);
	cycles += 12;
}
void call_cc(conditional c)
{
	word address = memory_get16(*PC + 1);
	bool increment = false;
	switch(c)
	{
		case NOTZERO:
			if(!get_zero())
			{
				*SP -= 2;
				memory_set16_logical(*SP, *PC + 3);
				*PC = address;
			}
			else increment = true;
			break;
		case ZERO:
			if(get_zero())
			{
				*SP -= 2;
				memory_set16_logical(*SP, *PC + 3);
				*PC = address;
			}
			else increment = true;
			break;
		case NOTCARRY:
			if(!get_carry())
			{
				*SP -= 2;
				memory_set16_logical(*SP, *PC + 3);
				*PC = address;
			}
			else increment = true;
			break;
		case CARRY:
			if(get_carry())
			{
				*SP -= 2;
				memory_set16_logical(*SP, *PC + 3);
				*PC = address;
			}
			else increment = true;
			break;
		default:
			//Should never happen
			increment = true;
			break;
	}
	if(increment)
	{
		*PC += 3;
		cycles += 12;
	}
	else cycles += 24;
}

//Restart
void rst(byte address)
{
	*SP -= 2;
	memory_set16_logical(*SP, *PC + 1);
	*PC = address;
	cycles += 32;
}

//Returns
void ret()
{
	word address = memory_get16(*SP);
	*SP += 2;
	*PC = address;
	cycles += 8;
}
void ret_cc(conditional c)
{
	word address = memory_get16(*SP);
	bool increment = false;
	switch(c)
	{
		case NOTZERO:
			if(!get_zero())
			{
				*SP += 2;
				*PC = address;
			}
			else increment = true;
			break;
		case ZERO:
			if(get_zero())
			{
				*SP += 2;
				*PC = address;
			}
			else increment = true;
			break;
		case NOTCARRY:
			if(!get_carry())
			{
				*SP += 2;
				*PC = address;
			}
			else increment = true;
			break;
		case CARRY:
			if(get_carry())
			{
				*SP += 2;
				*PC = address;
			}
			else increment = true;
			break;
		default:
			//Should never happen
			increment = true;
			break;
	}
	if(increment)
	{
		*PC += 1;
		cycles += 8;
	}
	else cycles += 20;

}
void reti()
{
	interruptsEnabled = true;
	ret();
}

//Prefix CB
void prefix_cb()
{
	prefixCB = true;
	*PC += 1;
	cycles += 4;
}
