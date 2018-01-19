#ifndef COMMON_H
#define	COMMON_H

void outport(unsigned short port, unsigned char val);
unsigned char inport(unsigned short port);
void io_wait( void );

unsigned char* convert_int_ascii(unsigned int value);
unsigned int strcmp(unsigned char*, unsigned char*);

#endif
