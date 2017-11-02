/*
	render.c

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

#include <SDL.h>
#include <SDL_endian.h>
#include <stdlib.h>
#include <stdbool.h>
#include "render.h"
#include "ppu.h"
#include "globaldefs.h"

static inline void putpixel(unsigned, unsigned, pixel_value);

void render_init()
{
	if(SDL_Init(SDL_INIT_VIDEO))
	{
		//message
		exit(1);
	}
	window = SDL_CreateWindow("kozoume", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, RENDER_WIDTH, RENDER_HEIGHT, SDL_WINDOW_SHOWN);
	if(!window)
	{
		//message
		exit(1);
	}
	surface = SDL_GetWindowSurface(window);
	render = SDL_GetRenderer(window);
	bpp = surface->format->BytesPerPixel;
	lock = SDL_MUSTLOCK(surface);
}

//probably refactor as render_screen for performance reasons once debugging is over
void render_scanline(unsigned lineno)
{
	if(lock) SDL_LockSurface(surface);
	for(unsigned i=0;i<SCREEN_RES_X;i++) putpixel(lineno, i, PIXEL(lineno, i));
	if(lock) SDL_UnlockSurface(surface);
	SDL_RenderPresent(render);
}

static inline void putpixel(unsigned y, unsigned x, pixel_value px)
{
	byte *pixel_pointer = (byte*)(surface->pixels);
	pixel_pointer += (y * surface->pitch) + (x * bpp);
	switch(bpp)
	{
		case 1:
			*pixel_pointer = rgb_px_val[px];
			break;
		case 2:
			(*(word*) pixel_pointer) = rgb_px_val[px];
			break;
		case 3:
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN) 
				for(int i=0;i<3;i++) pixel_pointer[i] = (rgb_px_val[px] >> (i*8)) & 0xFF;
			else 
				for(int i=0;i<3;i++) pixel_pointer[i] = (rgb_px_val[px] >> (16 - (i*8))) & 0xFF;
			break;
		case 4:
			(*(unsigned*) pixel_pointer) = rgb_px_val[px];
			break;
		default:
			//message
			exit(1);
	}
}
