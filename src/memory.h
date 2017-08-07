/*
	memory.h

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
#include "globaldefs.h"

#define MEMORY_SIZE 0x10000

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
extern const word interrupt_enable_flag;

byte memory_get8(word);
word memory_get16(word);

void memory_set8(word, byte);
void memory_set16(word, word);

void memory_init(void);
void memory_free(void);
