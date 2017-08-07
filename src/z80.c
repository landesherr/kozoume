/*
	z80.c

	Copyright 2017 Nolan Varani

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
bool interruptsEnabled = true;

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
	memory_set8(*HL, *source);
	*PC += 1;
	cycles += 8;
}
void ld_mem_n()
{
	memory_set8(*HL, memory_get8(*PC + 1));
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
	memory_set8(memory_get16(*address), *A);
	*PC += 1;
	cycles += 8;
}
void ld_nnmem_a()
{
	memory_set8(memory_get16(*PC + 1), *A);
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
	memory_set8(0xFF00 + *C, *A);
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
	memory_set8(*HL,*A);
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
	memory_set8(*HL,*A);
	*HL += 1;
	*PC += 1;
	cycles += 8;
}
void ldh_n_a()
{
	memory_set8(0xFF00 + memory_get8(*PC + 1), *A);
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
	signed char tempvalue;
	word result;
	tempvalue = (signed char) memory_get8(*PC + 1);
	result = do_signed_add_reg16_byte(*SP, tempvalue);
	calc_halfcarry_16(result, *SP, false);
	calc_carry_16(result, *SP, false);
	set_zero(false);
	set_subtract(false);
	*HL = result;
	*PC += 2;
	cycles += 12;
}
void ld_nn_sp()
{
	memory_set16(memory_get16(*PC + 1), *SP);
	*PC += 3;
	cycles += 20;
}
void push_nn(reg16 reg)
{
	*SP -= 2;
	memory_set16(*SP, *reg);
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
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void add_a_reg8(reg8 reg)
{
	byte result = *A + *reg;
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void add_a_hl()
{
	byte result = *A + memory_get8(*HL);
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void adc_a_n()
{
	byte result;
	result = *A + memory_get8(*PC + 1) + get_carry();
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void adc_a_reg8(reg8 reg)
{
	byte result = *A + *reg + get_carry();
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void adc_a_hl()
{
	byte result = *A + memory_get8(*HL) + get_carry();
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void sub_a_n()
{
	byte result = *A - memory_get8(*PC + 1);
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void sub_a_reg8(reg8 reg)
{
	byte result = *A - *reg;
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void sub_a_hl()
{
	byte result = *A - memory_get8(*HL);
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void sbc_a_n()
{
	byte result = *A - (memory_get8(*PC + 1) + get_carry());
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*A = result;
	*PC += 2;
	cycles += 8;
}
void sbc_a_reg8(reg8 reg)
{
	byte result = *A - (*reg + get_carry());
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 4;
}
void sbc_a_hl()
{
	byte result = *A - (memory_get8(*HL) + get_carry());
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*A = result;
	*PC += 1;
	cycles += 8;
}
void cp_n()
{
	byte result = *A - memory_get8(*PC + 1);
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*PC += 2;
	cycles += 8;
}
void cp_reg8(reg8 reg)
{
	byte result = *A - *reg;
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*PC += 1;
	cycles += 4;
}
void cp_hl()
{
	byte result = *A - *HL;
	calc_halfcarry_8(result, *A, true);
	calc_carry_8(result, *A, true);
	if(!result) set_zero(true);
	set_subtract(true);
	*PC += 1;
	cycles += 8;
}
void and_n()
{
	*A &= memory_get8(*PC + 1);
	if(!*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(true);
	set_carry(false);
	*PC += 2;
	cycles += 8;
}
void and_reg8(reg8 reg)
{
	*A &= *reg;
	if(*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(true);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void and_hl()
{
	*A &= memory_get8(*HL);
	if(!*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(true);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void or_n()
{
	*A |= memory_get8(*PC + 1);
	if(!*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 2;
	cycles += 8;
}
void or_reg8(reg8 reg)
{
	*A |= *reg;
	if(*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void or_hl()
{
	*A |= memory_get8(*HL);
	if(!*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void xor_n()
{
	*A ^= memory_get8(*PC + 1);
	if(!*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 2;
	cycles += 8;
}
void xor_reg8(reg8 reg)
{
	*A ^= *reg;
	if(*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 4;
}
void xor_hl()
{
	*A ^= memory_get8(*HL);
	if(!*A) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void inc_reg8(reg8 reg)
{
	byte result = *reg + 1;
	calc_halfcarry_8(*reg, result, false);
	set_subtract(false);
	if(!result) set_zero(true);
	*reg = result;
	*PC += 1;
	cycles += 4;
}
void inc_hl()
{
	byte result = memory_get8(*HL) + 1;
	calc_halfcarry_8(memory_get8(*HL), result, false);
	set_subtract(false);
	if(!result) set_zero(true);
	memory_set8(*HL, result);
	*PC += 1;
	cycles += 12;
}
void dec_reg8(reg8 reg)
{
	byte result = *reg - 1;
	calc_halfcarry_8(*reg, result, true);
	set_subtract(true);
	if(!result) set_zero(true);
	*reg = result;
	*PC += 1;
	cycles += 4;
}
void dec_hl()
{
	byte result = memory_get8(*HL) - 1;
	calc_halfcarry_8(memory_get8(*HL), result, true);
	set_subtract(true);
	if(!result) set_zero(true);
	memory_set8(*HL, result);
	*PC += 1;
	cycles += 12;
}

//16-bit ALU
void add_hl_reg16(reg16 reg)
{
	word result = *HL + *reg;
	calc_halfcarry_16(result, *HL, false);
	calc_carry_16(result, *HL, false);
	set_subtract(false);
	*HL = result;
	*PC += 1;
	cycles += 8;
}
void add_sp_n()
{
	word result;
	signed char tempval = memory_get8(*PC + 1);
	result = do_signed_add_reg16_byte(SP, tempval);
	calc_halfcarry_16(result, *SP, false);
	calc_carry_16(result, *SP, false);
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
	if(!*reg) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 8;
}
void swap_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value & 0xF;
	value = (value >> 4);
	value |= (temp << 4);
	memory_set8(*HL, value);
	if(!value) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	set_carry(false);
	*PC += 1;
	cycles += 16;
}
void daa()
{
	//TODO copy implementation from VM
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
//Technically, enabling or disabling interrupts shouldn't happen immediately
//It should be delayed by one instruction on actual GB hardware
//However, the GBC doesn't behave like this, and compatibility is fine
void di()
{
	interruptsEnabled = false;
	*PC += 1;
	cycles += 4;
}
void ei()
{
	interruptsEnabled = true;
	*PC += 1;
	cycles += 4;
}

//Rotates + Shifts
void rlc_reg8(reg8 reg)
{
	//8-bit rotation, bit 7 goes to both CF and bit 0
	byte temp = *reg << 1;
	temp |= (*reg >> 7);
	if(!temp) set_zero(true);
	set_carry( (*reg & 0x8) );
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
	if(!temp) set_zero(true);
	set_carry( (value & 0x8) );
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, temp);
	*PC += 1;
	cycles += 16;
}
void rl_reg8(reg8 reg)
{
	//9-bit rotation, CF to bit 0, bit 7 to CF
	byte temp = *reg << 1;
	temp |= get_carry();
	if(!temp) set_zero(true);
	set_carry(*reg >> 7);
	set_subtract(false);
	set_halfcarry(false);
	*reg = temp;
	*PC += 1;
	cyles += 4;
}
void rl_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value << 1;
	temp |= get_carry();
	if(!temp) set_zero(true);
	set_carry(value >> 7);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, temp)
	*PC += 1;
	cyles += 16;
}
void rrc_reg8(reg8 reg)
{
	//8-bit rotation, bit 0 goes to bit 7 and CF
	byte temp = *reg >> 1;
	temp |= (*reg << 7);
	if(!temp) set_zero(true);
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
	if(!temp) set_zero(true);
	set_carry(value & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, temp);
	*PC += 1;
	cycles += 16;
}
void rr_reg8(reg8 reg)
{
	//9-bit rotation, CF goes to bit 7, bit 0 to CF
	byte temp = *reg >> 1;
	temp |= get_carry() << 7;
	if(!temp) set_zero(true);
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
	if(!temp) set_zero(true);
	set_carry(value & 0x1);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, temp);
	*PC += 1;
	cycles += 16;
}
void sla_reg8(reg8 reg)
{
	set_carry(*reg & 0x80);
	*reg <<= 1;
	if(!*reg) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 8;
}
void sla_hl()
{
	byte value = memory_get8(*HL);
	set_carry(value & 0x80);
	value <<= 1;
	if(!value) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, value);
	*PC += 1;
	cycles += 16;
}
void sra_reg8(reg8 reg)
{
	//bit 7 remains unchanged
	byte temp = *reg >> 1;
	set_carry(*reg & 0x1);
	*reg = (*reg & 0x80) | temp;
	if(!*reg) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 8;
}
void sra_hl()
{
	byte value = memory_get8(*HL);
	byte temp = value >> 1;
	set_carry(value & 0x1);
	value = (value & 0x80) | temp;
	if(!value) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, value);
	*PC += 1;
	cycles += 16;
}
void srl_reg8(reg8 reg)
{
	//bit 7 reset to 0
	set_carry(*reg & 0x1);
	*reg >>= 1;
	if(!*reg) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	*PC += 1;
	cycles += 8;
}
void srl_hl()
{
	byte value = memory_get8(*HL);
	set_carry(value & 0x1);
	value >>= 1;
	if(!value) set_zero(true);
	set_subtract(false);
	set_halfcarry(false);
	memory_set8(*HL, value);
	*PC += 1;
	cycles += 16;
}

//Bit opcodes
void bit_reg8(byte bit, reg8 reg)
{
	set_zero( ((*reg >> bit) & 0x1) == 0 );
	set_subtract(false);
	set_halfcarry(true);
	*PC += 1;
	cycles += 8;
}
void bit_hl(byte bit)
{
	byte value = memory_get8(*HL);
	set_zero( ((value >> bit) & 0x1) == 0 );
	set_subtract(false);
	set_halfcarry(true);
	*PC += 1;
	cycles += 16;
}
void set_reg8(byte bit, reg8 reg)
{
	*reg |= (1 << bit);
	*PC += 1;
	cycles += 8;
}
void set_hl(byte bit)
{
	byte value = memory_get8(*HL);
	value |= (1 << bit);
	memory_set8(*HL, value);
	*PC += 1;
	cycles += 16;
}
void res_reg8(byte bit, reg8 reg)
{
	*reg &= ~(1 << bit);
	*PC += 1;
	cycles += 8;
}
void res_hl(byte bit)
{
	byte value = memory_get8(*HL);
	value &= ~(1 << bit);
	memory_set8(*HL, value);
	*PC += 1;
	cycles += 16;
}
