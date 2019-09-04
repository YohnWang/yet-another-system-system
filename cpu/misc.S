	.text
	.global get_mstatus
	.align 4
get_mstatus:
	csrr a0,mstatus
	ret 
	
	.global set_mstatus
set_mstatus:
	csrw mstatus,a0
	ret 
	
	.global get_mstatus_and_disable_int
get_mstatus_and_disable_int:
	csrr a0,mstatus
	csrci mstatus,0x8
	ret 
