#include "io.h"

/*
Sends a 8/16/32-bit value on a I/O location. Traditional names are outb, outw and outl respectively. The a modifier enforces val to be placed in the eax register before the asm command is issued and Nd allows for one-byte constant values to be assembled as constants, freeing the edx register for other cases.
*/
static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

/*
Receives a 8/16/32-bit value from an I/O location. Traditional names are inb, inw and inl respectively.
*/
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

/*
Forces the CPU to wait for an I/O operation to complete. only use this when there's nothing like a status register or an IRQ to tell you the info has been received.
*/
static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}