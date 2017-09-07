/*
	ppu.c

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

#include "ppu.h"
#include "z80.h"
#include "memory.h"
#include "io.h"
#include "interpreter.h"
#include "interrupts.h"
#include "globaldefs.h"
#include <stdbool.h>
#include <stdio.h> //remove when done debugging

ppu_mode gfxmode = SEARCH;
static unsigned hblank_count, mode_cycles;

void ppu_tick()
{
	//144 search->transfer->hblank cycles followed by vblank
	//hblank = 456 cycles
	//(80 mode 2, 172 mode 3, 204 mode 0)
	//vblank = 4560 cycles
	//refresh = 70224 cycles
	mode_cycles += (cycles - cycles_prev);
	switch(gfxmode)
	{
		case HBLANK:
			if(mode_cycles >= MODE_0_CYCLES)
			{
				mode_cycles -= MODE_0_CYCLES;
				hblank_count++;
				UPDATE_LY();
				if(hblank_count == SCREEN_RES_Y)
				{
					gfxmode = VBLANK;
					set_interrupt(VBLANK, true);
				}
				else
				{
					gfxmode = SEARCH;
				}
				set_mode_flag(gfxmode);
			}
			//TODO actual HBLANK logic
			break;
		case VBLANK:
			if(mode_cycles >= MODE_1_CYCLES)
			{
				mode_cycles -= MODE_1_CYCLES;
				gfxmode = SEARCH;
				set_mode_flag(gfxmode);
				hblank_count = 0;
				UPDATE_LY();
			}
			else if(mode_cycles % MODE_0_CYCLES == 0)
			{
				hblank_count++;
				UPDATE_LY();
			}
			//TODO actual VBLANK logic
			break;
		case SEARCH:
			if(mode_cycles >= MODE_2_CYCLES)
			{
				mode_cycles -= MODE_2_CYCLES;
				gfxmode = TRANSFER;
				set_mode_flag(gfxmode);
				mode_cycles = 0;
			}
			//TODO actual SEARCH logic
			break;
		case TRANSFER:
			if(mode_cycles >= MODE_3_CYCLES)
			{
				mode_cycles -= MODE_3_CYCLES;
				gfxmode = HBLANK;
				set_mode_flag(gfxmode);
			}
			//TODO actual TRANSFER logic
			break;
		default:
			dbgwrite("Invalid PPU mode! How did this even happen?\n");
	}
	set_stat(STAT_COINCIDENCE_FLAG, LY_COMPARE);
	//TODO is there any additional interrupt-specific logic needed below?
	if(gfxmode == HBLANK && HBLANK_INTERRUPT) set_interrupt(INT_STAT, true);
	else if(gfxmode == VBLANK && VBLANK_INTERRUPT) set_interrupt(INT_STAT, true);
	else if(gfxmode == SEARCH && OAM_INTERRUPT) set_interrupt(INT_STAT, true);
	else if(LY_COMPARE && COINCIDENCE_INTERRUPT) set_interrupt(INT_STAT, true);
}
