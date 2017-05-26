#include <stdio.h>
#include <stdlib.h>
#include "z80.h"
#include "globaldefs.h"

void basic_sanity_check(void);

int main()
{
	basic_sanity_check();
	return 0;
}

void basic_sanity_check()
{
	printf("Testing sanity with some basic arithmetic and flags...\n");
	unsigned i;
	for(i=0;i<100;i++)
	{
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
