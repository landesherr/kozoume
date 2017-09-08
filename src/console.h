/*
	console.h

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

typedef enum console_action
{
	RUNTO,
	RUNTIL,
	STEP,
	SETBRK,
	CLRBRK,
	PEEK,
	RUN,
	QUIT,
	BADCMD
} console_action;

typedef union console_param
{
	unsigned numeric;
	char *str;
} console_param;

typedef struct console_command
{
	//unsigned num_params;
	console_action action;
	console_param param;
	//console_param *params;
} console_command;

typedef enum runtil_condition
{
	UNTIL_VBLANK,
	UNTIL_JUMP,
	UNTIL_INTERRUPT,
	NO_RUNTIL
} runtil_condition;

console_command* console_get_command(void);
