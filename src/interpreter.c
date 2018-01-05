/*
	interpreter.c

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


#include "interpreter.h"
#include "interrupts.h"
#include "z80.h"
#include "opcodes.h"
#include "memory.h"
#include "io.h"
#include "ppu.h"
#include <stdbool.h>

unsigned cycles_prev;
bool haltingBugTriggered = false;

void interpreter_step()
{
	if(isHalting)
	{
		cycles++;
		cycles_prev++;
		io_tick();
		ppu_tick();
		if(check_interrupts())
		{
			if(interruptsEnabled)
			{
				isHalting = false;
				do_interrupts();
			}
			else
			{
				haltingBugTriggered = true;
				isHalting = false;
			}
		}
		return;
	}

	if(pendingEI)
	{
		interruptsEnabled = true;
		pendingEI = false;
	}
	if(pendingDI)
	{
		interruptsEnabled = false;
		pendingDI = false;
	}
	if(haltingBugTriggered)
	{
		haltingBugTriggered = false;
		cycles = cycles_prev;
	}
	cycles_prev = cycles;
	if(!prefixCB) standard_opcodes[FETCH()]();
	else
	{
		prefix_opcodes[FETCH()]();
		prefixCB = false;
	}
	*F &= 0xF0; //Only use top 4 bits of F
	io_tick();
	ppu_tick();

	if(interruptsEnabled && !prefixCB) do_interrupts();
}
