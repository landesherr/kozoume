/*
	render.c

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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <time.h>
#include "io.h"
#include "ppu.h"
#include "render.h"

#define WINDOW_TITLE "Kozoume"
#define FRAME_TIME (1.0 / 60.0)
#define DEFAULT_SCALE 5

#define DC bw //Default Colorset
color vapor[4] = { 0xCCFFFF, 0xF1DAFF, 0xE3E8FF, 0xFFCCFF };
color cool[4] = { 0xFF6AD5, 0xC774E8, 0xAD8CFF, 0x879E58 };
color original[4] = { 0x9BBC0F, 0x8BAC0F, 0x306230, 0x0F380F };
color bw[4] = { 0xCCCCCC, 0x999999, 0x666666, 0x000000 };

SDL_Window *window;
SDL_Renderer *renderer;
clock_t timer = 0;
bool initialized = false;

static inline void set_color(byte, color[]);

void render_init()
{
	if(initialized) return;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_RES_X * DEFAULT_SCALE, SCREEN_RES_Y * DEFAULT_SCALE, SDL_WINDOW_SHOWN);
	SDL_SetWindowResizable(window, SDL_TRUE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);
	SDL_RenderSetLogicalSize(renderer, SCREEN_RES_X, SCREEN_RES_Y);
	initialized = true;
}

void render_teardown()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	initialized = false;
}

void render_screen()
{
	//TODO: Event polling goes somewhere else (probably w/ input when that becomes a thing)
	SDL_Event event;
	SDL_PollEvent(&event);
	if(event.type == SDL_QUIT)
	{
		render_teardown();
		exit(0);
	}
	set_joypad_state(&event);
	if(!initialized) return;
	if(!timer) timer = clock();
	else
	{
		clock_t current = clock();
		double delta = (double)(current - timer) / CLOCKS_PER_SEC;
		if(delta < FRAME_TIME) SDL_Delay((FRAME_TIME - delta) * 1000);
		timer = current;
	}
	SDL_RenderClear(renderer);
	for(int y=0;y<SCREEN_RES_Y;y++)
	{
		for(int x=0;x<SCREEN_RES_X;x++)
		{
			//TODO Clean this up w/ a macro and avoid numeric literals
			set_color(PIXEL(y,x), DC);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
	SDL_RenderPresent(renderer);
}

static inline void set_color(byte colorno, color colorset[])
{
	colorno &= 3;
	color c = colorset[colorno];
	SDL_SetRenderDrawColor(renderer, GET_R(c), GET_G(c), GET_B(c), ALPHA);
}
