OUTPUT_ARCH( "riscv" )

ENTRY( _start )

SECTIONS
{
	. = 0x80000000;
	.text : 
			{ 
				*(.text) 
			}
			
	.data : 
			{
				*(.rodata)  
				*(.data) 
			}
			
	.bss  : 
			{ 
				__bss_start = . ;
				*(.sbss) 
				*(.bss) 
				*(COMMON) 
				__bss_end = .; 
			}
			
	.heap : 
			{
				. = ALIGN(4);
				_heap = .;
				_heap_start = .;
				. += 0x4000;
				_heap_end = .;
    		}
    		
	.stack : 
			{ 
				. += 0x4000; 
				. = ALIGN(4); 
				_stack = .;
			}
}
