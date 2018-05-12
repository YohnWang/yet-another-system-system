include ./Makefile.inc


INCLUDES = \
	-I. \
	-I./init \
	-I./schedule \
	-I./cpu \
	-I./core


CFLAGS =  \
	$(WARNINGS) $(INCLUDES) \
	-fomit-frame-pointer -fno-strict-aliasing -fno-builtin \
	-D__gracefulExit -mcmodel=medany -D_auto=__auto_type #-fPIC

GCCVER 	= $(shell $(GCC) --version | grep gcc | cut -d" " -f9)

#
# Define all object files.
#
OBJS = ./init/start.o ./init/init.o  main.o ./cpu/trap.o ./cpu/time.o ./core/task.o ./core/sem.o

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
	spike -l $(PROG).elf


