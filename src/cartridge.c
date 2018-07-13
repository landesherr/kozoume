/*
	cartridge.c

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

#include "globaldefs.h"
#include "cartridge.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BANK_SIZE 0x4000
#define RAM_BANK_SIZE 0x2000
#define MAX_CART_SIZE 0x500000 //Reject carts bigger than 5 MiB

cartridge *mycart;

static inline unsigned get_cart_size(FILE*);
static inline byte calc_rom_banks(byte);
static inline unsigned calc_ram_size(byte);
static inline bool calculate_checksum(FILE*);

static const char* translate_cart_name(char *name)
{
	static char safename[17] = {0};
	for(byte i=0;i<16;i++)
	{
		//No control chars, no symbols
		if(name[i] == 0 || (name[i] >= 0x30 && name[i] < 0x7B)) safename[i] = name[i];
		else safename[i] = '_'; //If such a character is found, replace it with an underscore
	}
	return safename;
}

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
	if(cart_filesize < BANK_SIZE || cart_filesize % 0x2000 != 0 || cart_filesize > MAX_CART_SIZE)
	{
		dbgwrite("Invalid ROM!\n");
		fclose(cart);
		exit(1);
	}
	cartridge *loaded_cart = malloc(sizeof(cartridge));
	//load base ROM
	dbgwrite("Reading from ROM...\n");
	fread(memory_map, BANK_SIZE, 1, cart);
	for(unsigned i=0;i<16;i++)
	{
		(loaded_cart->gamename)[i] = memory_get8(0x134 + i);
	}
	(loaded_cart->gamename)[16] = '\0';
	loaded_cart->filename = path;
	loaded_cart->bank = 0;
	loaded_cart->ram_bank = 0;
	loaded_cart->type = (cart_type) memory_get8(0x147);
	loaded_cart->is_gbc = (memory_get8(0x143) == 0xC0);
	loaded_cart->rom_banks = calc_rom_banks(memory_get8(0x148));
	loaded_cart->ram_bytes = calc_ram_size(memory_get8(0x149));
	loaded_cart->data = NULL;
	loaded_cart->ram = NULL;
	loaded_cart->ram_enable = false;
	loaded_cart->mode = MODE_SWITCH_ROM;
	loaded_cart->latch_req = RTC_LATCH_NONE;
	loaded_cart->latching = RTC_UNLATCHED;
	//load additional bank if multiple exist
	if(loaded_cart->rom_banks > 1)
	{
		fseek(cart, BANK_SIZE, SEEK_SET);
		fread(&memory_map[BANK_SIZE], BANK_SIZE, 1, cart);
		loaded_cart->bank = 1;
		if(loaded_cart->rom_banks > 2)
		{
			//load whole cartridge into memory
			loaded_cart->data = malloc(cart_filesize);
			fseek(cart, 0, SEEK_SET);
			fread(loaded_cart->data, sizeof(char), cart_filesize, cart);
		}
	}
	dbgwrite("Current ROM is type %X\n", loaded_cart->type);
	dbgwrite("Current ROM %s a GBC ROM\n", loaded_cart->is_gbc ? "IS" : "IS NOT");
	dbgwrite("%d banks ROM, %d bytes RAM\n", loaded_cart->rom_banks, loaded_cart->ram_bytes);
	dbgwrite("Name - %s\n", loaded_cart->gamename);
	calculate_checksum(cart);
	fclose(cart);
	mycart = loaded_cart;
	if(loaded_cart->ram_bytes > RAM_BANK_SIZE)
	{
		loaded_cart->ram = calloc(loaded_cart->ram_bytes, sizeof(char));
	}
	if(HAS_BATT(loaded_cart)) load_ram_from_file(loaded_cart);

	return loaded_cart;
}

void bank_switch(cartridge *c, word bankno)
{
	if(!bankno) bankno = 1;
	unsigned offset = BANK_SIZE * bankno;
	memcpy(&memory_map[BANK_SIZE], &(c->data[offset]), BANK_SIZE);
	c->bank = bankno;
}
void store_ram_bank(cartridge *c)
{
	//Copy current RAM bank back into the full RAM buffer
	byte available_banks = c->ram_bytes / RAM_BANK_SIZE;
	if(available_banks < 2) return;
	unsigned offset = RAM_BANK_SIZE * c->ram_bank;
	memcpy(&(c->ram[offset]), &memory_map[cart_ram.lower], RAM_BANK_SIZE);
}
void ram_bank_switch(cartridge *c, byte bankno)
{
	byte available_banks = c->ram_bytes / RAM_BANK_SIZE;
	if(available_banks < 2) return;
	if(bankno > available_banks) return;
	store_ram_bank(c);
	unsigned offset = RAM_BANK_SIZE * bankno;
	memcpy(&memory_map[cart_ram.lower], &(c->ram[offset]), RAM_BANK_SIZE);
	c->ram_bank = bankno;
}
void sync_ram_to_disk(cartridge *c)
{
	if(!HAS_BATT(c)) return;
	byte available_banks = c->ram_bytes / RAM_BANK_SIZE;
	char savename[32] = {0};
	sprintf(savename, "%s.kzsav", translate_cart_name(c->gamename));
	FILE *ramfile = fopen(savename, "wb");
	if(!ramfile) return; //TODO warn user that save could not be saved
	if(available_banks < 2) fwrite(&memory_map[cart_ram.lower], sizeof(char), c->ram_bytes, ramfile);
	else
	{
		store_ram_bank(c);
		fwrite(c->ram, sizeof(char), c->ram_bytes, ramfile); //TODO check for ferror
	}
	fclose(ramfile);
}
void load_ram_from_file(cartridge *c)
{
	if(!HAS_BATT(c)) return;
	byte available_banks = c->ram_bytes / RAM_BANK_SIZE;
	char savename[32] = {0};
	sprintf(savename, "%s.kzsav", translate_cart_name(c->gamename));
	FILE *ramfile = fopen(savename, "rb");
	if(!ramfile) return;
	if(available_banks < 2) fread(&memory_map[cart_ram.lower], sizeof(char), c->ram_bytes, ramfile);
	else
	{
		fread(c->ram, sizeof(char), c->ram_bytes, ramfile); //TODO check for ferror
		memcpy(&memory_map[cart_ram.lower], c->ram, RAM_BANK_SIZE);
	}
	fclose(ramfile);
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
			return 0x800; //2k, 1 bank (partial)
		case 2:
			return 0x2000; //8k, 1 bank (full)
		case 3:
			return 0x8000; //32k, 4 banks
		case 4:
			return 0x20000; //128k, 16 banks
		default:
			dbgwrite("Bad RAM size identifier\n");
			exit(1);
	}
}
