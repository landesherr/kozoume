/*
	ppu.h

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

#include "memory.h"
#include "globaldefs.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SPRITE_SIZE 8
#define SPRITE_SIZE_LARGE 16

#define SCREEN_RES_X 160
#define SCREEN_RES_Y 144
#define BACKGROUND_MAP_SIZE 255

#define MODE_0_CYCLES 204
#define MODE_1_CYCLES 4560
#define MODE_2_CYCLES 80
#define MODE_3_CYCLES 172
#define HBLANK_CYCLES (MODE_0_CYCLES + MODE_2_CYCLES + MODE_3_CYCLES)

#define TILE_MAP_1_BEGIN 0x9800
#define TILE_MAP_2_BEGIN 0x9C00
#define TILE_MAP_SIZE 0x400

#define TILE_DATA_1_BEGIN 0x8000
#define TILE_DATA_2_BEGIN 0x9000
#define TILE_DATA_SIZE 0x1000

#define UPDATE_LY() memory_set8(LY, hblank_count)
#define LY_COMPARE ( memory_get8(LY) == memory_get8(LYC) )

#define LCD_ON get_lcdc(7)

#define COINCIDENCE_INTERRUPT get_stat(STAT_COINCIDENCE)
#define OAM_INTERRUPT get_stat(STAT_OAM)
#define VBLANK_INTERRUPT get_stat(STAT_VBLANK)
#define HBLANK_INTERRUPT get_stat(STAT_HBLANK)

#define PIXEL(Y, X) screen_bitmap[(Y * SCREEN_RES_X) + X]

#define OAM_OPTIONS(OAM_ENTRY) (OAM_ENTRY >> 24)
#define OAM_TILE_NO(OAM_ENTRY) ((OAM_ENTRY >> 16) & 0xFF)
#define OAM_X_COORD(OAM_ENTRY) ((OAM_ENTRY >> 8) & 0xFF)
#define OAM_Y_COORD(OAM_ENTRY) (OAM_ENTRY & 0xFF)

typedef enum ppu_mode
{
	HBLANK = 0,
	VBLANK = 1,
	SEARCH = 2,
	TRANSFER = 3
} ppu_mode;

typedef enum stat_bit
{
	STAT_MODE_LOW = 0,
	STAT_MODE_HIGH = 1,
	STAT_COINCIDENCE_FLAG = 2,
	STAT_HBLANK = 3,
	STAT_VBLANK = 4,
	STAT_OAM = 5,
	STAT_COINCIDENCE = 6
} stat_bit;

typedef enum lcdc_bit
{
	LCDC_LCD_ON = 7,
	LCDC_WIN_TILEMAP = 6,
	LCDC_WIN_DISPLAY_ON = 5,
	LCDC_BG_WIN_TILE = 4,
	LCDC_BG_TILEMAP = 3,
	LCDC_OBJ_SIZE = 2,
	LCDC_OBJ_ENABLE = 1,
	LCDC_BG_DISPLAY = 0
} lcdc_bit;

typedef enum pixel_value
{
	PIXEL_OFF = 0,
	PIXEL_LIGHT = 1,
	PIXEL_DARK = 2,
	PIXEL_FULL = 3
} pixel_value;

typedef unsigned oam_entry;

extern ppu_mode gfxmode;
extern byte screen_bitmap[];

void ppu_tick(void);
void clear_tiles(void);
void update_tiles(void);
void scanline(void);
void debug_printscreen(void);
void dump_tile(pixel_value[][SPRITE_SIZE]);
void dump_oam(void);

static inline void set_stat(byte, bool);
static inline void set_mode_flag(ppu_mode);
static inline bool get_stat(stat_bit);
static inline bool get_lcdc(lcdc_bit);
static inline word get_tile_address(byte, byte, word);
static inline void get_tile(pixel_value[][SPRITE_SIZE], byte, word, bool, bool, bool);
static inline void free_tile(pixel_value**, bool);
static inline void set_palette(byte, pixel_value*);
static inline byte get_color(pixel_value);

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
static inline bool get_stat(stat_bit bit)
{
	return ((memory_get8(STAT) >> bit) & 1);
}
static inline bool get_lcdc(lcdc_bit bit)
{
	return ((memory_get8(LCDC) >> bit) & 1);
}
static inline word get_tile_address(byte y, byte x, word base)
{
	y >>= 3;
	x >>= 3;
	return (base + (y * 32) + x);
}
static inline void get_tile(pixel_value pixels[][SPRITE_SIZE], byte tileno, word base, bool flip_x, bool flip_y, bool is_large)
{
	word address;
	if(is_large) tileno &= 0xFE;
	byte sprite_y = is_large ? SPRITE_SIZE_LARGE : SPRITE_SIZE;
	//16 bytes per tile
	if(base == TILE_DATA_2_BEGIN && (tileno >> 7)) address = base - (((~tileno & 0xFF) + 1)*16);
	else address = base + (tileno * 16);
	word temp;
	for(unsigned i=0;i<sprite_y*2;i+=2)
	{
		temp = memory_get16(address + i);
		for(unsigned j=0;j<SPRITE_SIZE;j++)
		{
			pixels[flip_y ? (sprite_y-1)-(i>>1): i>>1][flip_x ? 7-j : j] = (((temp >> 8) >> (7 - j)) & 1) << 1 | ((temp >> (7 - j)) & 1);
		}
	}
}
static inline void get_tile_line(pixel_value *line, byte tileno, word base, bool flip_x, bool flip_y, bool is_large, byte tile_y)
{
	word address;
	if(is_large) tileno &= 0xFE;
	byte sprite_y = is_large ? SPRITE_SIZE_LARGE : SPRITE_SIZE;
	//16 bytes per tile
	if(base == TILE_DATA_2_BEGIN && (tileno >> 7)) address = base - (((~tileno & 0xFF) + 1)*16);
	else address = base + (tileno * 16);

	byte true_y = flip_y ? ((sprite_y - 1) - tile_y) : tile_y;
	word pixeldata = memory_get16(address + (2 * true_y));
	if(!flip_x) for(unsigned i=0;i<SPRITE_SIZE;i++) line[i] = (((pixeldata >> 8) >> (7 - i)) & 1) << 1 | ((pixeldata >> (7 - i)) & 1);
	else for(unsigned i=0;i<SPRITE_SIZE;i++) line[7 - i] = (((pixeldata >> 8) >> (7 - i)) & 1) << 1 | ((pixeldata >> (7 - i)) & 1);
}
static inline void free_tile(pixel_value **tile, bool big_sprites)
{
	byte sprite_y = big_sprites ? SPRITE_SIZE_LARGE : SPRITE_SIZE;
	for(unsigned i=0;i<sprite_y;i++) free(tile[i]);
	free(tile);
}
static inline void set_palette(byte palette, pixel_value *data)
{
	for(unsigned i=0;i<4;i++) data[i] = (palette >> i*2) & 3;
}
static inline byte get_color(pixel_value p)
{
	//TODO remap colors and convert to bitmap color
	return (byte) p;
}
