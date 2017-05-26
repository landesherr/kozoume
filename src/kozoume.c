#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "z80.h"
#include "globaldefs.h"

void basic_sanity_check(void);

int main()
{
	memory_init();
	basic_sanity_check();
	memory_free();
	return 0;
}

void basic_sanity_check()
{
	printf("Testing sanity with some basic arithmetic and flags...\n");
	unsigned i;
	memory_set8(1,0xFF);
	ld_nn_n(A);
	printf("Value of A after LD: %X\n\n",*A);
	*A = 0;
	for(i=0;i<100;i++)
	{
		memory_set8(*PC + 1,0x09);
		add_a_n();
		printf("Value of A: %X\n", *A);
		printf("Value of AF: %X\n", *AF);
		if(*F & 0b00100000)
		{
			printf("Half carry!\n");
			*F &= 0b11011111;
		}
		if(*F & 0b00010000)
		{
			printf("Carry!\n");
			return;
		}
	}
	printf("Too many iterations. Something's wrong.\n");
	exit(1);
}
