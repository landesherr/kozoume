/*
	ppu.h
	
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

#pragma once

#include "memory.h"
#include "globaldefs.h"
#include <stdbool.h>

#define SCREEN_RES_X 160
#define SCREEN_RES_Y 144
#define BACKGROUND_MAP_SIZE 255

#define MODE_0_CYCLES 204
#define MODE_1_CYCLES 4560
#define MODE_2_CYCLES 80
#define MODE_3_CYCLES 172

#define COINCIDENCE_INTERRUPT(TF) set_stat(6, tf)
#define OAM_INTERRUPT(TF) set_stat(5, TF)
#define VBLANK_INTERRUPT(TF) set_stat(4, TF)
#define HBLANK_INTERRUPT(TF) set_stat(3, TF)

typedef enum ppu_mode
{
	HBLANK = 0,
	VBLANK = 1,
	SEARCH = 2,
	TRANSFER = 3
} ppu_mode;

extern ppu_mode gfxmode;

void ppu_tick(void);

static inline void set_stat(byte, bool);
static inline void set_mode_flag(ppu_mode);

static inline void set_stat(byte bit, bool set)
{
	byte value = memory_get8(STAT);
	if(set) value |= (1 << bit);
	else value &= ~(1 << bit);
	memory_set8(STAT, value);
}
static inline void set_mode_flag(ppu_mode m)
{
	set_stat(0, m & 1);
	set_stat(1, (m >> 1) & 1);
}