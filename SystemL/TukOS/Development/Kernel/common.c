#include "common.h"
#include "maths.h"


void outport(unsigned short port, unsigned char val)
{
	asm volatile( "outb %0, %1" : : "a"(val), "Nd"(port) );
	io_wait();
}

unsigned char inport(unsigned short port)
{
	unsigned char ret;

	asm volatile( "inb %1, %0" : "=a"(ret) : "Nd"(port) );

	io_wait();
	return ret;
}

void io_wait( void )
{
	// port 0x80 is used for 'checkpoints' during POST.
	// The Linux kernel seems to think it is free for use :-/
    	asm volatile( "outb %%al, $0x80" : : "a"(0) );
}


unsigned char* convert_int_ascii(unsigned int value)	// The value is the integer which will be converted
{
	//unsigned char conversion[2][16] = {	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'}};
	unsigned char stringarray[9];							// Initialise the string array
	unsigned char *string = stringarray;						// Pointer to the string
	unsigned int n = 0;								// Bool to check for the first s.f.	
	unsigned int i = 0;								// Int to determine array entry
	unsigned int endofarray = 8;
	unsigned int backupvalue = value;
	
	while(i < endofarray)		// Check if the string array boundry is reached
	{
		if (n == 0 && ((value & 0xF0000000) >> 28) == 0)	// Find the first s.f.
		{
			value = value << 4;				// Shift towards the end
			endofarray--;					// Less numbers fit into the array
			string++;					// Increase the pointer by one to remove first zeroes
			if (endofarray == 0)				// If the value is 0, print a 0
			{
				string--;				// Reduce the pointer by one and add a zero to the string
				string[i] = '0';
				string--;
				string[i] = '0';			
			}
			continue;					// Skip the loop and search for the first s.f. again
		}		
		else
			n = 1;						// First s.f. is found

		string[i] = (value & 0xF0000000) >> 28;			// Put the highest 4 bits into the string
		
			if(string[i] < 10)
				string[i] = string[i] + 48;		// Convert numbers to ascii
			else if(string[i] < 16)
				string[i] = string[i] + 55;		// Convert letters to ascii
		i++;							// Next char in the array
		value = value << 4;					// Shift the value to the end
	}

	if(backupvalue < 16 && backupvalue != 0)			// Print two characters minimum
	{
		string--;
		string[0] = '0';					// Add an extra zero if the string is 1 digit
	}
		
	return string;			// Return the pointer with the string
}

unsigned int strcmp(unsigned char* string1, unsigned char* string2)
{
	unsigned int i = 0;
	unsigned int n = 0;

	while(string1[i] == string2[i])
	{
		n++;

		if(string1[i] == '\0')
			return n;
		i++;
	}
return n;
}
/*
char* convert_int_ascii(unsigned int integer)
{
	unsigned int testinteger = integer;
	unsigned int tempinteger = integer;
	char *string;
	unsigned int i = 0;
	unsigned int n = 0;	

	while(1) {
		if (testinteger < 10) {
			string[i] = testinteger + 48;
			i++;
			testinteger = tempinteger - testinteger*pow(10, n);
			n = 0;
			tempinteger = testinteger;
				
				if (testinteger < 10) {
					string[i] = testinteger + 48;
					i++;
					string[i] = '\0';
					break;
				}
			
		}

		testinteger = testinteger/10;
		n++;
	}

	return string;
}
*/		
	
			
	
	
