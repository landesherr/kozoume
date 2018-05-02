/*
	kozoume.c

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "z80.h"
#include "cartridge.h"
#include "globaldefs.h"
#include "opcodes.h"
#include "interpreter.h"
#include "interrupts.h"
#include "console.h"
#include "ppu.h" //can be removed when debugging is finished
#include "render.h"

void powerup(void);

int main(int argc, char *argv[])
{
	unsigned go = 1, run = 0, runto = 0;
	bool debug_print = false;
	byte triggerop;
	runtil_condition rc = NO_RUNTIL;
	console_command *cmd;
	memory_init();
	powerup();
	render_init();
	if(argc < 2)
	{
		dbgwrite("No ROM specified\n");
		exit(0);
	}
	else if(argc > 2)
	{
		if(!strcmp(argv[2], "debug")) debug_print = true;
	}
	cartridge *this_cart = load_cart(argv[1]);
	//basic_sanity_check();
	while(go)
	{
		if(debug_print) dbgwrite("Current PC: %X - Opcode: %X \n", *PC, memory_get8(*PC));
		interpreter_step();
		if(debug_print)
		{
			dbgwrite("A=%X F=%X \nB=%X C=%X \nD=%X E=%X \nH=%X L=%X \n", *A, *F, *B, *C, *D, *E, *H, *L);
			dbgwrite("Stack Pointer is %X \n", *SP);
			dbgwrite("(HL) is %X \n", memory_get16(*HL));
			dbgwrite("Last Stack Value is %X \n", memory_get16(*SP));
			dbgwrite("PPU mode is %u\n", gfxmode);
			dbgwrite("LCD is %s\n", LCD_ON ? "ON" : "OFF");
			dbgwrite("IME: %s\n", interruptsEnabled ? "enabled" : "disabled");
			dbgwrite("FFFF: %X\n", memory_get8(0xFFFF));
			dbgwrite("FF0F: %X\n", memory_get8(0xFF0F));
			dbgwrite("Cycles = %u, Prev = %u\n", cycles, cycles_prev);
			dbgwrite("DIV: %x | TIMA: %x, TMA: %x, TAC: %x\n", memory_get8(DIV), memory_get8(TIMA), memory_get8(TMA), memory_get8(TAC));
			dbgwrite("Cartridge Bank: %d\n", this_cart->bank);
		}
		if(!run)
		{
cmdagain:
			do {cmd = console_get_command();} while(cmd->action == BADCMD);
			if(cmd->action == QUIT)
			{
				dbgwrite("Quitting...\n");
				go = 0;
			}
			else if(cmd->action == RUN) run = 1;
			else if(cmd->action == RUNTO)
			{
				run = 1;
				runto = cmd->param.numeric;
			}
			else if(cmd->action == RUNOP)
			{
				run = 1;
				rc = UNTIL_OP;
				triggerop = cmd->param.numeric;
			}
			else if(cmd->action == RUNTIL)
			{
				run = 1;
				if(!strcmp(cmd->param.str, "vblank")) rc = UNTIL_VBLANK;
				else if(!strcmp(cmd->param.str, "jump")) rc = UNTIL_JUMP;
				else if(!strcmp(cmd->param.str, "interrupt")) rc = UNTIL_INTERRUPT;
				else if(!strcmp(cmd->param.str, "hblank")) rc = UNTIL_HBLANK;
				else
				{
					free(cmd);
					printf("\nInvalid parameter!\n");
					goto cmdagain;
				}
			}
			else if(cmd->action == PEEK)
			{
				printf("\nMemory value of %X: %X\n", cmd->param.numeric, memory_get8(cmd->param.numeric));
				free(cmd);
				goto cmdagain;
			}
			else if(cmd->action == DUMP_OAM)
			{
				dump_oam();
				free(cmd);
				goto cmdagain;
			}
			free(cmd);
		}
		else if(runto)
		{
			if(*PC == runto)
			{
				run = 0;
				runto = 0;
			}
		}
		else if(rc != NO_RUNTIL)
		{
			if((rc == UNTIL_VBLANK && gfxmode == 1) ||
				(rc == UNTIL_HBLANK && gfxmode == 0 && LCD_ON) ||
				(rc == UNTIL_JUMP && false) || //lazy, finish later
				(rc == UNTIL_INTERRUPT && false) ||//ditto
				(rc == UNTIL_OP && memory_get8(*PC) == triggerop)
			)
			{
				run = 0;
				rc = NO_RUNTIL;
			}
		}
		//if(*PC >= 0x8000) go = 0;
		//if(*PC < 100 && memory_get8(*PC) > 0xf0) go = 0;
	}
	memory_free();
	render_teardown();
	return 0;
}

//set memory/register values to defaults
void powerup()
{
	*A = 0x01;
	*F = 0xB0;
	*BC = 0x13;
	*DE = 0xD8;
	*HL = 0x14D;
	*SP = 0xFFFE;
	*PC = 0x100;
	//The following addresses are 0x00 and will be automatically set by calloc()
	//FF05, FF06, FF07, FF17, FF21, FF22, FF42, FF43, FF45, FF4A, FF4B, FFFF
	memory_set8(0xFF10, 0x80);
	memory_set8(0xFF11, 0xBF);
	memory_set8(0xFF12, 0xF3);
	memory_set8(0xFF14, 0xBF);
	memory_set8(0xFF16, 0x3F);
	memory_set8(0xFF19, 0xBF);
	memory_set8(0xFF1A, 0x7F);
	memory_set8(0xFF1B, 0xFF);
	memory_set8(0xFF1C, 0x9F);
	memory_set8(0xFF1E, 0xBF);
	memory_set8(0xFF20, 0xFF);
	memory_set8(0xFF23, 0xBF);
	memory_set8(0xFF24, 0x77);
	memory_set8(0xFF25, 0xF3);
	memory_set8(0xFF26, 0xF1); //F0 on SGB
	memory_set8(0xFF40, 0x91);
	memory_set8(0xFF47, 0xFC);
	memory_set8(0xFF48, 0xFF);
	memory_set8(0xFF49, 0xFF);
	memory_set8(0xFF0F, 0x01); //Set VBLANK to true for post-BIOS accuracy
}

