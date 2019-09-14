#include<encoding.h>
#include<cpu.h>
#include<clint.h>

/* 0000 msip hart 0
 * 0004 msip hart 1
 * 4000 mtimecmp hart 0 lo
 * 4004 mtimecmp hart 0 hi
 * 4008 mtimecmp hart 1 lo
 * 400c mtimecmp hart 1 hi
 * bff8 mtime lo
 * bffc mtime hi
 */
 
#define time_addr     (CLINT_BASE+0xbff8)
#define timecmp_addr0  (CLINT_BASE+0x4000)
#define timecmp_addr1  (CLINT_BASE+0x4008)

#define time (*(volatile uint64_t*)time_addr)
#define timecmp0 (*(volatile uint64_t*)timecmp_addr0)
#define timecmp1 (*(volatile uint64_t*)timecmp_addr1)
 
time_t read_time()
{
	return time;
}

void sync_timecmp0()
{
	timecmp0=time;
}

void add_timecmp0(int64_t offset)
{
	timecmp0+=offset;
}


