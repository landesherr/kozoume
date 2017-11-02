/*
	render.h

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

#pragma once
#include <SDL.h>
#include "globaldefs.h"

#define RENDER_WIDTH 160
#define RENDER_HEIGHT 144

SDL_Window *window;
SDL_Renderer *render;
SDL_Surface *surface;

//pixel values as 32-bit unsigned, from lightest to darkest
static unsigned rgb_px_val[4] = {0xE0E0E0, 0xA0A0A0, 0x404040, 0x000000};

byte bpp;
bool lock;

void render_init(void);
void render_scanline(unsigned);
