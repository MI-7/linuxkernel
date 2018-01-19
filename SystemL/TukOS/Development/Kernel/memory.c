#include "memory.h"
#include "video.h"
#include "stdio.h"

unsigned int mapmemory(unsigned char* startingaddress, unsigned int length)
{
	unsigned int i, n;
	printstring((unsigned char*)"\n");

	for(i = 0; i < length; i++)
	{	
		printint((unsigned int)startingaddress[i]);
		printstring((unsigned char*)"   ");		// Space between addresses

		if(((i+1)%8) == 0)				// 8 addresses have been print
		{
			for(n = i-7; n <= i; n++)		// Print 8 ascii character next to the addresses
			{
				printchar((unsigned char)startingaddress[n]);
			}

			printstring((unsigned char*)"\n");	// After printing 8 address locations, start a new line
		}
	}

	printstring((unsigned char*)"\n");	

return 0;
}

unsigned int memset(unsigned char *pointer, unsigned char value, unsigned int number)
{
	unsigned int i;

	for(i = 0; i < number; i++)
	{
		pointer[i] = value;
	}
return 0;
}
