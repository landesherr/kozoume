/*
	memory.c

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

#include "globaldefs.h"
#include "memory.h"
#include "ppu.h"
#include <stdlib.h>

byte *memory_map;
const mem_range \
	restart_interrupt_vectors = {0x0000,0x00FF}, \
	cart_header = {0x0100,0x014F}, \
	rom_bank_0 = {0x0150,0x3FFF}, \
	rom_bank_switch = {0x4000,0x7FFF}, \
	char_ram = {0x8000,0x97FF}, \
	bg_map_1 = {0x9800,0x9BFF}, \
	bg_map_2 = {0x9C00,0x9FFF}, \
	cart_ram = {0xA000,0xBFFF}, \
	internal_ram_bank_0 = {0xC000,0xCFFF}, \
	internal_ram_bank_switch = {0xD000,0xDFFF}, \
	echo_ram = {0xE000,0xFDFF}, \
	oam = {0xFE00,0xFE9F}, \
	unusable_mem = {0xFEA0,0xFEFF}, \
	hardware_io = {0xFF00,0xFF7F}, \
	zero_page = {0xFF80,0xFFFE};

void memory_init()
{
	memory_map = calloc(MEMORY_SIZE, sizeof(byte));
}
void memory_free()
{
	free(memory_map);
}

byte memory_get8(word address)
{
	return memory_map[address];
}
signed char memory_get8s(word address)
{
	return *(char*)&memory_map[address];
}
word memory_get16(word address)
{
	return *((word*)&memory_map[address]);
}

void memory_set8(word address, byte value)
{
	memory_map[address] = value;
}
void memory_set16(word address, word value)
{
	*((word*)&memory_map[address]) = value;
}

void memory_set8_logical(word address, byte value)
{
	if(MEMORY_IN_RANGE(address, echo_ram))
	{
		//propagate value in echo and regular ram areas
		//this is inefficient, but it's nice to have a flat memory model
		memory_map[address - 0x2000] = value;
	}
	else if(MEMORY_IN_RANGE(address, internal_ram_bank_0))
	{
		memory_map[address + 0x2000] = value;
	}
	else if(MEMORY_IN_RANGE(address, unusable_mem)) return;
	else if(MEMORY_IN_RANGE(address, oam))
	{
		if(gfxmode == SEARCH || gfxmode == TRANSFER) return;
	}
	else if(MEMORY_IN_RANGE(address, char_ram) \
		|| MEMORY_IN_RANGE(address, bg_map_1) \
		|| MEMORY_IN_RANGE(address, bg_map_2))
	{
		if(gfxmode == TRANSFER) return;
	}
	else if(MEMORY_IN_RANGE(address, rom_bank_0) \
		|| MEMORY_IN_RANGE(address, rom_bank_switch) \
		|| MEMORY_IN_RANGE(address, cart_header))
	{
		//can't write to ROM
		return;
	}
	//TODO IO register behavior, OAM/VRAM, cart RAM if available
	memory_map[address] = value;
}
void memory_set16_logical(word address, word value)
{
	if(MEMORY_IN_RANGE(address, echo_ram))
	{
		//propagate value in echo and regular ram areas
		//this is inefficient, but it's nice to have a flat memory model
		*((word*)&memory_map[address - 0x2000]) = value;
	}
	else if(MEMORY_IN_RANGE(address, internal_ram_bank_0))
	{
		*((word*)&memory_map[address + 0x2000]) = value;
	}
	else if(MEMORY_IN_RANGE(address, unusable_mem)) return;
	else if(MEMORY_IN_RANGE(address, oam))
	{
		if(gfxmode == SEARCH || gfxmode == TRANSFER) return;
	}
	else if(MEMORY_IN_RANGE(address, char_ram) \
		|| MEMORY_IN_RANGE(address, bg_map_1) \
		|| MEMORY_IN_RANGE(address, bg_map_2))
	{
		if(gfxmode == TRANSFER) return;
	}
	else if(MEMORY_IN_RANGE(address, rom_bank_0) \
		|| MEMORY_IN_RANGE(address, rom_bank_switch) \
		|| MEMORY_IN_RANGE(address, cart_header))
	{
		//can't write to ROM
		return;
	}
	//TODO IO register behavior, OAM/VRAM, cart RAM if available
	*((word*)&memory_map[address]) = value;
}
