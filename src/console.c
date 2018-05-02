/*
	console.c

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

#include "console.h"
#include "globaldefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

console_command* console_get_command()
{
	char command[80];
	char **words;
	unsigned i;
	unsigned word = 0, wordpos = 0;
	console_command *cmd = malloc(sizeof(console_command));
	printf("> ");
	fgets(command, 80, stdin);
	words = calloc(40, sizeof(char*));
	words[0] = calloc(80, sizeof(char));
	for(i=0;i<80;i++)
	{
		if(command[i] == '\0')
		{
			words[word][wordpos] = '\0';
			break;
		}
		if(command[i] == ' ')
		{
			words[word][wordpos] = '\0';
			word++;
		 	wordpos = 0;
			words[word] = calloc(80, sizeof(char));
		}
		else if((command[i] > 96 && command[i] < 123) || (command[i] > 47 && command[i] < 58))
		{
			words[word][wordpos++] = command[i];
		}
	}

	if(!strcmp(words[0], "runto"))
	{
		if(words == 0) goto badcmd;
		unsigned address = strtol(words[1], NULL, 16);
		//if(address > 0xFFFF) goto badcmd;
		cmd->action = RUNTO;
		cmd->param.numeric = address & 0xFFFF;
	}
	else if(!strcmp(words[0], "runop"))
	{
		if(words == 0) goto badcmd;
		byte op = strtol(words[1], NULL, 16);
		cmd->action = RUNOP;
		cmd->param.numeric = op;
	}
	else if(!strcmp(words[0], "runtil"))
	{
		if(words == 0) goto badcmd;
		cmd->action = RUNTIL;
		cmd->param.str = words[1];
	}
	else if(!strcmp(words[0], "peek"))
	{
		if(words == 0) goto badcmd;
		unsigned address = strtol(words[1], NULL, 16);
		if(address == 0) goto badcmd;
		cmd->action = PEEK;
		cmd->param.numeric = address & 0xFFFF;
	}
	else if(!strcmp(words[0], "dumpoam"))
	{
		cmd->action = DUMP_OAM;
	}
	else if(!strcmp(words[0], "setbrk"))
	{
		if(words == 0) goto badcmd;
		unsigned address = strtol(words[1], NULL, 16);
		if(address == 0) goto badcmd;
		cmd->action = SETBRK;
		cmd->param.numeric = address & 0xFFFF;
	}
	else if(!strcmp(words[0], "clrbrk"))
	{
		cmd->action = CLRBRK;
	}
	else if(!strcmp(words[0], "step") || !strcmp(words[0], "s"))
	{
		cmd->action = STEP;
	}
	else if(!strcmp(words[0], "run"))
	{
		cmd->action = RUN;
	}
	else if(!strcmp(words[0], "quit"))
	{
		cmd->action = QUIT;
	}
	else goto badcmd;
	goto goodcmd;
badcmd:
	printf("\nInvalid Command!\n");
	cmd->action = BADCMD;
goodcmd:
	for(i=0;i<word;i++) free(words[i]);
	free(words);
	return cmd;
}


