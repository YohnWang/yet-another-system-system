include ./Makefile.inc


INCLUDES = \
	-I. \
	-I./init \
	-I./cpu \
	-I./kernel


CFLAGS =  \
	$(WARNINGS) $(INCLUDES)\
	 -fno-strict-aliasing -fno-builtin \
	-D__gracefulExit -mcmodel=medany -D_auto=__auto_type -O2#-fPIC
#/*-fomit-frame-pointer*/
GCCVER 	= $(shell $(GCC) --version | grep gcc | cut -d" " -f9)

#
# Define all object files.
#
OBJS = ./init/start.o ./init/init.o  \
		./cpu/trap.o ./cpu/traps.o ./cpu/time.o ./cpu/host.o \
		./kernel/task.o ./kernel/sem.o ./kernel/alloc.o ./kernel/clock.o ./kernel/rrsched.o ./kernel/fixed.o ./kernel/prio.o ./kernel/ds.o \
		main.o 
		

LDFLAGS	 = -T link.ld -nostartfiles -static -nostdlib
LIBS	 = -L$(CCPATH)/lib/gcc/$(TARGET)/$(GCCVER) \
		   -L$(CCPATH)/$(TARGET)/lib \
		   -lc -lgcc

%.o: %.c
	@echo "    CC $<"
	@$(GCC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	@echo "    CC $<"
	@$(GCC) -c $(CFLAGS) -o $@ $<

all: $(PROG).elf

$(PROG).elf  : $(OBJS) Makefile 
	@echo Linking....
	@$(GCC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)
	@$(OBJDUMP) -S $(PROG).elf > $(PROG).asm	
	@echo Completed $@

clean :
	-find . -name "*.o" | xargs rm 
	-find . -name "*.elf" | xargs rm 
	-find . -name "*.out" | xargs rm 
	-rm $(PROG).asm

force_true:
	@true

#-------------------------------------------------------------
sim: all
	spike $(PROG).elf


