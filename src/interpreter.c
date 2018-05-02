/*
	interpreter.c

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


#include "interpreter.h"
#include "interrupts.h"
#include "z80.h"
#include "opcodes.h"
#include "memory.h"
#include "io.h"
#include "ppu.h"
#include <stdbool.h>

unsigned cycles_prev;
byte haltingBugTriggered = 0; //0 = Not checked, Not enabled 1 = Enabled, 2 = Checked, Not enabled

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
			isHalting = false;
			cycles += 4;
			cycles_prev += 4;
			if(interruptsEnabled)
			{
				do_interrupts();
			}
			else
			{
				haltingBugTriggered = 0;
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
	cycles_prev = cycles;
	if(haltingBugTriggered == 1)
	{
		standard_opcodes[BUGFETCH()]();
		haltingBugTriggered = 0;
	}
	else
	{
		if(!prefixCB) standard_opcodes[FETCH()]();
		else
		{
			prefix_opcodes[FETCH()]();
			prefixCB = false;
		}
	}
	*F &= 0xF0; //Only use top 4 bits of F
	io_tick();
	ppu_tick();
	if(isHalting && !interruptsEnabled && haltingBugTriggered != 2)
	{
		if(check_interrupts())
		{
			isHalting = false;
			haltingBugTriggered = 1;
		}
		else haltingBugTriggered = 2;
	}
	if(interruptsEnabled && !prefixCB && !isHalting) do_interrupts();
}
