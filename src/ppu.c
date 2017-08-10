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
#include "interpreter.h"
#include "globaldefs.h"
#include <stdbool.h>

static ppu_mode mode = SEARCH;
static unsigned hblank_count, mode_cycles;

void ppu_tick()
{
	//144 search->transfer->hblank cycles followed by vblank
	//hblank = 456 cycles
	//(80 mode 2, 172 mode 3, 204 mode 0)
	//vblank = 4560 cycles
	//refresh = 70224 cycles
	mode_cycles += (cycles - cycles_prev);
	switch(ppu_mode)
	{
		case HBLANK:
			if(mode_cycles >= MODE_0_CYCLES)
			{
				mode_cycles -= MODE_0_CYCLES;
				hblank_count++;
				if(hblank_count == 10)
				{
					ppu_mode = VBLANK;
					hblank_count = 0;
					VBLANK_INTERRUPT(true);
				}
				else
				{
					ppu_mode = SEARCH;
					OAM_INTERRUPT(true);
				}
				set_mode_flag(ppu_mode);
			}
			//TODO actual HBLANK logic
			break;
		case VBLANK:
			HBLANK_INTERRUPT(false);
			if(mode_cycles >= MODE_1_CYCLES)
			{
				mode_cycles -= MODE_1_CYCLES;
				ppu_mode = SEARCH;
				OAM_INTERRUPT(true);
				set_mode_flag(ppu_mode);
			}
			//TODO actual VBLANK logic
			break;
		case SEARCH:
			VBLANK_INTERRUPT(false);
			HBLANK_INTERRUPT(false);
			if(mode_cycles >= MODE_2_CYCLES)
			{
				mode_cycles -= MODE_1_CYCLES;
				ppu_mode = TRANSFER;
				set_mode_flag(ppu_mode);
			}
			//TODO actual SEARCH logic
			break;
		case TRANSFER:
			OAM_INTERRUPT(false);
			if(mode_cycles >= MODE_3_CYCLES)
			{
				mode_cycles -= MODE_3_CYCLES;
				ppu_mode = HBLANK;
				HBLANK_INTERRUPT(true);
				set_mode_flag(ppu_mode);
			}
			//TODO actual TRANSFER logic
			break;
		default:
			dbgwrite("Invalid PPU mode! How did this even happen?\n");
			exit(1);
	}
}
