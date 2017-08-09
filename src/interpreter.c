/*
	interpreter.c
	
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


#include "interpreter.h"
#include "z80.h"
#include "opcodes.h"
#include "memory.h"
#include "io.h"

unsigned cycles_prev;

void interpreter_step()
{
	//io_tick();
	//if(interruptsEnabled) do_interrupts();
	cycles_prev = cycles;

	if(!prefixCB) standard_opcodes[FETCH()]();
	else
	{
		prefix_opcodes[FETCH()]();
		prefixCB = false;
	}
}
