#include "shell.h"
#include "stdio.h"
#include "memory.h"
#include "video.h"
#include "common.h"

unsigned int shell()
{
	unsigned char argumentstring[200];
	unsigned char* argument = argumentstring;
	unsigned int i = 0;
	unsigned char character;
	printstring((unsigned char*)"Initialising shell... Succes\n");
	printstring((unsigned char*)"\nEnter command: ");
	while(1)
	{
		if( i == 200)
		{
			printstring((unsigned char*)"\nArgument is too long, please reduce size");
			memset(argument, 0, 200);
			i = 0;
			printstring((unsigned char*)"\n");
			continue;
			
		}

		character = getchar();

		if(character != '\n')
		{
			printchar(character);
			argument[i] = character;
			i++;
		}

		else if(character == '\n')
		{
			processargument(argument);
			memset(argument, 0, 200);
			i = 0;
			printstring((unsigned char*)"\n\nEnter command: ");
		}
	}

return 0;
}

unsigned int processargument(unsigned char* argument)
{
	printstring((unsigned char*)"\n");

	if(strcmp((unsigned char*)"MAPMEMORY", argument) == sizeof("MAPMEMORY"))
	{

	}

	else
		printstring((unsigned char*)"Test failed");
return 0;	
}
