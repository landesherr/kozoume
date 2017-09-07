#include "console.h"
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
		if(address == 0) goto badcmd;
		cmd->action = RUNTO;
		cmd->param.numeric = address & 0xFFFF;
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

