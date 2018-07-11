/*
	io.h

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
#include <stdbool.h>
#include <SDL2/SDL.h>

#define JOY_RIGHT  0x1
#define JOY_LEFT   0x2
#define JOY_UP     0x4
#define JOY_DOWN   0x8
#define JOY_A      0x10
#define JOY_B      0x20
#define JOY_SELECT 0x40
#define JOY_START  0x80

#define CHECK_JOYPAD_P15 true
#define CHECK_JOYPAD_P14 false

void io_tick(void);
void div_tick(void);
void tima_tick(void);

void dma_transfer(void);
void check_joypad(bool);
void set_joypad_state(SDL_Event*);
