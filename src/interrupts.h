/*
	interrupts.h

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

#include "globaldefs.h"
#include "z80.h"
#include "memory.h"
#include <stdbool.h>

#define INTERRUPT_FLAG 0xFF0F
#define INTERRUPT_ENABLE 0xFFFF

#define NUM_INTERRUPTS 5
#define VECTOR_VBLANK 0x40
#define VECTOR_STAT 0x48
#define VECTOR_TIMER 0x50
#define VECTOR_SERIAL 0x58
#define VECTOR_JOYPAD 0x60

typedef enum interrupt
{
	INT_VBLANK = 0,
	INT_STAT = 1,
	INT_TIMER = 2,
	INT_SERIAL = 3,
	INT_JOYPAD = 4
} interrupt;

void do_interrupts(void);
void set_interrupt(interrupt, bool);
void enable_interrupt(interrupt, bool);
bool check_interrupts();

static inline void do_interrupt(interrupt);
static inline word get_interrupt_vector(interrupt);

static inline word get_interrupt_vector(interrupt i)
{
	switch(i)
	{
		case INT_VBLANK:
			return VECTOR_VBLANK;
		case INT_STAT:
			return VECTOR_STAT;
		case INT_TIMER:
			return VECTOR_TIMER;
		case INT_SERIAL:
			return VECTOR_SERIAL;
		case INT_JOYPAD:
			return VECTOR_JOYPAD;
		default:
			return 0;
	}
}
static inline void do_interrupt(interrupt i)
{
	set_interrupt(i, false);
	interruptsEnabled = false;
	*SP -= 2;
	memory_set16(*SP, *PC);
	*PC = get_interrupt_vector(i);
	cycles += 20;
}
