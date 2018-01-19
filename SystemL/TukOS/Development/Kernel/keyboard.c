#include "keyboard.h"

unsigned char convert_scancode_ascii(unsigned char scancode)
{
	unsigned char asciitable[2][38] = 	{{0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x39, 0x1C},
						{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '\n'}};

	unsigned int i;

	for(i = 0; i < sizeof(asciitable[0]); i++)
	{
		if(scancode == asciitable[0][i])
		{
			return asciitable[1][i];
		}

		else if(scancode == asciitable[0][i] + 0x80)
		{
			return 0;
		}
	}

return 0;
}

unsigned char keyboardbuffer(unsigned char function, unsigned char argument)
{
	static unsigned char bufferkey = 0;
	static unsigned char activate = 0;
	static unsigned char update = 0;

	if(function == 1 && activate == 1)	// Save the bufferkey + set the update byte
	{
		bufferkey = argument;
		update = 1;
		return 0;
	}

	else if(function == 2)		// Disable/Activate the buffer
	{
		activate = 1;
		update = 0;
		return 0;
	}

	else if(function == 3 && activate == 1)	// Return the buffered char
	{
		return bufferkey;
	}

	else if(function == 4)	// Return the updated flag status
		return update;

	return 0;
	
return 0;
}
