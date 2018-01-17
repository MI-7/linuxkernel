#ifndef GDT_H
#define GDT_H

#include <stddef.h>
#include <stdint.h>

/* gdt entry = 64 bits long
   0:15(16)  = limit[0:15]
   16:31(16) = base[0:15]
   32:39(8)  = base[16:23]
   40:47(8)  = access byte
   48:51(4)  = limit[16:19]
   52:55(4)  = flags
   56:63(8)  = base[24:31]
*/
struct GDT {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
}__attribute__((packed));

struct GDT_PTR {
	uint16_t limit;
	uint32_t base;
}__attribute__((packed));

struct GDT gdt[3];
struct GDT_PTR gdt_ptr;

extern void _gdt_flush();

void gdt_create_entries(uint8_t entry_num, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);
void gdt_initialize();

#endif
