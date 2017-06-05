#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "z80.h"
#include "cartridge.h"
#include "globaldefs.h"

void basic_sanity_check(void);
void powerup(void);

int main(int argc, char *argv[])
{
	memory_init();
	powerup();
	if(argc < 2)
	{
		dbgwrite("No ROM specified\n");
		exit(0);
	}
	load_cart(argv[1]);
	basic_sanity_check();
	memory_free();
	return 0;
}

void basic_sanity_check()
{
	bool doOnce = true;
	dbgwrite("Testing memory values per power up sequence...\n");
	dbgwrite("FF10 is %X - %s\n", memory_get8(0xFF10), memory_get8(0xFF10) == 0x80 ? "OK" : "FAIL");
	dbgwrite("FF1E is %X - %s\n", memory_get8(0xFF1E), memory_get8(0xFF1E) == 0xBF ? "OK" : "FAIL");
	dbgwrite("FF26 is %X - %s\n", memory_get8(0xFF26), memory_get8(0xFF26) == 0xF1 ? "OK" : "FAIL");
	dbgwrite("Testing sanity with some basic arithmetic and flags...\n");
	unsigned i;
	memory_set8(1,0xFF);
	ld_nn_n(A);
	dbgwrite("Value of A after LD: %X\n",*A);
	*A = 0;
	for(i=0;i<100;i++)
	{
		memory_set8(*PC + 1,0x09);
		add_a_n();
		//dbgwrite("Value of A: %X\n", *A);
		//dbgwrite("Value of AF: %X\n", *AF);
		if(*F & 0b00100000)
		{
			if(doOnce)
			{
				dbgwrite("Half Carry OK\n");
				doOnce = false;
			}
			*F &= 0b11011111;
		}
		if(*F & 0b00010000)
		{
			dbgwrite("Carry OK\n");
			return;
		}
	}
	dbgwrite("Too many iterations. Something's wrong.\n");
	exit(1);
}

//set memory/register values to defaults
void powerup()
{
	*F = 0xB0;
	*BC = 0x13;
	*DE = 0xD8;
	*HL = 0x14D;
	*SP = 0xFFFE;
	//The following addresses are 0x00 and will be automatically set by calloc()
	//FF05, FF06, FF07, FF17, FF21, FF22, FF42, FF43, FF45, FF4A, FF4B, FFFF
	memory_set8(0xFF10, 0x80);
	memory_set8(0xFF11, 0xBF);
	memory_set8(0xFF12, 0xF3);
	memory_set8(0xFF14, 0xBF);
	memory_set8(0xFF16, 0x3F);
	memory_set8(0xFF19, 0xBF);
	memory_set8(0xFF1A, 0x7F);
	memory_set8(0xFF1B, 0xFF);
	memory_set8(0xFF1C, 0x9F);
	memory_set8(0xFF1E, 0xBF);
	memory_set8(0xFF20, 0xFF);
	memory_set8(0xFF23, 0xBF);
	memory_set8(0xFF24, 0x77);
	memory_set8(0xFF25, 0xF3);
	memory_set8(0xFF26, 0xF1); //F0 on SGB
	memory_set8(0xFF40, 0x91);
	memory_set8(0xFF47, 0xFC);
	memory_set8(0xFF48, 0xFF);
	memory_set8(0xFF49, 0xFF);
}
