#include "stdio.h"
#include "keyboard.h"
#include "video.h"

unsigned char getchar()
{
	unsigned char returnchar = 0;

	keyboardbuffer(2, 1);
	
	while(returnchar == 0)
	{
		if(keyboardbuffer(4, 0) != 0)
			returnchar = keyboardbuffer(3, 0);
	}

	keyboardbuffer(2, 0);

return returnchar;
}

unsigned int printchar(unsigned char character)
{
	unsigned char stringarray[2] = {0, 0};			// Null terminating character 
	unsigned char *string = stringarray;

	stringarray[0] = character;
	
	printstring(string);

	set_cursor();
return 0;
}
