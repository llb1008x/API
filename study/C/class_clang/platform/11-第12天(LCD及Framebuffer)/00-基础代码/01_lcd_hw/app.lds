SECTIONS { 
  . = 0x40000000;
 .text :
	{
		*(.text)
	}
  .data ALIGN(4) : { *(.data) }
  .rodata ALIGN(4) : { *(.rodata) }
  .bss ALIGN(4)  : { *(.bss) }
} 
