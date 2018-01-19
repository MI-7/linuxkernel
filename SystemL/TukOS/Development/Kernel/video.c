#include "video.h"
#include "common.h"

static char *videoram = (char*)0xB8000;
unsigned int videoramposition;

unsigned int video_init()
{
	get_cursor();

	printstring((unsigned char*)"Succes\n");			// Succes message for entering protected mode

	printstring((unsigned char*)"Initialising Video Settings... ");

	printstring((unsigned char*)"Succes\n");

return 0;
}

unsigned int set_cursor()
{
	outport(0x3D4, 0x0F);
	outport(0x3D5, (unsigned char)(videoramposition/2) & 0xFF);		// Set the lower byte location

    	outport(0x3D4, 0x0E);
   	outport(0x3D5, (unsigned char)((videoramposition/2)>>8) & 0xFF);	// Set the upper byte location

return 0;
}

unsigned int get_cursor()
{
	unsigned char portlocation;
	unsigned char addressfield;
	unsigned char lowlocation;
	unsigned char highlocation;

	unsigned int cursorlocation;

	portlocation = inport(0x03CC);				// Check the location of the VGA registers


	if(portlocation & 1)					// Check if bit 0 is set to 1
	{	
		addressfield = inport(0x03D4);			// Safe the current address r

		outport(0x03D4, (unsigned char)0x0F);		
		lowlocation = inport(0x03D5);			// Get the lower byte location

		outport(0x03D4, (unsigned char)0x0E);
		highlocation = inport(0x03D5);			// Get the upper byte location

		outport(0x03D4, addressfield);			// Restore the address register

	}

	cursorlocation = lowlocation + (highlocation << 8);	// Add the higher to the lower location
	
	videoramposition = cursorlocation * 2;			// Multiply by 2 due to 2 bytes per location

return 0;
}

unsigned int printstring (unsigned char* string)
{
	int i;

	for (i = 0; string[i] != '\0'; i++)			// Check for the end of the string
	{
				switch(string[i])		// Check for special characters
				{
					case '\n':
						newline();
					break;

					default:
						if(videoramposition < 3996)
						{
							videoram[videoramposition] = string[i];
							videoramposition += 2;
						}

						else
						{
							scrolldown();
							i--;
						}
					break;
				}
	}

	set_cursor();		// Update the cursor's position

return 0;
}

unsigned int printint (unsigned int integer)
{
	unsigned char *string;
	string = convert_int_ascii((unsigned int)integer);	// Convert integer to hex
	printstring((unsigned char*)"0x");			
	printstring(string);
	
return 0;
}

unsigned int clearscreen(unsigned int attribute)
{
	unsigned int i;

	for (i = 0; i <= 4000; i++)			// A total of 4000 bytes in the videoram
	{
		videoram[i] = ' ';
		i++;
		videoram[i] = attribute;		// Set the attribute byte
	}
	videoramposition = 0;
				
	set_cursor();
return 0;
}

unsigned int newline()
{

	unsigned int currentline = videoramposition / 160;	// Get the currentline

	if(currentline == 24)
	{
		scrolldown();
		videoramposition = 3840;
	}

	else
	{
		videoramposition = (currentline + 1)*160;		// Increase the location
	}
return 0;
}

unsigned int scrolldown()
{
	unsigned int i;
	
	for(i = 0; i <= 3840; i++)
	{
		videoram[i] = videoram[i+160];			// Shift all videoram 160 bytes back
	}
	
	for(i = 3842; i <= 4000; i++)
	{
		videoram[i] = ' ';
		i++;
	}
	
	

	videoramposition = videoramposition - 160;

return 0;
}


