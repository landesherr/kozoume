/*
	ppu.c

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

#include "ppu.h"
#include "z80.h"
#include "memory.h"
#include "io.h"
#include "interpreter.h"
#include "interrupts.h"
#include "globaldefs.h"
#include "render.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define LINE_TO_TILE(LINE) ((LINE) >> 3)
#define NUM_OAM_ENTRIES 40

ppu_mode gfxmode = VBLANK;
static unsigned hblank_count = 153, mode_cycles = 4500;
const byte text_pixels[4] = {'#', '=', '-', ' '};
byte *screen_bitmap;

void ppu_tick()
{
	if(!LCD_ON)
	{
		gfxmode = 0;
		hblank_count = 0;
		mode_cycles = 0;
		UPDATE_LY();
		return;
	}
	//144 search->transfer->hblank cycles followed by vblank
	//hblank = 456 cycles
	//(80 mode 2, 172 mode 3, 204 mode 0)
	//vblank = 4560 cycles
	//refresh = 70224 cycles
	mode_cycles += (cycles - cycles_prev);
	switch(gfxmode)
	{
		case HBLANK:
			if(mode_cycles >= MODE_0_CYCLES)
			{
				mode_cycles -= MODE_0_CYCLES;
				hblank_count++;
				UPDATE_LY();
				if(hblank_count == SCREEN_RES_Y)
				{
					render_screen();
					gfxmode = VBLANK;
					set_interrupt(INT_VBLANK, true);
				}
				else
				{
					gfxmode = SEARCH;
				}
				set_mode_flag(gfxmode);
			}
			//TODO actual HBLANK logic
			break;
		case VBLANK:
			if(mode_cycles >= MODE_1_CYCLES)
			{
				mode_cycles -= MODE_1_CYCLES;
				gfxmode = SEARCH;
				set_mode_flag(gfxmode);
				hblank_count = 0;
				UPDATE_LY();
			}
			else if(mode_cycles % (MODE_0_CYCLES + MODE_2_CYCLES + MODE_3_CYCLES) == 0)
			{
				hblank_count++;
				UPDATE_LY();
			}
			//TODO actual VBLANK logic
			break;
		case SEARCH:
			if(mode_cycles >= MODE_2_CYCLES)
			{
				mode_cycles -= MODE_2_CYCLES;
				gfxmode = TRANSFER;
				set_mode_flag(gfxmode);
				mode_cycles = 0;
			}
			//TODO actual SEARCH logic
			break;
		case TRANSFER:
			if(mode_cycles >= MODE_3_CYCLES)
			{
				mode_cycles -= MODE_3_CYCLES;
				gfxmode = HBLANK;
				set_mode_flag(gfxmode);
				scanline();
			}
			//TODO actual TRANSFER logic
			break;
		default:
			dbgwrite("Invalid PPU mode! How did this even happen?\n");
	}
	set_stat(STAT_COINCIDENCE_FLAG, LY_COMPARE);
	//TODO is there any additional interrupt-specific logic needed below?
	if(gfxmode == HBLANK && HBLANK_INTERRUPT) set_interrupt(INT_STAT, true);
	else if(gfxmode == VBLANK && VBLANK_INTERRUPT) set_interrupt(INT_STAT, true);
	else if(gfxmode == SEARCH && OAM_INTERRUPT) set_interrupt(INT_STAT, true);
	else if(LY_COMPARE && COINCIDENCE_INTERRUPT) set_interrupt(INT_STAT, true);
}

void scanline()
{
	if(!screen_bitmap) screen_bitmap = calloc(SCREEN_RES_X * SCREEN_RES_Y, sizeof(byte));
	word map_bg = get_lcdc(LCDC_BG_TILEMAP) ? TILE_MAP_2_BEGIN : TILE_MAP_1_BEGIN;
	word map_win = get_lcdc(LCDC_WIN_TILEMAP) ? TILE_MAP_2_BEGIN : TILE_MAP_1_BEGIN;

	word tile_data= get_lcdc(LCDC_BG_WIN_TILE) ? TILE_DATA_1_BEGIN : TILE_DATA_2_BEGIN;

	byte ly = memory_get8(LY);
	byte scroll_x = memory_get8(SCX);
	byte scroll_y = memory_get8(SCY);
	byte window_y = memory_get8(WY);
	byte window_x = memory_get8(WX) - 7;
	bool use_window;

	byte y = ly + scroll_y;
	byte x;

	byte tile_y = y & 7;
	byte tile_x;
	byte temptile;
	static byte current_bg_tile, current_win_tile;
	static pixel_value bgtile[SPRITE_SIZE][SPRITE_SIZE] = {0}, oamtile[SPRITE_SIZE_LARGE][SPRITE_SIZE] = {0}, wintile[SPRITE_SIZE][SPRITE_SIZE] = {0};
	oam_entry current_entry;
	byte yc, xc, tile_no, oam_options;

	byte oam_palette;
	pixel_value bg_palette_data[4], oam_palette_data[4];
	if(get_lcdc(LCDC_BG_DISPLAY))
	{
		set_palette(memory_get8(BGP), bg_palette_data);
		for(unsigned i=0;i<SCREEN_RES_X;i++)
		{
			x = i + scroll_x;
			tile_x = x & 7;
			temptile = memory_get8(get_tile_address(y, x, map_bg));
			if(temptile != current_bg_tile || i == 0)
			{
				current_bg_tile = temptile;
				get_tile(bgtile, current_bg_tile, tile_data, false, false, false);
			}
			//Window stuff
			if(get_lcdc(LCDC_WIN_DISPLAY_ON) && window_x <= 166 && window_x <= i && window_y <= ly)
			{
				use_window = true;
				temptile = memory_get8(get_tile_address(ly - window_y, i - window_x, map_win));
				if(temptile != current_win_tile || wintile == NULL)
				{
					current_win_tile = temptile;
					get_tile(wintile, current_win_tile, tile_data, false, false, false);
				}
			}
			else use_window = false;
			PIXEL(ly, i) = use_window ? bg_palette_data[wintile[ly & 7][i & 7]] : bg_palette_data[bgtile[tile_y][tile_x]];
		}
	}
	if(get_lcdc(LCDC_OBJ_ENABLE)) //if drawing sprites is enabled
	{
		bool big_sprites = get_lcdc(LCDC_OBJ_SIZE);
		byte sprite_size_y = big_sprites ? SPRITE_SIZE_LARGE : SPRITE_SIZE;
		for(unsigned j=0;j<NUM_OAM_ENTRIES;j++)
		{
			current_entry = (oam_entry) memory_get32(oam.lower + (j * sizeof(oam_entry)));
			if(!current_entry) continue;
			yc = OAM_Y_COORD(current_entry);
			xc = OAM_X_COORD(current_entry);
			if(yc == 0 || xc == 0) continue;
			yc -= 16;
			xc -= 8;
			oam_options = OAM_OPTIONS(current_entry);
			tile_no = OAM_TILE_NO(current_entry);
			get_tile(oamtile, tile_no, TILE_DATA_1_BEGIN, (oam_options >> 5) & 1, (oam_options >> 6) & 1, big_sprites);
			if(yc <= ly && (yc + sprite_size_y) > ly)
			{
				if(big_sprites) tile_y = (ly - yc) & 0xF;
				else tile_y = (ly - yc) & 0x7;
				oam_palette = (oam_options >> 4) & 1 ? memory_get8(OBP1) : memory_get8(OBP0);
				set_palette(oam_palette, oam_palette_data);
				for(unsigned k=0;k<SPRITE_SIZE;k++)
				{
					if((xc + k) >= 0 && (xc + k) < SCREEN_RES_X)
					{
						tile_x = k;

						//only draw if high priority or no other pixel exists here
						if(oam_options >> 7 == 0 || PIXEL(ly, xc+k) == PIXEL_OFF)
						{
							if(oamtile[tile_y][tile_x] <= 3 && oamtile[tile_y][tile_x] != 0)
								PIXEL(ly, xc+k) = oam_palette_data[oamtile[tile_y][tile_x]];
						}
					}
				}
			}
		}

	}
}

void dump_tile(pixel_value tile[][SPRITE_SIZE])
{
	for(unsigned j=0;j<8;j++)
	{
		for(unsigned k=0;k<8;k++)
		{
			printf("%c", text_pixels[tile[j][k] & 3]);
		}
		printf("\n");
	}
	printf("\n");
}

void dump_oam()
{
	pixel_value oamtile[SPRITE_SIZE_LARGE][SPRITE_SIZE] = {0};
	for(unsigned i=0;i<NUM_OAM_ENTRIES;i++)
	{
		byte current_entry = (oam_entry) memory_get32(oam.lower + (i * sizeof(oam_entry)));
		get_tile(oamtile, OAM_TILE_NO(current_entry), TILE_DATA_1_BEGIN, false, false, false);
		dump_tile(oamtile);
	}
}

void debug_printscreen()
{
	//printf("\033[2J\033[1;1H"); //clear terminal
	printf("\033c");
	for(unsigned i=0;i<SCREEN_RES_Y;i++)
	{
		for(unsigned j=0;j<SCREEN_RES_X;j++)
		{
			printf("%c", text_pixels[PIXEL(i,j)]);
		}
		printf("\n");
	}
	fflush(stdout);
	usleep(10000);
	/*
	for(unsigned i=0;i<SCREEN_RES_Y-1;i+=2)
	{
		char out = '#';
		for(unsigned j=0;j<SCREEN_RES_X;j++)
		{
			switch(PIXEL(i,j))
			{
				case 0:
					switch(PIXEL(i+1,j))
					{
						case 0:
							out = '#';
							break;
						case 1:
							out = '9';
							break;
						case 2:
							out = 'T';
							break;
						case 3:
							out = '\"';
							break;
					}
					break;
				case 1:
					switch(PIXEL(i+1,j))
					{
						case 0:
							out = '&';
							break;
						case 1:
							out = 'X';
							break;
						case 2:
							out = 'F';
							break;
						case 3:
							out = '`';
							break;
					}
					break;
				case 2:
					switch(PIXEL(i+1,j))
					{
						case 0:
							out = 'L';
							break;
						case 1:
							out = 'i';
							break;
						case 2:
							out = '=';
							break;
						case 3:
							out = '\'';
							break;
					}
					break;
				case 3:
					switch(PIXEL(i+1,j))
					{
						case 0:
							out = '_';
							break;
						case 1:
							out = ',';
							break;
						case 2:
							out = '.';
							break;
						case 3:
							out = ' ';
							break;
					}
					break;
			}
			printf("%c", out);
		}
		printf("\n");
	}
	*/
}
