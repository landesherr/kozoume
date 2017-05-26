#include "globaldefs.h"
#include "z80.h"
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

void ld_nn_n(reg8 reg)
{
	//*reg = memory_get(*PC + 1);
	*PC += 2;
	cycles += 8;
}

void add_a_n()
{
	byte result, n = 9;
	//n = memory_get(*PC + 1);
	result = *A + n;
	calc_halfcarry_8(result, *A, false);
	calc_carry_8(result, *A, false);
	if(!result) set_zero(true);
	set_subtract(false);
	*A = result;
	*PC += 2;
	cycles += 8;
}
