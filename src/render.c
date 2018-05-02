#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <time.h>
#include "ppu.h"
#include "render.h"

#define WINDOW_TITLE "Kozoume"
#define FRAME_TIME (1.0 / 60.0)

#define DC cool //Default Colorset
color vapor[4] = { 0xCCFFFF, 0xF1DAFF, 0xE3E8FF, 0xFFCCFF };
color cool[4] = { 0xFF6AD5, 0xC774E8, 0xAD8CFF, 0x879E58 };
color original[4] = { 0x9BBC0F, 0x8BAC0F, 0x306230, 0x0F380F };

SDL_Window *window;
SDL_Renderer *renderer;
clock_t timer = 0;
bool initialized = false;

static inline void set_color(byte, color[]);

void render_init()
{
	if(initialized) return;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_RES_X, SCREEN_RES_Y, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
