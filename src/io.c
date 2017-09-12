/*
	io.c

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

#include "io.h"
#include "interpreter.h"
#include "z80.h"
#include "interrupts.h"
#include <stdio.h>
#include "globaldefs.h"

#define CLOCKS_TO_DMA 0x280
#define CLOCKS_PER_BYTE 4

void io_tick()
{
	div_tick();
	tima_tick();
	dma_transfer();
}

void div_tick()
{
	byte value;
	//increment DIV every 64 clock cycles
	if(cycles % 64 < cycles_prev % 64)
	{
		value = memory_get8(DIV) + 1;
		memory_set8(DIV, value);
	}
}
void tima_tick()
{
	byte value;
	byte timer_control = memory_get8(TAC);
	unsigned input_clock, cycles_per_tick;
	if(timer_control & 0x4) //Start Timer bit (bit 2)
	{
		switch(timer_control & 0x3)
		{
			case 0:
				input_clock = 4096;
				break;
			case 1:
				input_clock = 262144;
				break;
			case 2:
				input_clock = 65536;
				break;
			case 3:
				input_clock = 16384;
				break;
		}
		cycles_per_tick = Z80_CYCLES / input_clock;
		if(cycles % cycles_per_tick < cycles_prev % cycles_per_tick)
		{
			value = memory_get8(TIMA) + 1;
			if(!value)
			{
				memory_set8(TIMA, memory_get8(TMA));
				set_interrupt(INT_TIMER, true);
			}
			else memory_set8(TIMA, value);
		}
	}
}

void dma_transfer()
{
	static int dma_cycles = 0, completed = 0;
	static word dma_address = 0;
	unsigned bytes_to_copy;
	if(dma_cycles)
	{
		dbgwrite("### Processing DMA ###\n");
		dma_cycles -= (cycles - cycles_prev);
		if(dma_cycles <= 0)
		{
			dma_cycles = 0;
			completed = 0;
			return;
		}
		else
		{
			bytes_to_copy = (CLOCKS_TO_DMA - dma_cycles) / CLOCKS_PER_BYTE;
			for(int i=completed;i<bytes_to_copy;i++)
			{
				//OAM = FE00-FE9F
				memory_set8(dma_address + i, memory_get8(0xFE00 + i));
			}
			completed = bytes_to_copy;
		}
		return;
	}
	dma_address = ((word) memory_get8(DMA)) << 8;
	//If DMA register is 0, don't do anything
	if(dma_address)
	{
		dbgwrite("### DMA requested at %X ###\n", dma_address);
		//40 * 28 bit transfer from 0000-f19f to OAM
		dma_cycles = CLOCKS_TO_DMA;
		memory_set8(DMA, 0); //Does this get reset as soon as DMA is acknowledged?
	}
}
