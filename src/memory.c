#include "globaldefs.h"
#include "memory.h"
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
const word interrupt_enable_flag = 0xFFFF;

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