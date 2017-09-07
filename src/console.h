#pragma once

typedef enum console_action
{
	RUNTO,
	STEP,
	SETBRK,
	CLRBRK,
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

console_command* console_get_command(void);
