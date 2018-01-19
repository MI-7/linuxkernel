OUTPUT_FORMAT(binary)
STARTUP(entry.o)
SECTIONS
{
	. = 0x500;

	.text  0x500 : {
	*(.text)
	}
}
