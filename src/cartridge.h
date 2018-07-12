/*
	cartridge.h

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
#include <stdbool.h>

#define MODE_SWITCH_ROM false
#define MODE_SWITCH_RAM true
#define RTC_LATCH_NONE false
#define RTC_LATCH_STEP true
#define RTC_UNLATCHED false
#define RTC_LATCHED true

typedef enum cart_type
{
	ROM_ONLY = 0,
	ROM_MBC1 = 1,
	ROM_MBC1_RAM = 2,
	ROM_MBC1_RAM_BATT = 3,
	ROM_MBC2 = 5,
	ROM_MBC2_BATT = 6,
	ROM_RAM = 8,
	ROM_RAM_BATT = 9,
	ROM_MMM01 = 0xB,
	ROM_MMM01_SRAM = 0xC,
	ROM_MMM01_SRAM_BATT = 0xD,
	ROM_MBC3_TIMER_BATT = 0xF,
	ROM_MBC3_TIMER_RAM_BATT = 0x10,
	ROM_MBC3 = 0x11,
	ROM_MBC3_RAM = 0x12,
	ROM_MBC3_RAM_BATT = 0x13,
	ROM_MBC5 = 0x19,
	ROM_MBC5_RAM = 0x1A,
	ROM_MBC5_RAM_BATT = 0x1B,
	ROM_MBC5_RUMBLE = 0x1C,
	ROM_MBC5_RUMBLE_SRAM = 0x1D,
	ROM_MBC5_RUMBLE_SRAM_BATT = 0x1E,
	POCKET_CAMERA = 0x1F,
	TAMA5 = 0xFD,
	HUC3 = 0xFE,
	HUC1 = 0xFF
} cart_type;

typedef struct cartridge
{
	char *filename;
	char *gamename;
	byte bank;
	cart_type type;
	bool is_gbc;
	byte rom_banks;
	unsigned ram_bytes;
	byte *data;
	byte *ram;
	//MBC variables
	bool ram_enable;
	bool mode;
	bool latch_req;
	bool latching;
} cartridge;

extern cartridge *mycart;

cartridge* load_cart(char*);
void free_cart(cartridge*);
void bank_switch(cartridge*, byte);
