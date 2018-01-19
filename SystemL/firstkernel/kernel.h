#ifndef KERNEL_H
#define KERNEL_H

/* Check if the compiler thinks we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include "terminal.h"
#include "gdt.h"
#include "test.h"
#include "interrupt.h"
#include "idt.h"

#endif
