/*
	interrupts.c

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
#include "interrupts.h"
#include "memory.h"
#include "z80.h"
#include <stdbool.h>

#define INTERRUPT_ENABLE_ALL() memory_set8(INTERRUPT_ENABLE, 0x0F)
#define INTERRUPT_DISABLE_ALL() memory_set8(INTERRUPT_ENABLE, 0x00)
#define INTERRUPT_SET(INTERRUPT) ((memory_get8(INTERRUPT_FLAG) >> INTERRUPT) & 1)
#define INTERRUPT_ENABLED(INTERRUPT) ((memory_get8(INTERRUPT_ENABLE) >> INTERRUPT) & 1)

static const interrupt all_interrupts[NUM_INTERRUPTS] = { INT_VBLANK, INT_STAT, INT_TIMER, INT_SERIAL, INT_JOYPAD };

bool check_interrupts()
{
	interrupt in;
	for(unsigned i=0;i<NUM_INTERRUPTS;i++)
	{
		in = all_interrupts[i];
		if(INTERRUPT_ENABLED(in) && INTERRUPT_SET(in)) return true;
	}
	return false;
}

void do_interrupts()
{
	if(interruptsEnabled)
	{
		interrupt in;
		for(unsigned i=0;i<NUM_INTERRUPTS;i++)
		{
			in = all_interrupts[i];
			if(INTERRUPT_ENABLED(in) && INTERRUPT_SET(in))
			{
				do_interrupt(in);
				return;
			}
		}
	}
}

void set_interrupt(interrupt i, bool set)
{
	byte flags = memory_get8(INTERRUPT_FLAG);
	if(set) flags |= (1 << i);
	else flags &= ~(1 << i);
	memory_set8(INTERRUPT_FLAG, flags);
}
void enable_interrupt(interrupt i, bool enable)
{
	byte flags = memory_get8(INTERRUPT_ENABLE);
	if(enable) flags |= (1 << i);
	else flags &= ~(1 << i);
	memory_set8(INTERRUPT_ENABLE, flags);
}
