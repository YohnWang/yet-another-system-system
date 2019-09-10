OUTPUT_ARCH( "riscv" )

ENTRY( _start )

SECTIONS
{
	. = 0x80000000;
	.text : 
			{ 
				*(.text) 
			}
			
	.rodata :
			{
				*(.rodata)
			}
							
	.data : 
			{
				*(.data) 
				*(.sdata)
			}

	__bss_start = . ;
	.sbss :
		{
			*(.sbss) 
		}
			
	.bss  : 
			{ 
				*(.bss) 
				*(COMMON) 
			}
	__bss_end = .; 
}
