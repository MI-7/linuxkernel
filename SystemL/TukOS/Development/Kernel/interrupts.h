#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

extern void int_general();		// Default interrupt routine
extern void int_0();
extern void int_1();
extern void int_2();
extern void int_3();
extern void int_4();
extern void int_5();
extern void int_6();
extern void int_7();
extern void int_8();
extern void int_9();
extern void int_10();
extern void int_11();
extern void int_12();
extern void int_13();
extern void int_14();
extern void int_15();
extern void int_16();
extern void int_17();
extern void int_18();
extern void int_19();
extern void int_20();
extern void int_21();
extern void int_22();
extern void int_23();
extern void int_24();
extern void int_25();
extern void int_26();
extern void int_27();
extern void int_28();
extern void int_29();
extern void int_30();
extern void int_31();
extern void int_32();
extern void int_33();			// Keyboard interrupt

unsigned char* idt_init();
unsigned int idt_entry(unsigned int, void*, unsigned short, unsigned char);
unsigned int remappic();

void isr_general();			// Default interrupt handler
void isr_33();				

struct idtstruct {
	unsigned short offset1;
	unsigned short selector;
	unsigned char unused;
	unsigned char flag;
	unsigned short offset2;
} __attribute__((packed));		// Packed to avoid padding

struct idtpointer {
	unsigned short size;
	unsigned int offset;	
} __attribute__((packed));		// Packed to avoid padding

#endif
