#include "globaldefs.h"
#include "cartridge.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

#define BANK_SIZE 0x4000

static inline unsigned get_cart_size(FILE*);
static inline byte rom_banks(byte);
static inline unsigned ram_size(byte);
static inline bool calculate_checksum(FILE*);

cartridge* load_cart(char *path)
{
	FILE *cart;
	cart = fopen(path,"rb");
	if(!cart)
	{
		printf("Unable to load ROM!\n");
		exit(1);
	}
	cartridge *loaded_cart = malloc(sizeof(cartridge));
	if(get_cart_size(cart) < BANK_SIZE)
	{
		printf("Invalid ROM!\n");
		exit(1);
	}
	//load base ROM
	printf("Reading from ROM...\n");
	fread(&memory_map[0], BANK_SIZE, 1, cart);
	printf("ROM size - %X\n", memory_get8(0x148));
	printf("RAM size - %X\n", memory_get8(0x149));
	int i;
	char name[17];
	for(i=0;i<16;i++)
	{
		name[i] = memory_get8(0x134 + i);
	}
	name[16] = '\0';
	loaded_cart->gamename = name;
	loaded_cart->filename = path;
	loaded_cart->bank = 0;

	printf("Name - %s\n", name);
	calculate_checksum(cart);
	fclose(cart);
	return NULL;
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
	while(!feof(cart) && !ferror(cart)) actual += fgetc(cart);
	//Apparently, the checksum bytes themselves aren't included
	byte *checksum_bytes = (byte*) &checksum;
	actual -= checksum_bytes[0];
	actual -= checksum_bytes[1];
	printf("Testing Checksum - Desired: %X, Actual %X\n", checksum, actual);
	rewind(cart);
	return (actual == checksum);
}

static inline byte rom_banks(byte identifier)
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
			printf("Bad ROM size identifier\n");
			exit(1);
	}
}
static inline unsigned ram_size(byte identifier)
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
			printf("Bad RAM size identifier\n");
			exit(1);
	}
}
