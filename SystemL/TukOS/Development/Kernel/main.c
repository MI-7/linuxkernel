#include "video.h"
#include "common.h"
#include "memory.h"
#include "interrupts.h"
#include "stdio.h"
#include "shell.h"

int main()
{
	video_init();	
	idt_init();	
	shell();

return 0;
}

