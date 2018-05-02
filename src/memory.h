/*
	memory.h

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

#pragma once
#include "globaldefs.h"

#define MEMORY_SIZE 0x10000
#define MEMORY_IN_RANGE(ADDRESS, RANGE) (ADDRESS >= RANGE.lower && ADDRESS <= RANGE.upper)

typedef struct mem_range
{
	word lower, upper;
} mem_range;

extern byte *memory_map;
extern const mem_range \
	restart_interrupt_vectors, \
	cart_header, \
	rom_bank_0, \
	rom_bank_switch, \
	char_ram, \
	bg_map_1, \
	bg_map_2, \
	cart_ram, \
	internal_ram_bank_0, \
	internal_ram_bank_switch, \
	echo_ram, \
	oam, \
	unusable_mem, \
	hardware_io, \
	zero_page;

typedef enum ioreg
{
	P1 = 0xFF00,
	SB = 0xFF01,
	SC = 0xFF02,
	DIV = 0xFF04,
	TIMA = 0xFF05,
	TMA = 0xFF06,
	TAC = 0xFF07,
	IF = 0xFF0F,
	NR_10 = 0xFF10,
	NR_11 = 0xFF11,
	NR_12 = 0xFF12,
	NR_13 = 0xFF13,
	NR_14 = 0xFF14,
	NR_21 = 0xFF16,
	NR_22 = 0xFF17,
	NR_23 = 0xFF18,
	NR_24 = 0xFF19,
	NR_30 = 0xFF1A,
	NR_31 = 0xFF1B,
	NR_32 = 0xFF1C,
	NR_33 = 0xFF1D,
	NR_34 = 0xFF1E,
	NR_41 = 0xFF20,
	NR_42 = 0xFF21,
	NR_43 = 0xFF22,
	NR_44 = 0xFF23,
	NR_50 = 0xFF24,
	NR_51 = 0xFF25,
	NR_52 = 0xFF26,
	LCDC = 0xFF40,
	STAT = 0xFF41,
	SCY = 0xFF42,
	SCX = 0xFF43,
	LY = 0xFF44,
	LYC = 0xFF45,
	DMA = 0xFF46,
	BGP = 0xFF47,
	OBP0 = 0xFF48,
	OBP1 = 0xFF49,
	WY = 0xFF4A,
	WX = 0xFF4B,
	IE = 0xFFFF
} ioreg;

byte memory_get8(word);
signed char memory_get8s(word);
word memory_get16(word);
unsigned memory_get32(word);

//set memory values exactly as specified (faster, for use in emulator logic)
void memory_set8(word, byte);
void memory_set16(word, word);
//set memory values per GB memory map logic (slower, for use in emulated ops)
void memory_set8_logical(word, byte);
void memory_set16_logical(word, word);

void memory_init(void);
void memory_free(void);
