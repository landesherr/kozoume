/*
	cartridge.c

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
#include "cartridge.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

#define BANK_SIZE 0x4000

static inline unsigned get_cart_size(FILE*);
static inline byte calc_rom_banks(byte);
static inline unsigned calc_ram_size(byte);
static inline bool calculate_checksum(FILE*);

cartridge* load_cart(char *path)
{
	FILE *cart;
	cart = fopen(path,"rb");
	if(!cart)
	{
		dbgwrite("Unable to load ROM!\n");
		exit(1);
	}
	unsigned cart_filesize = get_cart_size(cart);
	if(cart_filesize < BANK_SIZE || cart_filesize % 0x2000 != 0)
	{
		dbgwrite("Invalid ROM!\n");
		exit(1);
	}
	cartridge *loaded_cart = malloc(sizeof(cartridge));
	//load base ROM
	dbgwrite("Reading from ROM...\n");
	fread(memory_map, BANK_SIZE, 1, cart);
	char name[17];
	for(unsigned i=0;i<16;i++)
	{
		name[i] = memory_get8(0x134 + i);
	}
	name[16] = '\0';
	loaded_cart->gamename = name;
	loaded_cart->filename = path;
	loaded_cart->bank = 0;
	loaded_cart->type = (cart_type) memory_get8(0x147);
	loaded_cart->is_gbc = (memory_get8(0x143) == 0xC0);
	loaded_cart->rom_banks = calc_rom_banks(memory_get8(0x148));
	loaded_cart->ram_bytes = calc_ram_size(memory_get8(0x149));
	//load additional bank if multiple exist
	if(loaded_cart->rom_banks > 0)
	{
		fseek(cart, BANK_SIZE, SEEK_SET);
		fread(&memory_map[BANK_SIZE], BANK_SIZE, 1, cart);
		//TODO refactor, handle bank switching
	}
	dbgwrite("Current ROM is type %X\n", loaded_cart->type);
	dbgwrite("Current ROM %s a GBC ROM\n", loaded_cart->is_gbc ? "IS" : "IS NOT");
	dbgwrite("%d banks ROM, %d bytes RAM\n", loaded_cart->rom_banks, loaded_cart->ram_bytes);
	dbgwrite("Name - %s\n", name);
	calculate_checksum(cart);
	fclose(cart);
	return loaded_cart;
}

static inline unsigned get_cart_size(FILE *cart)
{
	unsigned filesize;
	fseek(cart, 0, SEEK_END);
	filesize = ftell(cart);
	rewind(cart);
	return filesize;
}

static inline bool calculate_checksum(FILE *cart)
{
	rewind(cart);
	word checksum = ((word) memory_get8(0x14E)) << 8;
	checksum |= ((word) memory_get8(0x14F));
	word actual = 1;
	byte header = 0;
	unsigned i = 0;
	while(!feof(cart) && !ferror(cart))
	{
		if(i >= 0x134 && i <= 0x14D)
		{
			byte temp = fgetc(cart);
			if(i == 0x14D)
			{
				dbgwrite("Testing Header Checksum - Desired %X, Actual %X\n", temp, header);
			}
			else header -= (temp + 1);
			actual += temp;
		}
		else actual += fgetc(cart);
		++i;
	}
	//Apparently, the checksum bytes themselves aren't included
	byte *checksum_bytes = (byte*) &checksum;
	actual -= checksum_bytes[0];
	actual -= checksum_bytes[1];
	dbgwrite("Testing Checksum - Desired: %X, Actual %X\n", checksum, actual);
	rewind(cart);
	return (actual == checksum);
}

static inline byte calc_rom_banks(byte identifier)
{
	if(identifier < 7) return (2 << identifier);
	switch(identifier)
	{
		case 0x52:
			return 72;
		case 0x53:
			return 80;
		case 0x54:
			return 96;
		default:
			dbgwrite("Bad ROM size identifier\n");
			exit(1);
	}
}
static inline unsigned calc_ram_size(byte identifier)
{
	switch(identifier)
	{
		case 0:
			return 0;
		case 1:
			return 0x800;
		case 2:
			return 0x2000;
		case 3:
			return 0x8000;
		case 4:
			return 0x20000;
		default:
			dbgwrite("Bad RAM size identifier\n");
			exit(1);
	}
}
