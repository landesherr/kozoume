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
	//int calc = *SP;
	//calc += (signed char) memory_get8(*PC + 1);
	word result;
	*HL = *SP;
	tempvalue = (signed char) memory_get8(*PC + 1);
	if(tempvalue > 0)
	{
		result = *HL + tempvalue;
		calc_carry_16(result, *HL, false);
	}
	else if(tempvalue < 0)
	{
		result = *HL - (0 - tempvalue);
		calc_carry_16(result, *HL, true);
	}
	set_zero(false);
	set_subtract(false);
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
