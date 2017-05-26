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
